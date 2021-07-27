#pragma once

struct lin_type { explicit constexpr lin_type() {} };
struct log_type { explicit constexpr log_type() {} };

static constexpr lin_type lin_style{};
static constexpr log_type log_style{};

namespace Catch
{
	namespace Benchmark
	{
		struct Groupometer : Chronometer
		{
			Groupometer(Detail::ChronometerConcept & meter, int k, int s)
				: Chronometer(meter, k)
				, s(s)
			{}

			int size() const { return s; }

		private:

			int s;
		};

		namespace Detail
		{
			class Dump
			{
			public:

				struct group
				{
					const char * name;
					std::vector<unsigned long long> ys; // xs.size() * samples

					group(const char * name, std::size_t size)
						: name(name)
						, ys(size)
					{}
				};

				const char * filename;

				int type; // linear(0) or logarithmic(1)
				int reps;

				std::vector<unsigned int> xs;
				std::vector<group> groups;

				~Dump()
				{
					Catch::IConfigPtr cfg = Catch::getCurrentContext().getConfig();

#if defined(_MSC_VER)
					FILE * file;
					if (ful_check(fopen_s(&file, filename, "wb") == 0))
#else
					FILE * const file = std::fopen(filename, "wb");
					if (ful_check(file))
#endif
					{
						std::fprintf(file, type == 0 ? "style plot\n" : "style loglog\n");
						std::fprintf(file, "results %u\n", static_cast<unsigned int>(groups.size()));

						for (unsigned int i = 0; i < groups.size(); i++)
						{
							std::fprintf(file, "name %s\n", groups[i].name);
							std::fprintf(file, "points %d\n", static_cast<unsigned int>(xs.size()));
							std::fprintf(file, "samples %d\n", static_cast<unsigned int>(cfg->benchmarkSamples()));
							auto * y_ptr = groups[i].ys.data();
							for (auto size : xs)
							{
								std::fprintf(file, "x %llu\ny", static_cast<unsigned long long>(size));
								for (int s = 0; s < cfg->benchmarkSamples(); s++)
								{
									std::fprintf(file, " %llu", static_cast<unsigned long long>(*y_ptr));
									y_ptr++;
								}
								std::fprintf(file, "\n");
							}
						}

						std::fclose(file);
					}
				}

				explicit Dump(const char * filename, lin_type, int reps, unsigned int from, unsigned int to, unsigned int step)
					: filename(filename)
					, type(0)
					, reps(reps)
				{
					xs.resize(((to - from) - 1) / step + 1);

					auto begin = xs.begin();
					auto end = xs.end();
					for (; begin != end;)
					{
						*begin = from;
						begin++;
						from += step;
					}
				}

				explicit Dump(const char * filename, log_type, int reps, unsigned char from, unsigned char to)
					: filename(filename)
					, type(1)
					, reps(reps)
				{
					xs.resize((to - from) * 2);

					unsigned int size = 1u << from;

					auto begin = xs.begin();
					auto end = xs.end();
					for (; begin != end;)
					{
						*begin = size;
						begin++;
						*begin = size + (size >> 1);
						begin++;
						size = size << 1;
					}
				}

				template <typename F>
				Dump & operator = (F && f)
				{
					Catch::IConfigPtr cfg = Catch::getCurrentContext().getConfig();

					Catch::Benchmark::Detail::ChronometerModel<Catch::Benchmark::default_clock> meter;

					auto * y_ptr = groups.back().ys.data();
					for (auto size : xs)
					{
						for (int s = 0; s < cfg->benchmarkSamples(); s++)
						{
							f(Catch::Benchmark::Groupometer(meter, reps, size));

							*y_ptr = std::chrono::duration_cast<std::chrono::nanoseconds>(meter.elapsed()).count();
							y_ptr++;
						}
					}

					return *this;
				}

				explicit operator bool() const { return true; } // hack

				Dump & add(const char * name)
				{
					Catch::IConfigPtr cfg = Catch::getCurrentContext().getConfig();

					groups.emplace_back(name, xs.size() * cfg->benchmarkSamples());

					return *this;
				}

			};
		}
	}
}

#define BENCHMARK_DUMP(...) if (Catch::Benchmark::Detail::Dump my_little_secret{__VA_ARGS__})
#define BENCHMARK_GROUP(...) my_little_secret.add(__VA_ARGS__) = [&]

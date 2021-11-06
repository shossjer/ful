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
			Groupometer(Detail::ChronometerConcept & meter, int k, int g, size_t s)
				: Chronometer(meter, k)
				, g(g)
				, s(s)
			{}

			int group() const { return g; }
			size_t size() const { return s; }

		private:

			int g;
			size_t s;
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
				const char * xlabel;

				int type; // linear(0) or logarithmic(1)
				//int reps;

				std::vector<unsigned int> xs;
				std::vector<group> groups;

				~Dump()
				{
					Catch::IConfigPtr cfg = Catch::getCurrentContext().getConfig();

#if defined(_MSC_VER)
					FILE * file;
					if (fopen_s(&file, filename, "wb") == 0)
#else
					FILE * const file = std::fopen(filename, "wb");
					if (file)
#endif
					{
						std::fprintf(file, type == 0 ? "style plot\n" : "style loglog\n");
						std::fprintf(file, "xlabel %s\n", xlabel);
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

				explicit Dump(const char * filename, lin_type/*, int reps*/, unsigned int from, unsigned int to, unsigned int step, const char * xlabel = "")
					: filename(filename)
					, xlabel(xlabel)
					, type(0)
					//, reps(reps)
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

				explicit Dump(const char * filename, log_type/*, int reps*/, unsigned char from, unsigned char to, const char * xlabel = "")
					: filename(filename)
					, xlabel(xlabel)
					, type(1)
					//, reps(reps)
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
					using Clock = Catch::Benchmark::default_clock;

					Catch::IConfigPtr cfg = Catch::getCurrentContext().getConfig();

					auto env = Detail::measure_environment<Clock>();

					auto min_time = env.clock_resolution.mean * Detail::minimum_ticks;
					auto run_time = std::max(min_time, std::chrono::duration_cast<decltype(min_time)>(cfg->benchmarkWarmupTime()));

					Catch::Benchmark::Detail::ChronometerModel<Clock> meter;

					auto * y_ptr = groups.back().ys.data();
					for (unsigned int sizei = 0; sizei < xs.size(); sizei++)
					{
						const unsigned int size = xs[sizei];

						auto how_long = std::chrono::duration_cast<ClockDuration<Clock>>(run_time);
						ClockDuration<Clock> test_elapsed(0);
						auto test_iterations = 1;
						while (test_iterations < (1 << 30)) {
							//Catch::Benchmark::Detail::ChronometerModel<Clock> meter;
							f(Catch::Benchmark::Groupometer(meter, test_iterations, static_cast<int>(sizei), size));

							if (meter.elapsed() >= how_long) {
								test_elapsed = meter.elapsed();
								break;
							}
							test_iterations *= 2;
						}
						if (test_elapsed.count() == 0)
							throw optimized_away_error{};

						unsigned int new_iters = static_cast<unsigned int>(std::ceil(min_time * test_iterations / test_elapsed));

						for (int s = 0; s < cfg->benchmarkSamples(); s++)
						{
							f(Catch::Benchmark::Groupometer(meter, static_cast<int>(new_iters), static_cast<int>(sizei), size));

							*y_ptr = static_cast<unsigned long long>(std::chrono::duration_cast<std::chrono::nanoseconds>(meter.elapsed()).count()) / new_iters; // todo division results in loss of precision
							y_ptr++;
						}
					}

					return *this;
				}

				explicit operator bool() const { return true; } // hack

				Dump & add(const char * name)
				{
					Catch::IConfigPtr cfg = Catch::getCurrentContext().getConfig();

					groups.emplace_back(name, xs.size() * static_cast<size_t>(cfg->benchmarkSamples()));

					return *this;
				}

			};
		}
	}
}

#define BENCHMARK_DUMP(...) if (Catch::Benchmark::Detail::Dump my_little_secret{__VA_ARGS__})
#define BENCHMARK_GROUP(...) my_little_secret.add(__VA_ARGS__) = [&]

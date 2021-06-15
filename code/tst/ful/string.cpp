#include "ful/string.hpp"

#include "buffer.hpp"

#include <catch2/catch.hpp>

namespace
{
	void mem_test(unsigned long long size)
	{
		const long long offset = 1;
		const long long shift = 1;

		buffer_utf8 buffer;
		buffer.allocate(offset + size + shift + size);

		for (int i = 0; i < size; i++)
		{
			buffer.data()[offset + size + shift + i] = static_cast<ful::char8>(i % (127 - 32) + 32);
			buffer.data()[offset + i] = static_cast<ful::char8>(126 - i % (127 - 32));
		}

		ful::char8 * const copy_begin = reinterpret_cast<ful::char8 *>(buffer.data() + offset + size + shift);
		ful::char8 * const copy_end = reinterpret_cast<ful::char8 *>(buffer.data() + offset + size + shift + size);
		ful::char8 * const swap_begin = reinterpret_cast<ful::char8 *>(buffer.data() + offset);
		ful::char8 * const swap_end = reinterpret_cast<ful::char8 *>(buffer.data() + offset + size);

		if (16u < size)
		{
#if defined(__AVX__)
			SECTION("avx")
			{
				CHECK(ful::detail::memcopy_avx(copy_begin, copy_end, copy_begin - shift) == copy_end - shift);
				for (int i = 0; i < size; i++)
				{
					CHECK((copy_begin - shift)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}

				CHECK(ful::detail::memypoc_avx(copy_begin - shift, copy_end - shift, copy_end) == copy_begin);
				for (int i = 0; i < size; i++)
				{
					CHECK(copy_begin[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}

				CHECK(ful::detail::memswap_avx(copy_begin, copy_end, swap_begin) == swap_end);
				for (int i = 0; i < size; i++)
				{
					CHECK(swap_begin[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
					CHECK(copy_begin[i] == static_cast<ful::char8>(126 - i % (127 - 32)));
				}

				ful::detail::memset8_avx(copy_begin, copy_end, static_cast<ful::char8>(7));
				for (int i = 0; i < size; i++)
				{
					CHECK(copy_begin[i] == static_cast<ful::char8>(7));
				}
			}
#endif

#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			SECTION("sse")
			{
				CHECK(ful::detail::memcopy_sse(copy_begin, copy_end, copy_begin - shift) == copy_end - shift);
				for (int i = 0; i < size; i++)
				{
					CHECK((copy_begin - shift)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}

				CHECK(ful::detail::memypoc_sse(copy_begin - shift, copy_end - shift, copy_end) == copy_begin);
				for (int i = 0; i < size; i++)
				{
					CHECK(copy_begin[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}

				CHECK(ful::detail::memswap_sse(copy_begin, copy_end, swap_begin) == swap_end);
				for (int i = 0; i < size; i++)
				{
					CHECK(swap_begin[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
					CHECK(copy_begin[i] == static_cast<ful::char8>(126 - i % (127 - 32)));
				}

				ful::detail::memset8_sse(copy_begin, copy_end, static_cast<ful::char8>(7));
				for (int i = 0; i < size; i++)
				{
					CHECK(copy_begin[i] == static_cast<ful::char8>(7));
				}
			}
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			SECTION("sse2")
			{
				CHECK(ful::detail::memcopy_sse2(copy_begin, copy_end, copy_begin - shift) == copy_end - shift);
				for (int i = 0; i < size; i++)
				{
					CHECK((copy_begin - shift)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}

				CHECK(ful::detail::memypoc_sse2(copy_begin - shift, copy_end - shift, copy_end) == copy_begin);
				for (int i = 0; i < size; i++)
				{
					CHECK(copy_begin[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}

				CHECK(ful::detail::memswap_sse2(copy_begin, copy_end, swap_begin) == swap_end);
				for (int i = 0; i < size; i++)
				{
					CHECK(swap_begin[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
					CHECK(copy_begin[i] == static_cast<ful::char8>(126 - i % (127 - 32)));
				}

				ful::detail::memset8_sse2(copy_begin, copy_end, static_cast<ful::char8>(7));
				for (int i = 0; i < size; i++)
				{
					CHECK(copy_begin[i] == static_cast<ful::char8>(7));
				}
			}
#endif

			SECTION("none")
			{
				CHECK(ful::detail::memcopy_none(copy_begin, copy_end, copy_begin - shift) == copy_end - shift);
				for (int i = 0; i < size; i++)
				{
					CHECK((copy_begin - shift)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}

				CHECK(ful::detail::memypoc_none(copy_begin - shift, copy_end - shift, copy_end) == copy_begin);
				for (int i = 0; i < size; i++)
				{
					CHECK(copy_begin[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}

				CHECK(ful::detail::memswap_none(copy_begin, copy_end, swap_begin) == swap_end);
				for (int i = 0; i < size; i++)
				{
					CHECK(swap_begin[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
					CHECK(copy_begin[i] == static_cast<ful::char8>(126 - i % (127 - 32)));
				}

				ful::detail::memset8_none(copy_begin, copy_end, static_cast<ful::char8>(7));
				for (int i = 0; i < size; i++)
				{
					CHECK(copy_begin[i] == static_cast<ful::char8>(7));
				}
			}
		}

		SECTION("default")
		{
			CHECK(ful::memcopy(copy_begin, copy_end, copy_begin - shift) == copy_end - shift);
			for (int i = 0; i < size; i++)
			{
				CHECK((copy_begin - shift)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
			}

			CHECK(ful::memypoc(copy_begin - shift, copy_end - shift, copy_end) == copy_begin);
			for (int i = 0; i < size; i++)
			{
				CHECK(copy_begin[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
			}

			CHECK(ful::memswap(copy_begin, copy_end, swap_begin) == swap_end);
			for (int i = 0; i < size; i++)
			{
				CHECK(swap_begin[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				CHECK(copy_begin[i] == static_cast<ful::char8>(126 - i % (127 - 32)));
			}

			ful::memset(copy_begin, copy_end, static_cast<ful::char8>(7));
			for (int i = 0; i < size; i++)
			{
				CHECK(copy_begin[i] == static_cast<ful::char8>(7));
			}
		}
	}
}

TEST_CASE("mem", "")
{
	SECTION("nullptr")
	{
		CHECK(ful::memcopy(nullptr, nullptr, nullptr) == nullptr);
		CHECK(ful::memypoc(nullptr, nullptr, nullptr) == nullptr);
	}

	SECTION("size 1")
	{
		mem_test(1);
	}

	SECTION("size 3")
	{
		mem_test(3);
	}

	SECTION("size 5")
	{
		mem_test(5);
	}

	SECTION("size 7")
	{
		mem_test(7);
	}

	SECTION("size 9")
	{
		mem_test(9);
	}

	SECTION("size 15")
	{
		mem_test(15);
	}

	SECTION("size 17")
	{
		mem_test(17);
	}

	SECTION("size 31")
	{
		mem_test(31);
	}

	SECTION("size 33")
	{
		mem_test(33);
	}

	SECTION("size 63")
	{
		mem_test(63);
	}

	SECTION("size 65")
	{
		mem_test(65);
	}

	SECTION("size 127")
	{
		mem_test(127);
	}

	SECTION("size 129")
	{
		mem_test(129);
	}

	SECTION("size 255")
	{
		mem_test(255);
	}

	SECTION("size 257")
	{
		mem_test(257);
	}
}

#include "ful/string.hpp"

#include "buffer.hpp"

#include <catch2/catch.hpp>

namespace
{
	void copy_rcopy_test(unsigned long long size, unsigned int from, unsigned int to)
	{
		buffer_utf8 buffer;
		buffer.allocate((from < to ? to : from) + size);

		for (int i = 0; i < size; i++)
		{
			buffer.data()[from + i] = static_cast<ful::char8>(i % (126 - 32) + 32);
		}

		ful::char8 * const from_begin = reinterpret_cast<ful::char8 *>(buffer.data() + from);
		ful::char8 * const from_end = reinterpret_cast<ful::char8 *>(buffer.data() + from + size);
		ful::char8 * const to_begin = reinterpret_cast<ful::char8 *>(buffer.data() + to);
		ful::char8 * const to_end = reinterpret_cast<ful::char8 *>(buffer.data() + to + size);

		if (16u < size)
		{
#if defined(__AVX__)
			SECTION("avx")
			{
				CHECK(ful::detail::copy_8_avx(from_begin, from_end, to_begin) == to_end);
				for (int i = 0; i < size; i++)
				{
					CHECK(buffer.data()[to + i] == static_cast<ful::char8>(i % (126 - 32) + 32));
				}

				CHECK(ful::detail::rcopy_8_avx(to_begin, to_end, from_end) == from_begin);
				for (int i = 0; i < buffer.size() - 2; i++)
				{
					CHECK(buffer.data()[from + i] == static_cast<ful::char8>(i % (126 - 32) + 32));
				}
			}
#endif

#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			SECTION("sse")
			{
				CHECK(ful::detail::copy_8_sse(from_begin, from_end, to_begin) == to_end);
				for (int i = 0; i < size; i++)
				{
					CHECK(buffer.data()[to + i] == static_cast<ful::char8>(i % (126 - 32) + 32));
				}

				CHECK(ful::detail::rcopy_8_sse(to_begin, to_end, from_end) == from_begin);
				for (int i = 0; i < buffer.size() - 2; i++)
				{
					CHECK(buffer.data()[from + i] == static_cast<ful::char8>(i % (126 - 32) + 32));
				}
			}
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			SECTION("sse2")
			{
				CHECK(ful::detail::copy_8_sse2(from_begin, from_end, to_begin) == to_end);
				for (int i = 0; i < size; i++)
				{
					CHECK(buffer.data()[to + i] == static_cast<ful::char8>(i % (126 - 32) + 32));
				}

				CHECK(ful::detail::rcopy_8_sse2(to_begin, to_end, from_end) == from_begin);
				for (int i = 0; i < buffer.size() - 2; i++)
				{
					CHECK(buffer.data()[from + i] == static_cast<ful::char8>(i % (126 - 32) + 32));
				}
			}
#endif

			SECTION("none")
			{
				CHECK(ful::detail::copy_8_none(from_begin, from_end, to_begin) == to_end);
				for (int i = 0; i < size; i++)
				{
					CHECK(buffer.data()[to + i] == static_cast<ful::char8>(i % (126 - 32) + 32));
				}

				CHECK(ful::detail::rcopy_8_none(to_begin, to_end, from_end) == from_begin);
				for (int i = 0; i < buffer.size() - 2; i++)
				{
					CHECK(buffer.data()[from + i] == static_cast<ful::char8>(i % (126 - 32) + 32));
				}
			}
		}

		SECTION("default")
		{
			CHECK(ful::copy(from_begin, from_end, to_begin) == to_end);
			for (int i = 0; i < size; i++)
			{
				CHECK(buffer.data()[to + i] == static_cast<ful::char8>(i % (126 - 32) + 32));
			}

			CHECK(ful::rcopy(to_begin, to_end, from_end) == from_begin);
			for (int i = 0; i < buffer.size() - 2; i++)
			{
				CHECK(buffer.data()[from + i] == static_cast<ful::char8>(i % (126 - 32) + 32));
			}
		}
	}
}

TEST_CASE("copy/rcopy", "")
{
	SECTION("nullptr")
	{
		CHECK(ful::copy(nullptr, nullptr, nullptr) == nullptr);
		CHECK(ful::rcopy(nullptr, nullptr, nullptr) == nullptr);
	}

	SECTION("size 1")
	{
		copy_rcopy_test(1, 2, 1);
	}

	SECTION("size 3")
	{
		copy_rcopy_test(3, 2, 1);
	}

	SECTION("size 5")
	{
		copy_rcopy_test(5, 2, 1);
	}

	SECTION("size 7")
	{
		copy_rcopy_test(7, 2, 1);
	}

	SECTION("size 9")
	{
		copy_rcopy_test(9, 2, 1);
	}

	SECTION("size 15")
	{
		copy_rcopy_test(15, 2, 1);
	}

	SECTION("size 17")
	{
		copy_rcopy_test(17, 2, 1);
	}

	SECTION("size 31")
	{
		copy_rcopy_test(31, 2, 1);
	}

	SECTION("size 33")
	{
		copy_rcopy_test(33, 2, 1);
	}

	SECTION("size 63")
	{
		copy_rcopy_test(63, 2, 1);
	}

	SECTION("size 65")
	{
		copy_rcopy_test(65, 2, 1);
	}

	SECTION("size 127")
	{
		copy_rcopy_test(127, 2, 1);
	}

	SECTION("size 129")
	{
		copy_rcopy_test(129, 2, 1);
	}

	SECTION("size 255")
	{
		copy_rcopy_test(255, 2, 1);
	}

	SECTION("size 257")
	{
		copy_rcopy_test(257, 2, 1);
	}
}

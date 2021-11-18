#include "ful/string.hpp"
#include "ful/string_init.hpp"

#include "buffer.hpp"

#include <catch2/catch.hpp>

namespace
{
	void mem_test(unsigned long long size)
	{
		const long long offset = 1;

		buffer_utf8 buffer1;
		buffer1.allocate(offset + size);
		buffer_utf8 buffer2;
		buffer2.allocate(offset + size);

		for (int i = 0; i < size; i++)
		{
			buffer1.data()[offset + i] = static_cast<ful::char8>(i % (127 - 32) + 32);
			buffer2.data()[offset + i] = static_cast<ful::char8>(0);
		}

		ful::byte * const copy_begin = reinterpret_cast<ful::byte *>(buffer1.data() + offset);
		ful::byte * const copy_end = reinterpret_cast<ful::byte *>(buffer1.data() + offset + size);
		ful::byte * const swap_begin = reinterpret_cast<ful::byte *>(buffer2.data() + offset);
		ful::byte * const swap_end = reinterpret_cast<ful::byte *>(buffer2.data() + offset + size);

#if defined(__AVX__)
		if (64u < size)
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
		if (32u < size)
#else
		if (16u < size)
#endif
		{
#if defined(__AVX__)
			SECTION("avx")
			{
				CHECK(ful::detail::memcopy_avx(copy_begin, copy_end, swap_begin) == swap_end);
				for (int i = 0; i < size; i++)
				{
					CHECK((copy_begin)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
					CHECK((swap_begin)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}

				CHECK(ful::detail::memmovef_avx(swap_begin, swap_end, swap_begin - offset) == swap_end - offset);
				for (int i = 0; i < size; i++)
				{
					CHECK((swap_begin - offset)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}

				CHECK(ful::detail::memmover_avx(swap_begin - offset, swap_end - offset, swap_end) == swap_begin);
				for (int i = 0; i < size; i++)
				{
					CHECK((swap_begin)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}

				ful::detail::memset8_avx(reinterpret_cast<ful::char8 *>(swap_begin), reinterpret_cast<ful::char8 *>(swap_end), static_cast<ful::char8>(7));
				for (int i = 0; i < size; i++)
				{
					CHECK((swap_begin)[i] == static_cast<ful::char8>(7));
				}

				CHECK(ful::detail::memswap_avx(copy_begin, copy_end, swap_begin) == swap_end);
				for (int i = 0; i < size; i++)
				{
					CHECK((copy_begin)[i] == static_cast<ful::char8>(7));
					CHECK((swap_begin)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}
			}
#endif

#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			SECTION("sse")
			{
				CHECK(ful::detail::memcopy_sse(copy_begin, copy_end, swap_begin) == swap_end);
				for (int i = 0; i < size; i++)
				{
					CHECK((copy_begin)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
					CHECK((swap_begin)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}

				CHECK(ful::detail::memmovef_sse(swap_begin, swap_end, swap_begin - offset) == swap_end - offset);
				for (int i = 0; i < size; i++)
				{
					CHECK((swap_begin - offset)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}

				CHECK(ful::detail::memmover_sse(swap_begin - offset, swap_end - offset, swap_end) == swap_begin);
				for (int i = 0; i < size; i++)
				{
					CHECK((swap_begin)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}

				ful::detail::memset8_sse(reinterpret_cast<ful::char8 *>(swap_begin), reinterpret_cast<ful::char8 *>(swap_end), static_cast<ful::char8>(7));
				for (int i = 0; i < size; i++)
				{
					CHECK((swap_begin)[i] == static_cast<ful::char8>(7));
				}

				CHECK(ful::detail::memswap_sse(copy_begin, copy_end, swap_begin) == swap_end);
				for (int i = 0; i < size; i++)
				{
					CHECK((copy_begin)[i] == static_cast<ful::char8>(7));
					CHECK((swap_begin)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}
			}
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			SECTION("sse2")
			{
				CHECK(ful::detail::memcopy_sse2(copy_begin, copy_end, swap_begin) == swap_end);
				for (int i = 0; i < size; i++)
				{
					CHECK((copy_begin)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
					CHECK((swap_begin)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}

				CHECK(ful::detail::memmovef_sse2(swap_begin, swap_end, swap_begin - offset) == swap_end - offset);
				for (int i = 0; i < size; i++)
				{
					CHECK((swap_begin - offset)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}

				CHECK(ful::detail::memmover_sse2(swap_begin - offset, swap_end - offset, swap_end) == swap_begin);
				for (int i = 0; i < size; i++)
				{
					CHECK((swap_begin)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}

				ful::detail::memset8_sse2(reinterpret_cast<ful::char8 *>(swap_begin), reinterpret_cast<ful::char8 *>(swap_end), static_cast<ful::char8>(7));
				for (int i = 0; i < size; i++)
				{
					CHECK((swap_begin)[i] == static_cast<ful::char8>(7));
				}

				CHECK(ful::detail::memswap_sse2(copy_begin, copy_end, swap_begin) == swap_end);
				for (int i = 0; i < size; i++)
				{
					CHECK((copy_begin)[i] == static_cast<ful::char8>(7));
					CHECK((swap_begin)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}
			}
#endif

			SECTION("none")
			{
				CHECK(ful::detail::memcopy_none(copy_begin, copy_end, swap_begin) == swap_end);
				for (int i = 0; i < size; i++)
				{
					CHECK((copy_begin)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
					CHECK((swap_begin)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}

				CHECK(ful::detail::memmovef_none(swap_begin, swap_end, swap_begin - offset) == swap_end - offset);
				for (int i = 0; i < size; i++)
				{
					CHECK((swap_begin - offset)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}

				CHECK(ful::detail::memmover_none(swap_begin - offset, swap_end - offset, swap_end) == swap_begin);
				for (int i = 0; i < size; i++)
				{
					CHECK((swap_begin)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}

				ful::detail::memset8_none(reinterpret_cast<ful::char8 *>(swap_begin), reinterpret_cast<ful::char8 *>(swap_end), static_cast<ful::char8>(7));
				for (int i = 0; i < size; i++)
				{
					CHECK((swap_begin)[i] == static_cast<ful::char8>(7));
				}

				CHECK(ful::detail::memswap_none(copy_begin, copy_end, swap_begin) == swap_end);
				for (int i = 0; i < size; i++)
				{
					CHECK((copy_begin)[i] == static_cast<ful::char8>(7));
					CHECK((swap_begin)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				}
			}
		}

		SECTION("default")
		{
			CHECK(ful::memcopy(copy_begin, copy_end, swap_begin) == swap_end);
			for (int i = 0; i < size; i++)
			{
				CHECK((copy_begin)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
				CHECK((swap_begin)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
			}

			CHECK(ful::memmovef(swap_begin, swap_end, swap_begin - offset) == swap_end - offset);
			for (int i = 0; i < size; i++)
			{
				CHECK((swap_begin - offset)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
			}

			CHECK(ful::memmover(swap_begin - offset, swap_end - offset, swap_end) == swap_begin);
			for (int i = 0; i < size; i++)
			{
				CHECK((swap_begin)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
			}

			ful::memset(reinterpret_cast<ful::char8 *>(swap_begin), reinterpret_cast<ful::char8 *>(swap_end), static_cast<ful::char8>(7));
			for (int i = 0; i < size; i++)
			{
				CHECK((swap_begin)[i] == static_cast<ful::char8>(7));
			}

			CHECK(ful::memswap(copy_begin, copy_end, swap_begin) == swap_end);
			for (int i = 0; i < size; i++)
			{
				CHECK((copy_begin)[i] == static_cast<ful::char8>(7));
				CHECK((swap_begin)[i] == static_cast<ful::char8>(i % (127 - 32) + 32));
			}
		}
	}
}

TEST_CASE("mem", "")
{
	SECTION("nullptr")
	{
		char * const null_ptr = nullptr;

		CHECK(ful::memcopy(null_ptr, null_ptr, null_ptr) == nullptr);
		CHECK(ful::memmovef(null_ptr, null_ptr, null_ptr) == nullptr);
		CHECK(ful::memmover(null_ptr, null_ptr, null_ptr) == nullptr);
		CHECK(ful::memswap(null_ptr, null_ptr, null_ptr) == nullptr);
		ful::memset(null_ptr, null_ptr, ful::char8{});
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

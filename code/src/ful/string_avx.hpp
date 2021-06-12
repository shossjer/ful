#pragma once

namespace ful
{
	namespace detail
	{
		ful_target("avx") inline
		char8 * copy_large_avx(const char8 * first, const char8 * last, char8 * begin)
		{
			if (!ful_expect(16 <= last - first))
				return begin;

			const usize size = last - first;
			if (size <= 32)
			{
				const auto first_half = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first));
				const auto last_half = _mm_loadu_si128(reinterpret_cast<const __m128i *>(last - 16));
				_mm_storeu_si128(reinterpret_cast<__m128i *>(begin), first_half);
				_mm_storeu_si128(reinterpret_cast<__m128i *>(begin + size - 16), last_half);

				return begin + size;
			}
			else if (size <= 64)
			{
				const auto first_word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));
				const auto last_word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last - 32));
				_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin), first_word);
				_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + size - 32), last_word);

				return begin + size;
			}
			else if (size <= 128)
			{
				const auto first_word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));
				const auto second_word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first + 32));
				const auto s2last_word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last - 64));
				const auto last_word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last - 32));
				_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin), first_word);
				_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + 32), second_word);
				_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + size - 64), s2last_word);
				_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + size - 32), last_word);

				return begin + size;
			}
			else
			{
				char8 * begin_aligned = reinterpret_cast<char8 *>(reinterpret_cast<puint>(begin + 32) & -32);
				const char8 * first_aligned = first + (begin_aligned - begin); // although it is not aligned

				const __m256i head = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));
				const __m256i head_aligned = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first_aligned));
				_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin), head);
				_mm256_store_si256(reinterpret_cast<__m256i *>(begin_aligned), head_aligned);

				first_aligned += 32;
				begin_aligned += 32;

				const char8 * const last_aligned = last - 64;
				while (first_aligned < last_aligned)
				{
					const __m256i word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first_aligned));
					_mm256_store_si256(reinterpret_cast<__m256i *>(begin_aligned), word);

					first_aligned += 32;
					begin_aligned += 32;
				}

				const __m256i tail_aligned = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first_aligned));
				const __m256i tail = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last - 32));
				_mm256_store_si256(reinterpret_cast<__m256i *>(begin_aligned), tail_aligned);
				_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin_aligned + (last - first_aligned) - 32), tail);

				return begin_aligned + (last - first_aligned);
			}
		}

		ful_target("avx") inline
		char8 * rcopy_large_avx(const char8 * first, const char8 * last, char8 * end)
		{
			if (!ful_expect(16 <= last - first))
				return end;

			const usize size = last - first;
			if (size <= 32)
			{
				const auto first_half = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first));
				const auto last_half = _mm_loadu_si128(reinterpret_cast<const __m128i *>(last - 16));
				_mm_storeu_si128(reinterpret_cast<__m128i *>(end - size), first_half);
				_mm_storeu_si128(reinterpret_cast<__m128i *>(end - 16), last_half);

				return end - size;
			}
			else if (size <= 64)
			{
				const auto first_word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));
				const auto last_word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last - 32));
				_mm256_storeu_si256(reinterpret_cast<__m256i *>(end - size), first_word);
				_mm256_storeu_si256(reinterpret_cast<__m256i *>(end - 32), last_word);

				return end - size;
			}
			else if (size <= 128)
			{
				const auto first_word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));
				const auto second_word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first + 32));
				const auto s2last_word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last - 64));
				const auto last_word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last - 32));
				_mm256_storeu_si256(reinterpret_cast<__m256i *>(end - size), first_word);
				_mm256_storeu_si256(reinterpret_cast<__m256i *>(end - size + 32), second_word);
				_mm256_storeu_si256(reinterpret_cast<__m256i *>(end - 64), s2last_word);
				_mm256_storeu_si256(reinterpret_cast<__m256i *>(end - 32), last_word);

				return end - size;
			}
			else
			{
				char8 * end_aligned = reinterpret_cast<char8 *>(reinterpret_cast<puint>(end - 1) & -32);
				const char8 * last_aligned = last - (end - end_aligned); // although it is not aligned

				const __m256i tail = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last - 32));
				const __m256i tail_aligned = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last_aligned - 32));
				_mm256_storeu_si256(reinterpret_cast<__m256i *>(end - 32), tail);
				_mm256_store_si256(reinterpret_cast<__m256i *>(end_aligned - 32), tail_aligned);

				last_aligned -= 32;
				end_aligned -= 32;

				const char8 * const first_aligned = first + 64;
				while (first_aligned < last_aligned)
				{
					const __m256i word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last_aligned - 32));
					_mm256_store_si256(reinterpret_cast<__m256i *>(end_aligned - 32), word);

					last_aligned -= 32;
					end_aligned -= 32;
				}

				const __m256i head_aligned = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last_aligned - 32));
				const __m256i head = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));
				_mm256_store_si256(reinterpret_cast<__m256i *>(end_aligned - 32), head_aligned);
				_mm256_storeu_si256(reinterpret_cast<__m256i *>(end_aligned - (last_aligned - first)), head);

				return end_aligned - (last_aligned - first);
			}
		}

		ful_target("avx") inline
		void fill_large_avx(char8 * from, char8 * to, char8 u)
		{
			const usize size = to - from;
			if (!ful_expect(16 <= size))
				return;

			if (size <= 32)
			{
				const __m128i u128 = _mm_set1_epi8(u);

				_mm_storeu_si128(reinterpret_cast<__m128i *>(from), u128);
				_mm_storeu_si128(reinterpret_cast<__m128i *>(to - 16), u128);
			}
			else if (size <= 64)
			{
				const __m256i u256 = _mm256_set1_epi8(u);

				_mm256_storeu_si256(reinterpret_cast<__m256i *>(from), u256);
				_mm256_storeu_si256(reinterpret_cast<__m256i *>(to - 32), u256);
			}
			else
			{
				const __m256i u256 = _mm256_set1_epi8(u);

				_mm256_storeu_si256(reinterpret_cast<__m256i *>(from), u256);

				from = ful_align_next_32(from);

				char8 * const to_word = to - 64;
				if (from < to_word)
				{
					do
					{
						_mm256_stream_si256(reinterpret_cast<__m256i *>(from), u256);
						_mm256_stream_si256(reinterpret_cast<__m256i *>(from + 32), u256);

						from += 64;
					}
					while (from < to_word);

					from = ful_align_next_32(to_word);
				}

				_mm256_stream_si256(reinterpret_cast<__m256i *>(from), u256);
				_mm_sfence();

				_mm256_storeu_si256(reinterpret_cast<__m256i *>(to_word + 32), u256);
			}
		}
	}
}

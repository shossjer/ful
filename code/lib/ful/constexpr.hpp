#pragma once

#include "ful/ranges.hpp"

namespace ful
{
	namespace cxp
	{
		namespace detail
		{
			template <typename T>
			constexpr T * strend(T * cstr)
			{
				return *cstr == 0 ? cstr
				     : strend(cstr + 1);
			}

			template <typename First, typename Last, typename Begin>
			constexpr bool match_bstr(First first, Last last, Begin begin)
			{
				return first == last ? true
				     : *first != *begin ? false
				     : match_bstr(first + 1, last, begin + 1);
			}

			template <typename First, typename Last, typename Begin, typename End, typename To>
			constexpr Begin find_bstr(First first, Last last, Begin begin, End end, To to)
			{
				return match_bstr(first, last, begin) ? begin
				     : begin == to ? end
				     : find_bstr(first, last, begin + 1, end, to);
			}

			template <typename First, typename Last, typename Begin, typename End, typename It>
			constexpr Begin rfind_bstr(First first, Last last, Begin begin, End end, It it)
			{
				return match_bstr(first, last, it) ? it
				     : it == begin ? end
				     : rfind_bstr(first, last, begin, end, it - 1);
			}
		}

		template <typename Begin, typename End, typename First, typename Last>
		constexpr bool equals(Begin begin, End end, First first, Last last)
		{
			return static_cast<usize>(end - begin) != static_cast<usize>(last - first) ? false
			     : detail::match_bstr(first, last, begin);
		}

		template <typename Begin, typename End, typename First, typename Last>
		constexpr Begin find(Begin begin, End end, First first, Last last)
		{
			return static_cast<usize>(end - begin) < static_cast<usize>(last - first) ? end
			     : detail::find_bstr(first, last, begin, end, end - static_cast<usize>(last - first));
		}

		template <typename R, typename T>
		constexpr auto find(R && x, const T * cstr)
			-> decltype(find(begin(x), end(x), cstr, detail::strend(cstr)))
		{
			return find(begin(x), end(x), cstr, detail::strend(cstr));
		}

		template <typename Begin, typename End, typename First, typename Last>
		constexpr Begin rfind(Begin begin, End end, First first, Last last)
		{
			return static_cast<usize>(end - begin) < static_cast<usize>(last - first) ? end
			     : detail::rfind_bstr(first, last, begin, end, end - static_cast<usize>(last - first));
		}

		template <typename R, typename T>
		constexpr auto rfind(R && x, const T * cstr)
			-> decltype(rfind(begin(x), end(x), cstr, detail::strend(cstr)))
		{
			return rfind(begin(x), end(x), cstr, detail::strend(cstr));
		}

		template <typename Begin, typename End, typename First, typename Last>
		constexpr bool starts_with(Begin begin, End end, First first, Last last)
		{
			return static_cast<usize>(end - begin) < static_cast<usize>(last - first) ? false
			     : detail::match_bstr(first, last, begin);
		}

		template <typename R, typename T>
		constexpr auto starts_with(const R & x, const T * cstr)
			-> decltype(starts_with(begin(x), end(x), cstr, detail::strend(cstr)))
		{
			return starts_with(begin(x), end(x), cstr, detail::strend(cstr));
		}
	}
}

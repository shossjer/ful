#pragma once

namespace ful
{
	namespace hck
	{
		template <bool Value>
		struct bool_type
		{};

		using true_type = bool_type<true>;
		using false_type = bool_type<false>;

		namespace detail
		{
			template <typename T = int>
			static T requires_impl(hck::true_type);
		}
	}
}

#define ful_requires(...) decltype(ful::hck::detail::requires_impl(ful::hck::bool_type<(__VA_ARGS__)>{})) = 0

#pragma once

namespace std
{
	struct bidirectional_iterator_tag;
}

namespace ful
{
	namespace hck
	{
		template <bool Value>
		struct bool_type
		{};

		using true_type = bool_type<true>;
		using false_type = bool_type<false>;

		template <typename T>
		struct remove_reference { using type = T; };
		template <typename T>
		struct remove_reference<T &> { using type = T; };
		template <typename T>
		struct remove_reference<T &&> { using type = T; };
		template <typename T>
		using remove_reference_t = typename remove_reference<T>::type;

		template <typename T>
		T && forward(hck::remove_reference_t<T> & x) { return static_cast<T &&>(x); }
		template <typename T>
		T && forward(hck::remove_reference_t<T> && x) { return static_cast<T &&>(x); }

		template <typename T>
		static T && declval();

		namespace detail
		{
			template <typename T>
			static auto compact_type_impl(const T &, int) -> typename T::compact_type;
			template <typename T>
			static auto compact_type_impl(const T &, ...) -> T;
		}
		template <typename T>
		using compact_type_t = decltype(detail::compact_type_impl(hck::declval<T>(), 0));
		// note this is necessary due to char_fast24

		namespace detail
		{
			template <typename T = int>
			static T requires_impl(hck::true_type);
		}
	}
}

#define ful_requires(...) decltype(ful::hck::detail::requires_impl(ful::hck::bool_type<(__VA_ARGS__)>{})) = 0

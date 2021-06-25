#pragma once

namespace ful
{
#if defined(__SIZE_TYPE__)
	using usize = __SIZE_TYPE__;
#elif defined(_MSC_VER)
# if defined(_WIN64)
	using usize = unsigned __int64;
# else
	using usize = unsigned __int32;
# endif
#else
# error Missing implementation!
#endif

#if defined(__UINT64_TYPE__)
	using uint64 = __UINT64_TYPE__;
#elif defined(_MSC_VER)
	using uint64 = unsigned __int64;
#else
# error Missing implementation!
#endif

#if defined(__UINT32_TYPE__)
	using uint32 = __UINT32_TYPE__;
#elif defined(_MSC_VER)
	using uint32 = unsigned __int32;
#else
# error Missing implementation!
#endif

#if defined(__UINT16_TYPE__)
	using uint16 = __UINT16_TYPE__;
#elif defined(_MSC_VER)
	using uint16 = unsigned __int16;
#else
# error Missing implementation!
#endif

#if defined(__UINT8_TYPE__)
	using uint8 = __UINT8_TYPE__;
#elif defined(_MSC_VER)
	using uint8 = unsigned __int8;
#else
# error Missing implementation!
#endif

#if defined(__INT64_TYPE__)
	using sint64 = __INT64_TYPE__;
#elif defined(_MSC_VER)
	using sint64 = __int64;
#else
# error Missing implementation!
#endif

#if defined(__INT32_TYPE__)
	using sint32 = __INT32_TYPE__;
#elif defined(_MSC_VER)
	using sint32 = __int32;
#else
# error Missing implementation!
#endif

#if defined(__INT16_TYPE__)
	using sint16 = __INT16_TYPE__;
#elif defined(_MSC_VER)
	using sint16 = __int16;
#else
# error Missing implementation!
#endif

#if defined(__INT8_TYPE__)
	using sint8 = __INT8_TYPE__;
#elif defined(_MSC_VER)
	using sint8 = signed __int8;
#else
# error Missing implementation!
#endif

#if defined(__UINTPTR_TYPE__)
	using puint = __UINTPTR_TYPE__;
#elif defined(_MSC_VER)
	using puint = usize;
#else
# error Missing implementation!
#endif
	static_assert(sizeof(puint) == sizeof(void *), "puint must have the same size as a pointer!");

#if defined(__PTRDIFF_TYPE__)
	using pdiff = __PTRDIFF_TYPE__;
#elif defined(_MSC_VER)
# if defined(_WIN64)
	using pdiff = __int64;
# else
	using pdiff = __int32;
# endif
#else
# error Missing implementation!
#endif

	using ssize = pdiff;
	static_assert(sizeof(ssize) == sizeof(usize), "ssize must be the same size as usize!");

#if defined(__CHAR_BIT__)
	constexpr const usize byte_size = __CHAR_BIT__;
#elif defined(_MSC_VER)
	constexpr const usize byte_size = 8;
#else
# error Missing implementation!
#endif

	using char8 = sint8;
	using char16 = sint16;
	using char32 = sint32;
	using char64 = sint64;

	struct char24
	{
		char8 values_[3];

		char24() = default;

		/*ful_inline*/ explicit char24(char value) { values_[0] = value; *reinterpret_cast<uint16 *>(values_ + 1) = 0; }
		/*ful_inline*/ explicit char24(signed char value) { values_[0] = value; *reinterpret_cast<uint16 *>(values_ + 1) = 0; }
		/*ful_inline*/ explicit char24(unsigned char value) { values_[0] = value; *reinterpret_cast<uint16 *>(values_ + 1) = 0; }
		/*ful_inline*/ explicit char24(char16_t value) { *reinterpret_cast<uint16 *>(values_) = static_cast<uint16>(value); values_[2] = 0; }
		/*ful_inline*/ explicit char24(short value) { *reinterpret_cast<uint16 *>(values_) = static_cast<uint16>(value); values_[2] = 0; }
		/*ful_inline*/ explicit char24(unsigned short value) { *reinterpret_cast<uint16 *>(values_) = static_cast<uint16>(value); values_[2] = 0; }
		/*ful_inline*/ explicit char24(char32_t value) { *reinterpret_cast<uint16 *>(values_) = static_cast<uint16>(value); values_[2] = static_cast<char8>(static_cast<uint32>(value) >> 16); }
		/*ful_inline*/ explicit char24(int value) { *reinterpret_cast<uint16 *>(values_) = static_cast<uint16>(value); values_[2] = static_cast<char8>(static_cast<uint32>(value) >> 16); }
		/*ful_inline*/ explicit char24(unsigned int value) { *reinterpret_cast<uint16 *>(values_) = static_cast<uint16>(value); values_[2] = static_cast<char8>(static_cast<uint32>(value) >> 16); }
		/*ful_inline*/ explicit char24(long value) { *reinterpret_cast<uint16 *>(values_) = static_cast<uint16>(value); values_[2] = static_cast<char8>(static_cast<uint32>(value) >> 16); }
		/*ful_inline*/ explicit char24(unsigned long value) { *reinterpret_cast<uint16 *>(values_) = static_cast<uint16>(value); values_[2] = static_cast<char8>(static_cast<uint32>(value) >> 16); }
		/*ful_inline*/ explicit char24(long long value) { *reinterpret_cast<uint16 *>(values_) = static_cast<uint16>(value); values_[2] = static_cast<char8>(static_cast<uint32>(value) >> 16); }
		/*ful_inline*/ explicit char24(unsigned long long value) { *reinterpret_cast<uint16 *>(values_) = static_cast<uint16>(value); values_[2] = static_cast<char8>(static_cast<uint32>(value) >> 16); }

		/*ful_inline*/ explicit operator char () const { return static_cast<char>(values_[0]); }
		/*ful_inline*/ explicit operator signed char () const { return static_cast<signed char>(values_[0]); }
		/*ful_inline*/ explicit operator unsigned char () const { return static_cast<unsigned char>(values_[0]); }
		/*ful_inline*/ explicit operator char16_t () const { return *reinterpret_cast<const char16_t *>(values_); }
		/*ful_inline*/ explicit operator short () const { return *reinterpret_cast<const short *>(values_); }
		/*ful_inline*/ explicit operator unsigned short () const { return *reinterpret_cast<const unsigned short *>(values_); }
		/*ful_inline*/ explicit operator char32_t () const { return static_cast<char32_t>((static_cast<uint32>(values_[2]) << 16) | (*reinterpret_cast<const uint16 *>(values_))); }
		/*ful_inline*/ explicit operator int () const { return static_cast<int>((static_cast<uint32>(values_[2]) << 16) | (*reinterpret_cast<const uint16 *>(values_))); }
		/*ful_inline*/ explicit operator unsigned int () const { return static_cast<unsigned int>((static_cast<uint32>(values_[2]) << 16) | (*reinterpret_cast<const uint16 *>(values_))); }
		/*ful_inline*/ explicit operator long () const { return static_cast<long>((static_cast<uint32>(values_[2]) << 16) | (*reinterpret_cast<const uint16 *>(values_))); }
		/*ful_inline*/ explicit operator unsigned long () const { return static_cast<unsigned long>((static_cast<uint32>(values_[2]) << 16) | (*reinterpret_cast<const uint16 *>(values_))); }
		/*ful_inline*/ explicit operator long long () const { return static_cast<long long>((static_cast<uint32>(values_[2]) << 16) | (*reinterpret_cast<const uint16 *>(values_))); }
		/*ful_inline*/ explicit operator unsigned long long () const { return static_cast<unsigned long long>((static_cast<uint32>(values_[2]) << 16) | (*reinterpret_cast<const uint16 *>(values_))); }
	};

	static_assert(sizeof(char24) == 3 * sizeof(char8), "");
}

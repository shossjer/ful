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

	struct char24;

	// note this type is needed due to bad compiler optimizations when
	// passing char24 by value to functions
	struct char_fast24
	{
		using compact_type = char24;

		uint32 value_;

		/*ful_inline*/ explicit operator uint32 () const { return value_; }

	private:

		/*ful_inline*/ friend bool operator == (char_fast24 x, char_fast24 y) { return x.value_ == y.value_; }
	};

	struct char24
	{
		uint8 bytes_[3];

		char24() = default;

		/*ful_inline*/ explicit char24(char_fast24 value) { *reinterpret_cast<uint16 *>(bytes_) = static_cast<uint16>(static_cast<uint32>(value)); bytes_[2] = static_cast<uint8>(static_cast<uint32>(value) >> 16); }

		/*ful_inline*/ explicit operator char_fast24 () const { return load32(this); }

	private:

		// note although entirely possible that this 4-byte read may cross a
		// page boundary (and causing a page fault) it is deemed very unlikely
		// since all string literals and string classes are null terminated
		// and so the only way for this to be a problem is when used together
		// with non-null terminated char buffers that end exactly at a page
		// boundary, therfore as long as YOU do not, this is fine :smile:
		/*ful_inline*/ static char_fast24 load32(const char24 * ptr) { return {*reinterpret_cast<const uint32 *>(ptr) & 0x00ffffff}; }

	};

	static_assert(sizeof(char24) == 3 * sizeof(char8), "");
}

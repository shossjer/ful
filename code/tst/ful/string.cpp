#include "ful/string.hpp"

#include <catch2/catch.hpp>

#include <vector>

TEST_CASE("", "")
{
	SECTION("")
	{
		CHECK(ful::copy(nullptr, nullptr, nullptr) == nullptr);
		CHECK(ful::rcopy(nullptr, nullptr, nullptr) == nullptr);
	}

	SECTION("")
	{
		std::vector<ful::unit_utf8> buffer(1 + 2, ful::unit_utf8(-1));
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			buffer[i + 2] = static_cast<ful::unit_utf8>(i % 128);
		}

		CHECK(ful::copy(buffer.data() + 2, buffer.data() + buffer.size(), buffer.data() + 1) == buffer.data() + buffer.size() - 1);
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			CHECK(buffer[i + 1] == static_cast<ful::unit_utf8>(i % 128));
		}

		CHECK(ful::rcopy(buffer.data() + 1, buffer.data() + buffer.size() - 1, buffer.data() + buffer.size()) == buffer.data() + 2);
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			CHECK(buffer[i + 2] == static_cast<ful::unit_utf8>(i % 128));
		}
	}

	SECTION("")
	{
		std::vector<ful::unit_utf8> buffer(3 + 2, ful::unit_utf8(-1));
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			buffer[i + 2] = static_cast<ful::unit_utf8>(i % 128);
		}

		CHECK(ful::copy(buffer.data() + 2, buffer.data() + buffer.size(), buffer.data() + 1) == buffer.data() + buffer.size() - 1);
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			CHECK(buffer[i + 1] == static_cast<ful::unit_utf8>(i % 128));
		}

		CHECK(ful::rcopy(buffer.data() + 1, buffer.data() + buffer.size() - 1, buffer.data() + buffer.size()) == buffer.data() + 2);
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			CHECK(buffer[i + 2] == static_cast<ful::unit_utf8>(i % 128));
		}
	}

	SECTION("")
	{
		std::vector<ful::unit_utf8> buffer(5 + 2, ful::unit_utf8(-1));
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			buffer[i + 2] = static_cast<ful::unit_utf8>(i % 128);
		}

		CHECK(ful::copy(buffer.data() + 2, buffer.data() + buffer.size(), buffer.data() + 1) == buffer.data() + buffer.size() - 1);
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			CHECK(buffer[i + 1] == static_cast<ful::unit_utf8>(i % 128));
		}

		CHECK(ful::rcopy(buffer.data() + 1, buffer.data() + buffer.size() - 1, buffer.data() + buffer.size()) == buffer.data() + 2);
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			CHECK(buffer[i + 2] == static_cast<ful::unit_utf8>(i % 128));
		}
	}

	SECTION("")
	{
		std::vector<ful::unit_utf8> buffer(7 + 2, ful::unit_utf8(-1));
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			buffer[i + 2] = static_cast<ful::unit_utf8>(i % 128);
		}

		CHECK(ful::copy(buffer.data() + 2, buffer.data() + buffer.size(), buffer.data() + 1) == buffer.data() + buffer.size() - 1);
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			CHECK(buffer[i + 1] == static_cast<ful::unit_utf8>(i % 128));
		}

		CHECK(ful::rcopy(buffer.data() + 1, buffer.data() + buffer.size() - 1, buffer.data() + buffer.size()) == buffer.data() + 2);
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			CHECK(buffer[i + 2] == static_cast<ful::unit_utf8>(i % 128));
		}
	}

	SECTION("")
	{
		std::vector<ful::unit_utf8> buffer(9 + 2, ful::unit_utf8(-1));
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			buffer[i + 2] = static_cast<ful::unit_utf8>(i % 128);
		}

		CHECK(ful::copy(buffer.data() + 2, buffer.data() + buffer.size(), buffer.data() + 1) == buffer.data() + buffer.size() - 1);
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			CHECK(buffer[i + 1] == static_cast<ful::unit_utf8>(i % 128));
		}

		CHECK(ful::rcopy(buffer.data() + 1, buffer.data() + buffer.size() - 1, buffer.data() + buffer.size()) == buffer.data() + 2);
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			CHECK(buffer[i + 2] == static_cast<ful::unit_utf8>(i % 128));
		}
	}

	SECTION("")
	{
		std::vector<ful::unit_utf8> buffer(15 + 2, ful::unit_utf8(-1));
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			buffer[i + 2] = static_cast<ful::unit_utf8>(i % 128);
		}

		CHECK(ful::copy(buffer.data() + 2, buffer.data() + buffer.size(), buffer.data() + 1) == buffer.data() + buffer.size() - 1);
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			CHECK(buffer[i + 1] == static_cast<ful::unit_utf8>(i % 128));
		}

		CHECK(ful::rcopy(buffer.data() + 1, buffer.data() + buffer.size() - 1, buffer.data() + buffer.size()) == buffer.data() + 2);
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			CHECK(buffer[i + 2] == static_cast<ful::unit_utf8>(i % 128));
		}
	}

	SECTION("")
	{
		std::vector<ful::unit_utf8> buffer(17 + 2, ful::unit_utf8(-1));
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			buffer[i + 2] = static_cast<ful::unit_utf8>(i % 128);
		}

		CHECK(ful::copy(buffer.data() + 2, buffer.data() + buffer.size(), buffer.data() + 1) == buffer.data() + buffer.size() - 1);
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			CHECK(buffer[i + 1] == static_cast<ful::unit_utf8>(i % 128));
		}

		CHECK(ful::rcopy(buffer.data() + 1, buffer.data() + buffer.size() - 1, buffer.data() + buffer.size()) == buffer.data() + 2);
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			CHECK(buffer[i + 2] == static_cast<ful::unit_utf8>(i % 128));
		}
	}

	SECTION("")
	{
		std::vector<ful::unit_utf8> buffer(31 + 2, ful::unit_utf8(-1));
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			buffer[i + 2] = static_cast<ful::unit_utf8>(i % 128);
		}

		CHECK(ful::copy(buffer.data() + 2, buffer.data() + buffer.size(), buffer.data() + 1) == buffer.data() + buffer.size() - 1);
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			CHECK(buffer[i + 1] == static_cast<ful::unit_utf8>(i % 128));
		}

		CHECK(ful::rcopy(buffer.data() + 1, buffer.data() + buffer.size() - 1, buffer.data() + buffer.size()) == buffer.data() + 2);
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			CHECK(buffer[i + 2] == static_cast<ful::unit_utf8>(i % 128));
		}
	}

	SECTION("")
	{
		std::vector<ful::unit_utf8> buffer(33 + 2, ful::unit_utf8(-1));
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			buffer[i + 2] = static_cast<ful::unit_utf8>(i % 128);
		}

		CHECK(ful::copy(buffer.data() + 2, buffer.data() + buffer.size(), buffer.data() + 1) == buffer.data() + buffer.size() - 1);
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			CHECK(buffer[i + 1] == static_cast<ful::unit_utf8>(i % 128));
		}

		CHECK(ful::rcopy(buffer.data() + 1, buffer.data() + buffer.size() - 1, buffer.data() + buffer.size()) == buffer.data() + 2);
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			CHECK(buffer[i + 2] == static_cast<ful::unit_utf8>(i % 128));
		}
	}

	SECTION("")
	{
		std::vector<ful::unit_utf8> buffer(47 + 2, ful::unit_utf8(-1));
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			buffer[i + 2] = static_cast<ful::unit_utf8>(i % 128);
		}

		CHECK(ful::copy(buffer.data() + 2, buffer.data() + buffer.size(), buffer.data() + 1) == buffer.data() + buffer.size() - 1);
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			CHECK(buffer[i + 1] == static_cast<ful::unit_utf8>(i % 128));
		}

		CHECK(ful::rcopy(buffer.data() + 1, buffer.data() + buffer.size() - 1, buffer.data() + buffer.size()) == buffer.data() + 2);
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			CHECK(buffer[i + 2] == static_cast<ful::unit_utf8>(i % 128));
		}
	}

	SECTION("")
	{
		std::vector<ful::unit_utf8> buffer(257 + 2, ful::unit_utf8(-1));
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			buffer[i + 2] = static_cast<ful::unit_utf8>(i % 128);
		}

		CHECK(ful::copy(buffer.data() + 2, buffer.data() + buffer.size(), buffer.data() + 1) == buffer.data() + buffer.size() - 1);
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			CHECK(buffer[i + 1] == static_cast<ful::unit_utf8>(i % 128));
		}

		CHECK(ful::rcopy(buffer.data() + 1, buffer.data() + buffer.size() - 1, buffer.data() + buffer.size()) == buffer.data() + 2);
		for (int i = 0; i < buffer.size() - 2; i++)
		{
			CHECK(buffer[i + 2] == static_cast<ful::unit_utf8>(i % 128));
		}
	}
}

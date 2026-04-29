#include <gtest/gtest.h>
#include <string>

import serde.targets.string.string_utils.from_structure.copy_quoted;

TEST(StringUtils_CopyQuoted, Basic)
{
    std::string s = R"(abc "hello" def)";
    auto it = s.begin();

    auto result = str_utils::copy_quoted(it, s.end(), '"');

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "\"hello\"");
}

TEST(StringUtils_CopyQuoted, MissingQuote)
{
    std::string s = "abc def";
    auto it = s.begin();

    auto result = str_utils::copy_quoted(it, s.end(), '"');

    EXPECT_FALSE(result.has_value());
}

TEST(StringUtils_CopyQuoted, UnclosedQuoteThrows)
{
    std::string s = R"(abc "hello)";
    auto it = s.begin();

    EXPECT_THROW(
        str_utils::copy_quoted(it, s.end(), '"'),
        std::runtime_error
    );
}
#include <gtest/gtest.h>
#include <string>

import serde.targets.string.string_utils.from_structure.extract_token;

TEST(StringUtils_ExtractToken, SimpleToken)
{
    std::string s = "[42]";
    auto it = s.begin();

    auto token = str_utils::extract_token(it, s.end(), '[', ']', '"');

    ASSERT_TRUE(token.has_value());
    EXPECT_EQ(*token, "42");
}

TEST(StringUtils_ExtractToken, TokenWithQuotes)
{
    std::string s = R"(["Alice"])";
    auto it = s.begin();

    auto token = str_utils::extract_token(it, s.end(), '[', ']', '"');

    ASSERT_TRUE(token.has_value());
    EXPECT_EQ(*token, "\"Alice\"");
}

TEST(StringUtils_ExtractToken, NestedToken)
{
    std::string s = "[[42]]";
    auto it = s.begin();

    auto token = str_utils::extract_token(it, s.end(), '[', ']', '"');

    ASSERT_TRUE(token.has_value());
    EXPECT_EQ(*token, "[42]");
}

TEST(StringUtils_ExtractToken, UnclosedTokenThrows)
{
    std::string s = "[42";
    auto it = s.begin();

    EXPECT_THROW(
        str_utils::extract_token(it, s.end(), '[', ']', '"'),
        std::runtime_error
    );
}
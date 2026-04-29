#include <gtest/gtest.h>
#include <string>
#include <vector>

import serde.targets.string.string_utils.from_structure.split_by_tokens_sequence;

TEST(StringUtils_SplitSequence, Basic)
{
    std::string s = "[42][3.14][\"Alice\"]";

    auto it = s.begin();

    auto result = str_utils::split_by_tokens_sequence(
        it,
        s.end(),
        '[',
        ']',
        '"'
    );

    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "42");
    EXPECT_EQ(result[1], "3.14");
    EXPECT_EQ(result[2], "\"Alice\"");
}

TEST(StringUtils_SplitSequence, Empty)
{
    std::string s = "";

    auto it = s.begin();

    auto result = str_utils::split_by_tokens_sequence(
        it,
        s.end(),
        '[',
        ']',
        '"'
    );

    EXPECT_TRUE(result.empty());
}

TEST(StringUtils_SplitSequence, Nested)
{
    std::string s = "[[1]][[2]]";

    auto it = s.begin();

    auto result = str_utils::split_by_tokens_sequence(
        it,
        s.end(),
        '[',
        ']',
        '"'
    );

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "[1]");
    EXPECT_EQ(result[1], "[2]");
}
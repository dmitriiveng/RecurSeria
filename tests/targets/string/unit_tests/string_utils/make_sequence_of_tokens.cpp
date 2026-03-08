#include <gtest/gtest.h>
#include <string>
#include <vector>

import serde.targets.string.string_utils.to_structure.make_sequence_of_tokens;

TEST(StringUtils_MakeSequence, Basic)
{
    std::vector<std::string> tokens = {
        "42",
        "3.14",
        "\"Alice\""
    };

    auto result = str_utils::make_sequence_of_tokens(tokens, '[', ']');

    EXPECT_EQ(result, "[42][3.14][\"Alice\"]");
}

TEST(StringUtils_MakeSequence, Empty)
{
    std::vector<std::string> tokens;

    auto result = str_utils::make_sequence_of_tokens(tokens, '[', ']');

    EXPECT_TRUE(result.empty());
}
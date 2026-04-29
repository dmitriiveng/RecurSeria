#include <gtest/gtest.h>
#include <string>

import serde.targets.string.string_utils.to_structure.make_token;

TEST(StringUtils_MakeToken, Basic)
{
    std::string token = "42";

    auto result = str_utils::make_token(token, '[', ']');

    EXPECT_EQ(result, "[42]");
}
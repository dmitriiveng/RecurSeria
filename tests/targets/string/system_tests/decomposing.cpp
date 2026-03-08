#include <gtest/gtest.h>
#include <string>
#include <vector>

import serde.core;
import serde.targets.string;

TEST(StringSequence, Decompose)
{
    std::string input = "[42][3.14][\"Alice\"]";

    std::vector<std::string> out;

    serde::functions::decompose_sequentially(out, input);

    ASSERT_EQ(out.size(), 3);
    EXPECT_EQ(out[0], "42");
    EXPECT_EQ(out[1], "3.14");
    EXPECT_EQ(out[2], "\"Alice\"");
}
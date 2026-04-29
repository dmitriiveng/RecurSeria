#include <gtest/gtest.h>
#include <string>
#include <vector>

import serde.core.meta.functions.data_structures.sequence.grouping;
import serde.targets.string.data_structures.sequence.grouping;

TEST(StringUtils_GroupSequentially, Basic)
{
    std::vector<std::string> input = {"42", "3.14", "test"};
    std::string out;

    serde::functions::group_sequentially(out, input);

    EXPECT_EQ(out, "[42][3.14][test]");
}

TEST(StringUtils_GroupSequentially, Empty)
{
    std::vector<std::string> input;
    std::string out;

    serde::functions::group_sequentially(out, input);

    EXPECT_TRUE(out.empty());
}

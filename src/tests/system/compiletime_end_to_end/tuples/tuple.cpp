#include <gtest/gtest.h>
#include <tuple>
#include <string>

import recurseria.core;
import recurseria.target.string.intermediate_representation;

TEST(CompileTimeEndToEnd, Tuple)
{
    using MyTuple = std::tuple<int, double, std::string>;

    MyTuple original{42, 3.14159, "Tuple"};

    bool equals = recurseria::core::meta::srlz_dsrlz_round_trip_validation<
        recurseria::target::string::string_intermediate_representation_format_tag,
        MyTuple,
        recurseria::target::string::StringIRTreeNode
    >(original);

    EXPECT_TRUE(equals);
}

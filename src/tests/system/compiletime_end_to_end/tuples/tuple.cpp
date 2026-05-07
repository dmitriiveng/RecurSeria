#include <gtest/gtest.h>
#include <tuple>
#include <string>

import recurseria.core;
import recurseria.targets.string;

TEST(CompileTimeEndToEnd, Tuple)
{
    using MyTuple = std::tuple<int, double, std::string>;

    MyTuple original{42, 3.14159, "Tuple"};

    bool equals = recurseria::core::meta::srlz_dsrlz_round_trip_validation<
        recurseria::target::string::default_string_format_tag,
        MyTuple,
        std::string
    >(original);

    EXPECT_TRUE(equals);
}

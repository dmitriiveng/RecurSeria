#include <gtest/gtest.h>

#include "serde/aggregate/PfrFieldsGetter.hpp"
#include "serde/serialization/template_function/default/srlz/str_serializers.hpp"
#include "../../../../../libraries/serde/meta/function_generator/serialization/gen_aggregate_srlz_func.hpp"

namespace {

    struct TestAggregate final {
        int a;
        double b;
        std::string c;
        std::vector<std::map<int, double>> v;
    };


    TEST(GenerateAggregateSrlzFunctionTest, AggregateSerialization) {
        const auto func = serde::srlz::gen_aggregate_srlz_func<TestAggregate, std::string, PfrFieldsGetter>();

        TestAggregate input;
        input.a = 1;
        input.b = 1.4;
        input.c = "abc";
        input.v = {{{1, 1.3}, {2, 2.3}}, {{1, 1.123}, {2, 2.4}}};

        const std::string output = func(&input);

        const std::string expected = "[1]"
                                     "[1.4]"
                                     "[\"abc\"]"
                                     "[[[[1][1.3]][[2][2.3]]][[[1][1.123]][[2][2.4]]]]";

        EXPECT_EQ(output, expected);
    }

}

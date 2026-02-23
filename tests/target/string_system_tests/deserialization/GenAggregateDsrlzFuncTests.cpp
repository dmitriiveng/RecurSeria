#include <gtest/gtest.h>

#include "serde/deserialization/template_function/default/dsrlz/str_deserializers.hpp"
#include "../../../../../libraries/serde/meta/function_generator/deserialization/gen_aggregate_dsrlz_func.hpp"
#include "serde/aggregate/PfrFieldsGetter.hpp"


using namespace serde::dsrlz;

namespace {
    struct TestAggregate final {
        int a;
        double b;
        std::string c;
        std::vector<std::map<int, double>> v;
    };

    TEST(GenerateSimpleTypeDsrlzFunctionTest, IntSpecialization) {
        const auto func = gen_aggregate_dsrlz_func<std::string, TestAggregate, PfrFieldsGetter>();

        const std::string ta_str = "[1]"
                                   "[1.4]"
                                   "[\"abc\"]"
                                   "["
                                        "["
                                            "[[1][1.3]]"
                                            "[[2][2.3]]"
                                        "]"

                                        "["
                                            "[[1][1.123]]"
                                            "[[2][2.4]]"
                                        "]"
                                   "]";
        VoidPtrAny result = func(ta_str);
        EXPECT_EQ(result.get_value<TestAggregate>().a, 1);
        EXPECT_EQ(result.get_value<TestAggregate>().b, 1.4);
        EXPECT_EQ(result.get_value<TestAggregate>().c, "abc");

        const std::map m1({
            std::pair(1, 1.3),
            std::pair(2, 2.3),
        });
        const std::map m2({
            std::pair(1, 1.123),
            std::pair(2, 2.4),
        });
        const std::vector res({m1, m2});
        EXPECT_EQ(result.get_value<TestAggregate>().v, res);
    }
}

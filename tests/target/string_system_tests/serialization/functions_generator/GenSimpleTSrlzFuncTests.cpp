#include <gtest/gtest.h>

#include "serde/serialization/template_function/default/srlz/str_serializers.hpp"
#include "../../../../../libraries/serde/meta/function_generator/serialization/gen_simple_type_srlz_func.hpp"

using namespace serde::srlz;

namespace {

    struct TestAggregate final {
        int a;
        double b;
        std::string c;
        std::vector<std::map<int, double>> v;
    };

    TEST(GenerateSimpleTypeSrlzFunctionTest, IntSpecialization) {
        const auto func = gen_simple_type_srlz_func<int, std::string>();
        int input = 42;
        std::string output = func(&input);
        EXPECT_EQ(output, "42");
    }

    TEST(GenerateSimpleTypeSrlzFunctionTest, FloatSpecialization) {
        const auto func = gen_simple_type_srlz_func<float, std::string>();
        float input = 3.5f;
        std::string output = func(&input);
        EXPECT_EQ(output, "3.5");
    }

    TEST(GenerateSimpleTypeSrlzFunctionTest, DoubleSpecialization) {
        const auto func = gen_simple_type_srlz_func<double, std::string>();
        double input = 2.25;
        std::string output = func(&input);
        EXPECT_EQ(output, "2.25");
    }

    TEST(GenerateSimpleTypeSrlzFunctionTest, StringSpecialization) {
        const auto func = gen_simple_type_srlz_func<std::string, std::string>();
        std::string input = "abc";
        std::string output = func(&input);
        EXPECT_EQ(output, "\"abc\"");
    }

    TEST(GenerateSimpleTypeSrlzFunctionTest, VectorSpecialization) {
        const auto func = gen_simple_type_srlz_func<std::vector<std::vector<int>>, std::string>();
        std::vector<std::vector<int>> input{{1,2}, {3,4}, {5,6}};
        std::string output = func(&input);
        EXPECT_EQ(output, "[[1][2]][[3][4]][[5][6]]");
    }

    TEST(GenerateSimpleTypeSrlzFunctionTest, MapSpecialization) {
        const auto func = gen_simple_type_srlz_func<std::map<int,std::vector<int>>, std::string>();
        std::map<int,std::vector<int>> input{
            {1, {2,2}},
            {3, {4,4}},
            {5, {6,6}}
        };
        std::string output = func(&input);
        EXPECT_EQ(output, "[[1][[2][2]]][[3][[4][4]]][[5][[6][6]]]");
    }

}
import <string>;
import <tuple>;

import <gtest/gtest.h>;
import serde.core.meta.functions.default_functions.aggregate_types.fields_getters.boost_pfr_fields_getter;

using namespace serde;

struct TestStruct {
    int a;
    double b;
    std::string c;
};

TEST(PfrFieldsGetterTest, FieldCount) {
    EXPECT_EQ(PfrFieldsGetter::field_count<TestStruct>(), 3);
}

TEST(PfrFieldsGetterTest, ForEachField) {
    TestStruct obj{42, 3.14, "hello"};
    int intSum = 0;
    double doubleSum = 0.0;
    std::string concatenated;

    PfrFieldsGetter::for_each_field(obj, [&](auto& field) {
        if constexpr (std::is_same_v<decltype(field), int&>) {
            intSum += field;
        } else if constexpr (std::is_same_v<decltype(field), double&>) {
            doubleSum += field;
        } else if constexpr (std::is_same_v<decltype(field), std::string&>) {
            concatenated += field;
        }
    });

    EXPECT_EQ(intSum, 42);
    EXPECT_DOUBLE_EQ(doubleSum, 3.14);
    EXPECT_EQ(concatenated, "hello");
}
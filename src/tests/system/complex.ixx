#include <gtest/gtest.h>
#include <string>

import recurseria.core;
import recurseria.targets.string;

struct Person {
    int id;
    double score;
    std::string name;
};

using Type = std::vector<Person>;

TEST(SerdeEndToEnd, Complex)
{
    using Key = std::string;

    recurseria::core::runtime::SrlzFuncMapper<std::string, Key> srlz_mapper;
    recurseria::core::runtime::DsrlzFuncMapper<std::string, Key> dsrlz_mapper;

    Key key = "PersonVector";

    srlz_mapper.add_function(
        key,
        recurseria::core::runtime::gen_simple_type_srlz_func<std::string, Type>()
    );

    dsrlz_mapper.add_function(
        key,
        recurseria::core::runtime::gen_simple_dsrlz_func<Type, std::string>()
    );

    Type original{};
    for (int i = 0; i < 10; ++i) {
        original.push_back(Person{i, i + 3.14, "AWAWAWAWAW" + std::to_string(i)});
    }

    // serialize
    std::string serialized =
        recurseria::core::runtime::serialize<std::string>(srlz_mapper, key, original);

    // deserialize
    recurseria::core::runtime::TypeErasedValueOwner result_te =
        recurseria::core::runtime::deserialize(dsrlz_mapper, key, serialized);

    Type result = result_te.cast_and_release<Type>();

    // verify
    for (int i = 0; i < original.size(); i++){
        EXPECT_EQ(result[i].id, original[i].id);
        EXPECT_DOUBLE_EQ(result[i].score, original[i].score);
        EXPECT_EQ(result[i].name, original[i].name);
    }
}

#include <gtest/gtest.h>
#include <string>

import serde.core;
import serde.targets.string;

import serde.targets.string.data_structures.sequence.grouping;
import serde.targets.string.data_structures.sequence.decomposing;

struct Person {
    int id;
    double score;
    std::string name;
};

using Type = std::vector<Person>;

TEST(SerdeEndToEnd, Complex)
{
    using Key = std::string;

    serde::SrlzFuncMapper<std::string, Key> srlz_mapper;
    serde::DsrlzFuncMapper<std::string, Key> dsrlz_mapper;

    Key key = "PersonVector";

    srlz_mapper.add_function(
        key,
        serde::gen_simple_type_srlz_func<std::string, Type>()
    );

    dsrlz_mapper.add_function(
        key,
        serde::gen_simple_dsrlz_func<Type, std::string>()
    );

    Type original{};
    for (int i = 0; i < 10; ++i) {
        original.push_back(Person{i, i + 3.14, "AWAWAWAWAW" + std::to_string(i)});
    }

    // serialize
    std::string serialized =
        serde::serialize<std::string>(srlz_mapper, key, original);

    // deserialize
    serde::TypeErasedValueOwner result_te =
        serde::deserialize(dsrlz_mapper, key, serialized);

    Type result = result_te.cast_and_release<Type>();

    // verify
    for (int i = 0; i < original.size(); i++){
        EXPECT_EQ(result[i].id, original[i].id);
        EXPECT_DOUBLE_EQ(result[i].score, original[i].score);
        EXPECT_EQ(result[i].name, original[i].name);
    }
}

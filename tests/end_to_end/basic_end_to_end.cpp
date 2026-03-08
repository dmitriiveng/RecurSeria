#include <gtest/gtest.h>
#include <string>
#include <any>

import serde.core;
import serde.targets.string;

import serde.targets.string.data_structures.sequence.grouping;
import serde.targets.string.data_structures.sequence.decomposing;

struct Person {
    int id;
    double score;
    std::string name;
};

TEST(SerdeEndToEnd, RuntimeMapperRoundtrip)
{
    using Key = std::string;

    serde::SrlzFuncMapper<std::string, Key> srlz_mapper;
    serde::DsrlzFuncMapper<std::string, Key> dsrlz_mapper;

    Key key = "Person";

    srlz_mapper.add_function(
        key,
        serde::gen_simple_type_srlz_func<std::string, Person>()
    );

    dsrlz_mapper.add_function(
        key,
        serde::gen_simple_dsrlz_func<Person, std::string>()
    );

    Person original{42, 3.14, "AWAWAWAWW"};

    // serialize
    std::string serialized =
        serde::serialize<std::string>(srlz_mapper, key, &original);

    // deserialize
    std::any result_any =
        serde::deserialize(dsrlz_mapper, key, serialized);

    Person result = std::any_cast<Person>(result_any);

    EXPECT_EQ(result.id, original.id);
    EXPECT_DOUBLE_EQ(result.score, original.score);
    EXPECT_EQ(result.name, original.name);
}
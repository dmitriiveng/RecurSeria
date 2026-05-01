#include <gtest/gtest.h>
#include <string>

import recurseria.core;
import recurseria.targets.string;

struct Person {
    int id;
    double score;
    std::string name;
};

TEST(SerdeEndToEnd, Aggregate)
{
    using Key = std::string;

    recurseria::core::runtime::SrlzFuncMapper<std::string, Key> srlz_mapper;
    recurseria::core::runtime::DsrlzFuncMapper<std::string, Key> dsrlz_mapper;

    Key key = "Person";

    srlz_mapper.add_function(
        key,
        recurseria::core::runtime::gen_simple_type_srlz_func<std::string, Person>()
    );

    dsrlz_mapper.add_function(
        key,
        recurseria::core::runtime::gen_simple_dsrlz_func<Person, std::string>()
    );

    Person original{42, 3.14, "AWAWAWAWAW"};

    // serialize
    std::string serialized =
        recurseria::core::runtime::serialize<std::string>(srlz_mapper, key, original);

    // deserialize
    recurseria::core::runtime::TypeErasedValueOwner result_te =
        recurseria::core::runtime::deserialize(dsrlz_mapper, key, serialized);

    Person result = result_te.cast_and_release<Person>();

    // verify
    EXPECT_EQ(result.id, original.id);

    EXPECT_DOUBLE_EQ(result.score, original.score);

    EXPECT_EQ(result.name, original.name);
}

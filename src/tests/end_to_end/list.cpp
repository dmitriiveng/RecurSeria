#include <gtest/gtest.h>
#include <list>
#include <string>

import serde.core;
import serde.targets.string;

import serde.targets.string.data_structures.sequence.grouping;
import serde.targets.string.data_structures.sequence.decomposing;

TEST(SerdeEndToEnd, Map)
{
    using Key = std::string;

    serde::SrlzFuncMapper<std::string, Key> srlz_mapper;
    serde::DsrlzFuncMapper<std::string, Key> dsrlz_mapper;

    Key key = "IntList";

    srlz_mapper.add_function(
        key,
        serde::gen_simple_type_srlz_func<std::string, std::list<int>>()
    );

    dsrlz_mapper.add_function(
        key,
        serde::gen_simple_dsrlz_func<std::list<int>, std::string>()
    );

    std::list<int> original = {1, 2, 3, 4, 5};

    // serialize
    std::string serialized =
        serde::serialize<std::string>(srlz_mapper, key, original);

    // deserialize
    serde::TypeErasedValueOwner result_te =
        serde::deserialize(dsrlz_mapper, key, serialized);

    std::list<int> result = result_te.cast_and_release<std::list<int>>();

    // verify
    EXPECT_EQ(result.size(), original.size());

    auto it_r = result.begin();
    auto it_o = original.begin();

    while (it_r != result.end() && it_o != original.end()) {
        EXPECT_EQ(*it_r, *it_o);
        ++it_r;
        ++it_o;
    }
}

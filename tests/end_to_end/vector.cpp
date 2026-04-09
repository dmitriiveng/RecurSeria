#include <gtest/gtest.h>
#include <vector>
#include <string>

import serde.core;
import serde.targets.string;

import serde.targets.string.data_structures.sequence.grouping;
import serde.targets.string.data_structures.sequence.decomposing;

TEST(SerdeEndToEnd, Vector)
{
    using Key = std::string;

    serde::SrlzFuncMapper<std::string, Key> srlz_mapper;
    serde::DsrlzFuncMapper<std::string, Key> dsrlz_mapper;

    Key key = "IntVector";

    srlz_mapper.add_function(
        key,
        serde::gen_simple_type_srlz_func<std::string, std::vector<int>>()
    );

    dsrlz_mapper.add_function(
        key,
        serde::gen_simple_dsrlz_func<std::vector<int>, std::string>()
    );

    std::vector<int> original = {1, 2, 3, 4, 5};

    // serialize
    std::string serialized =
        serde::serialize<std::string>(srlz_mapper, key, original);

    // deserialize
    serde::TypeErasedValueOwner result_te =
        serde::deserialize(dsrlz_mapper, key, serialized);

    std::vector<int> result = result_te.cast_and_release<std::vector<int>>();

    // verify
    EXPECT_EQ(result.size(), original.size());

    for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_EQ(result[i], original[i]);
    }
}

#include <gtest/gtest.h>
#include <vector>
#include <string>

import recurseria.core;
import recurseria.targets.string;

TEST(SerdeEndToEnd, Vector)
{
    using Key = std::string;

    recurseria::core::runtime::SrlzFuncMapper<std::string, Key> srlz_mapper;
    recurseria::core::runtime::DsrlzFuncMapper<std::string, Key> dsrlz_mapper;

    Key key = "IntVector";

    srlz_mapper.add_function(
        key,
        recurseria::core::runtime::gen_simple_type_srlz_func<std::string, std::vector<int>>()
    );

    dsrlz_mapper.add_function(
        key,
        recurseria::core::runtime::gen_simple_dsrlz_func<std::vector<int>, std::string>()
    );

    std::vector<int> original{};
    for (int i = 0; i < 1000; ++i) {
        original.push_back(i);
    }

    // serialize
    std::string serialized =
        recurseria::core::runtime::serialize<std::string>(srlz_mapper, key, original);

    // deserialize
    recurseria::core::runtime::TypeErasedValueOwner result_te =
        recurseria::core::runtime::deserialize(dsrlz_mapper, key, serialized);

    std::vector<int> result = result_te.cast_and_release<std::vector<int>>();

    // verify
    EXPECT_EQ(result.size(), original.size());

    for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_EQ(result[i], original[i]);
    }
}

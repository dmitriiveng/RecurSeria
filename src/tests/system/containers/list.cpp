#include <gtest/gtest.h>
#include <list>
#include <string>

import recurseria.core;
import recurseria.targets.string;

TEST(SerdeEndToEnd, Map)
{
    using Key = std::string;

    recurseria::core::runtime::SrlzFuncMapper<std::string, Key> srlz_mapper;
    recurseria::core::runtime::DsrlzFuncMapper<std::string, Key> dsrlz_mapper;

    Key key = "IntList";

    srlz_mapper.add_function(
        key,
        recurseria::core::runtime::gen_simple_type_srlz_func<std::string, std::list<int>>()
    );

    dsrlz_mapper.add_function(
        key,
        recurseria::core::runtime::gen_simple_dsrlz_func<std::list<int>, std::string>()
    );

    std::list<int> original = {1, 2, 3, 4, 5};

    // serialize
    std::string serialized =
        recurseria::core::runtime::serialize<std::string>(srlz_mapper, key, original);

    // deserialize
    recurseria::core::runtime::TypeErasedValueOwner result_te =
        recurseria::core::runtime::deserialize(dsrlz_mapper, key, serialized);

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

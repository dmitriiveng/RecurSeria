#include <gtest/gtest.h>
#include <list>

import recurseria.core;
import recurseria.target.string.intermediate_representation;

TEST(CompileTimeEndToEnd, List)
{
    std::list<int> original = {1, 2, 3, 4, 5};

    std::list<int> result = recurseria::core::meta::srlz_dsrlz_round_trip<
        recurseria::target::string::string_intermediate_representation_format_tag,
        std::list<int>,
        recurseria::target::string::StringIRTreeNode
    >(original);

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

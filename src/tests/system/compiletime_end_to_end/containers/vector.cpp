#include <gtest/gtest.h>
#include <vector>

import recurseria.core;
import recurseria.target.string.intermediate_representation;

TEST(CompileTimeEndToEnd, Vector)
{
    std::vector<int> original{};
    for (int i = 0; i < 100; ++i) {
        original.push_back(i);
    }

    std::vector<int> result = recurseria::core::meta::srlz_dsrlz_round_trip<
        recurseria::target::string::string_intermediate_representation_format_tag,
        std::vector<int>,
        recurseria::target::string::StringIRTreeNode
    >(original);

    // verify
    EXPECT_EQ(result.size(), original.size());

    for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_EQ(result[i], original[i]);
    }
}

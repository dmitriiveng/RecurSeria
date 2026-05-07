#include <gtest/gtest.h>
#include <vector>
#include <string>

import recurseria.core;
import recurseria.targets.string;

TEST(CompileTimeEndToEnd, Vector)
{
    std::vector<int> original{};
    for (int i = 0; i < 100; ++i) {
        original.push_back(i);
    }

    std::vector<int> result = recurseria::core::meta::srlz_dsrlz_round_trip<
        recurseria::target::string::default_string_format_tag,
        std::vector<int>,
        std::string
    >(original);

    // verify
    EXPECT_EQ(result.size(), original.size());

    for (size_t i = 0; i < result.size(); ++i) {
        EXPECT_EQ(result[i], original[i]);
    }
}

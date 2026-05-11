#include <gtest/gtest.h>
#include <string>

import recurseria.core;
import recurseria.target.string.intermediate_representation;

namespace {
    struct Person {
        int id;
        double score;
        std::string name;
    };

    TEST(CompileTimeEndToEnd, Aggregate)
    {
        Person original{42, 3.14, "AWAWAWAWAW"};

        Person result = recurseria::core::meta::srlz_dsrlz_round_trip<
            recurseria::target::string::string_intermediate_representation_format_tag,
            Person,
            recurseria::target::string::StringIRTreeNode
        >(original);

        // verify
        EXPECT_EQ(result.id, original.id);

        EXPECT_DOUBLE_EQ(result.score, original.score);

        EXPECT_EQ(result.name, original.name);
    }
}

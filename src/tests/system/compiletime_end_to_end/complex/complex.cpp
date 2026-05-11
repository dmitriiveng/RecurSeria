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

    using Type = std::vector<Person>;

    TEST(CompileTimeEndToEnd, Complex)
    {
        Type original{};
        for (int i = 0; i < 10; ++i) {
            original.push_back(Person{i, i + 3.14, "AWAWAWAWAW" + std::to_string(i)});
        }

        Type result = recurseria::core::meta::srlz_dsrlz_round_trip<
            recurseria::target::string::string_intermediate_representation_format_tag,
            Type,
            recurseria::target::string::StringIRTreeNode
        >(original);

        // verify
        for (int i = 0; i < original.size(); i++){
            EXPECT_EQ(result[i].id, original[i].id);
            EXPECT_DOUBLE_EQ(result[i].score, original[i].score);
            EXPECT_EQ(result[i].name, original[i].name);
        }
    }
}

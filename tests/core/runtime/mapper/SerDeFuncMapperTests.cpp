#include <gtest/gtest.h>
#include <functional>
#include <string>

import serde.core.runtime.mapper;

namespace {

    using Func = std::function<int(int)>;
    using Key  = std::string;
    using Mapper = serde::SerDeFuncMapper<Func, Key>;

    TEST(SerDeFuncMapperTest, AddAndGetFunction) {
        Mapper mapper;

        mapper.add_function("double", [](int v) {
            return v * 2;
        });

        const auto f = mapper.get_function("double");
        EXPECT_EQ(f(21), 42);
    }

    TEST(SerDeFuncMapperTest, AddDuplicateKeyThrows) {
        Mapper mapper;

        mapper.add_function("inc", [](int v) { return v + 1; });

        EXPECT_THROW(
            mapper.add_function("inc", [](int v) { return v + 2; }),
            std::logic_error
        );
    }

    TEST(SerDeFuncMapperTest, GetMissingKeyThrows) {
        const Mapper mapper;

        EXPECT_THROW(
            mapper.get_function("missing"),
            std::logic_error
        );
    }

    TEST(SerDeFuncMapperTest, ReplaceExistingFunction) {
        Mapper mapper;

        mapper.add_function("op", [](int v) { return v + 1; });

        EXPECT_EQ(mapper.get_function("op")(10), 11);

        mapper.replace_function("op", [](int v) { return v * 3; });

        EXPECT_EQ(mapper.get_function("op")(10), 30);
    }

    TEST(SerDeFuncMapperTest, ReplaceMissingKeyThrows) {
        Mapper mapper;

        EXPECT_THROW(
            mapper.replace_function("missing", [](const int v) { return v; }),
            std::logic_error
        );
    }

}

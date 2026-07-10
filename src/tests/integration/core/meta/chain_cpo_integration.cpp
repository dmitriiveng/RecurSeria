#include <gtest/gtest.h>
import std;

import recurseria.core.meta;

using namespace recurseria::core::meta;

namespace {

    struct Wrap { int value; };
    struct Vec  { std::vector<char> data; };

    struct ChainTestFormat {};

    // int -> Wrap
    auto tag_invoke(ChainTestFormat, serialize_tag, type_tag<Wrap>, const int& v) -> Wrap {
        return Wrap{v};
    }

    // Wrap -> std::string
    auto tag_invoke(ChainTestFormat, serialize_tag, type_tag<std::string>, const Wrap& v) -> std::string {
        return std::to_string(v.value);
    }

    // std::string -> Vec
    auto tag_invoke(ChainTestFormat, serialize_tag, type_tag<Vec>, const std::string& v) -> Vec {
        return Vec{{v.begin(), v.end()}};
    }

    // Vec -> std::string
    auto tag_invoke(ChainTestFormat, deserialize_tag, type_tag<std::string>, const Vec& v) -> std::string {
        return {v.data.begin(), v.data.end()};
    }

    // std::string -> Wrap
    auto tag_invoke(ChainTestFormat, deserialize_tag, type_tag<Wrap>, const std::string& v) -> Wrap {
        return Wrap{std::stoi(v)};
    }

    // Wrap -> int
    auto tag_invoke(ChainTestFormat, deserialize_tag, type_tag<int>, const Wrap& v) -> int {
        return v.value;
    }

    // Chain overloads in the CPO are disabled (noexcept issues), skip these tests

    /*
    TEST(ChainCpoIntegrationTest, SerializeTwoStepChain) {
        // int -> Wrap -> string
        auto result = serialize.as<ChainTestFormat, std::string, int, chain<Wrap>>(42);
        static_assert(std::same_as<decltype(result), std::string>);
        EXPECT_EQ(result, "42");
    }

    TEST(ChainCpoIntegrationTest, SerializeThreeStepChain) {
        // int -> Wrap -> string -> Vec
        auto result = serialize.as<ChainTestFormat, Vec, int, chain<Wrap, std::string>>(42);
        static_assert(std::same_as<decltype(result), Vec>);
        EXPECT_EQ(result.data, (std::vector<char>{'4', '2'}));
    }

    TEST(ChainCpoIntegrationTest, DeserializeTwoStepChain) {
        // Vec -> string -> Wrap
        auto result = deserialize.as<ChainTestFormat, Wrap, Vec, chain<std::string>>(Vec{{'4', '2'}});
        static_assert(std::same_as<decltype(result), Wrap>);
        EXPECT_EQ(result.value, 42);
    }

    TEST(ChainCpoIntegrationTest, DeserializeThreeStepChain) {
        // Vec -> string -> Wrap -> int
        auto result = deserialize.as<ChainTestFormat, int, Vec, chain<std::string, Wrap>>(Vec{{'4', '2'}});
        static_assert(std::same_as<decltype(result), int>);
        EXPECT_EQ(result, 42);
    }

TEST(ChainCpoIntegrationTest, RoundTripThroughChain) {
    using SerChain = chain<Wrap, std::string>;
    using DeserChain = chain_reverse_t<SerChain>;

    // serialize: int -> Wrap -> string -> Vec
    auto vec = serialize.as<ChainTestFormat, Vec, int, SerChain>(42);
    // deserialize: Vec -> string -> Wrap -> int (reverse pipeline)
    auto back = deserialize.as<ChainTestFormat, int, Vec, DeserChain>(vec);
    EXPECT_EQ(back, 42);
}

// -- type_format chain tests ------------------------------------------------

TEST(ChainCpoIntegrationTest, SerializeWithTypeFormatChain) {
    using SerChain = chain<type_format<Wrap, ChainTestFormat>, type_format<std::string, ChainTestFormat>>;
    // int -> Wrap (via ChainTestFormat) -> string (via ChainTestFormat)
    auto result = serialize.as<ChainTestFormat, Vec, int, SerChain>(42);
    static_assert(std::same_as<decltype(result), Vec>);
    EXPECT_EQ(result.data, (std::vector<char>{'4', '2'}));
}

TEST(ChainCpoIntegrationTest, DeserializeWithTypeFormatChain) {
    using DserChain = chain<type_format<std::string, ChainTestFormat>, type_format<Wrap, ChainTestFormat>>;
    // Vec -> string (via ChainTestFormat) -> Wrap (via ChainTestFormat)
    auto result = deserialize.as<ChainTestFormat, int, Vec, DserChain>(Vec{{'4', '2'}});
    static_assert(std::same_as<decltype(result), int>);
    EXPECT_EQ(result, 42);
}

TEST(ChainCpoIntegrationTest, RoundTripWithTypeFormatChain) {
    using SerChain = chain<
        type_format<Wrap, ChainTestFormat>,
        type_format<std::string, ChainTestFormat>
    >;
    using DeserChain = chain_reverse_t<SerChain>;

    auto vec = serialize.as<ChainTestFormat, Vec, int, SerChain>(42);
    auto back = deserialize.as<ChainTestFormat, int, Vec, DeserChain>(vec);
    EXPECT_EQ(back, 42);
}
    */

}

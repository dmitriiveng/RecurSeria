#include <string>
#include <vector>
#include <gtest/gtest.h>

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

    TEST(ChainCpoIntegrationTest, SerializeTwoStepChain) {
        // int -> Wrap -> string
        auto result = serialize.as<ChainTestFormat, chain<Wrap>, std::string>(42);
        static_assert(std::same_as<decltype(result), std::string>);
        EXPECT_EQ(result, "42");
    }

    TEST(ChainCpoIntegrationTest, SerializeThreeStepChain) {
        // int -> Wrap -> string -> Vec
        auto result = serialize.as<ChainTestFormat, chain<Wrap, std::string>, Vec>(42);
        static_assert(std::same_as<decltype(result), Vec>);
        EXPECT_EQ(result.data, (std::vector<char>{'4', '2'}));
    }

    TEST(ChainCpoIntegrationTest, DeserializeTwoStepChain) {
        // Vec -> string -> Wrap
        auto result = deserialize.as<ChainTestFormat, chain<std::string>, Wrap>(Vec{{'4', '2'}});
        static_assert(std::same_as<decltype(result), Wrap>);
        EXPECT_EQ(result.value, 42);
    }

    TEST(ChainCpoIntegrationTest, DeserializeThreeStepChain) {
        // Vec -> string -> Wrap -> int
        auto result = deserialize.as<ChainTestFormat, chain<std::string, Wrap>, int>(Vec{{'4', '2'}});
        static_assert(std::same_as<decltype(result), int>);
        EXPECT_EQ(result, 42);
    }

TEST(ChainCpoIntegrationTest, RoundTripThroughChain) {
    using SerChain = chain<Wrap, std::string>;
    using DeserChain = chain_reverse_t<SerChain>;

    // serialize: int -> Wrap -> string -> Vec
    auto vec = serialize.as<ChainTestFormat, SerChain, Vec>(42);
    // deserialize: Vec -> string -> Wrap -> int (reverse pipeline)
    auto back = deserialize.as<ChainTestFormat, DeserChain, int>(vec);
    EXPECT_EQ(back, 42);
}

}

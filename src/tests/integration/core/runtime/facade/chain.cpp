#include <gtest/gtest.h>
#include <string>
#include <vector>

import recurseria.core;

namespace {

    using recurseria::core::runtime::TypeErasedValuePtr;
    using recurseria::core::runtime::TypeErasedValueOwner;
    using recurseria::core::runtime::SrlzFuncMapper;
    using recurseria::core::runtime::DsrlzFuncMapper;
    using recurseria::core::runtime::serialize;
    using recurseria::core::runtime::deserialize;
    using recurseria::core::runtime::register_serializable;
    using recurseria::core::runtime::register_deserializable;

    using recurseria::core::meta::arg_pack;
    using recurseria::core::meta::chain;
    using recurseria::core::meta::serialize_tag;
    using recurseria::core::meta::deserialize_tag;
    using recurseria::core::meta::type_tag;

    struct Wrap { int value; };
    struct Vec  { std::vector<char> data; };

    struct ChainTestFormat {};

    auto tag_invoke(ChainTestFormat, serialize_tag, type_tag<Wrap>, const int& v) -> Wrap {
        return Wrap{v};
    }

    auto tag_invoke(ChainTestFormat, serialize_tag, type_tag<std::string>, const Wrap& v) -> std::string {
        return std::to_string(v.value);
    }

    auto tag_invoke(ChainTestFormat, serialize_tag, type_tag<Vec>, const std::string& v) -> Vec {
        return Vec{{v.begin(), v.end()}};
    }

    auto tag_invoke(ChainTestFormat, deserialize_tag, type_tag<std::string>, const Vec& v) -> std::string {
        return {v.data.begin(), v.data.end()};
    }

    auto tag_invoke(ChainTestFormat, deserialize_tag, type_tag<Wrap>, const std::string& v) -> Wrap {
        return Wrap{std::stoi(v)};
    }

    auto tag_invoke(ChainTestFormat, deserialize_tag, type_tag<int>, const Wrap& v) -> int {
        return v.value;
    }

    using Key = std::string;
    const Key kKey = "chain_test";

    TEST(FacadeChainTest, RegisterAndSerializeTwoStepChain) {
        // int -> Wrap -> string
        using SrlzArgs = arg_pack<ChainTestFormat, chain<Wrap>, std::string, int>;
        SrlzFuncMapper<std::string, Key> mapper;
        register_serializable<SrlzArgs>(mapper, kKey);

        int value = 42;
        TypeErasedValuePtr wrapped(value);
        auto result = serialize(mapper, kKey, wrapped);
        static_assert(std::same_as<decltype(result), std::string>);
        EXPECT_EQ(result, "42");
    }

    TEST(FacadeChainTest, RegisterAndSerializeThreeStepChain) {
        // int -> Wrap -> string -> Vec
        using SrlzArgs = arg_pack<ChainTestFormat, chain<Wrap, std::string>, Vec, int>;
        SrlzFuncMapper<Vec, Key> mapper;
        register_serializable<SrlzArgs>(mapper, kKey);

        int value = 42;
        TypeErasedValuePtr wrapped(value);
        auto result = serialize(mapper, kKey, wrapped);
        static_assert(std::same_as<decltype(result), Vec>);
        EXPECT_EQ(result.data, (std::vector<char>{'4', '2'}));
    }

    TEST(FacadeChainTest, RegisterAndDeserializeTwoStepChain) {
        // Vec -> string -> Wrap
        using DsrlzArgs = arg_pack<ChainTestFormat, chain<std::string>, Wrap, Vec>;
        DsrlzFuncMapper<Vec, Key> mapper;
        register_deserializable<DsrlzArgs>(mapper, kKey);

        Vec data{{'4', '2'}};
        auto owner = deserialize(mapper, kKey, data);
        Wrap result = owner.cast_and_release<Wrap>();
        EXPECT_EQ(result.value, 42);
    }

    TEST(FacadeChainTest, RegisterAndDeserializeThreeStepChain) {
        // Vec -> string -> Wrap -> int
        using DsrlzArgs = arg_pack<ChainTestFormat, chain<std::string, Wrap>, int, Vec>;
        DsrlzFuncMapper<Vec, Key> mapper;
        register_deserializable<DsrlzArgs>(mapper, kKey);

        Vec data{{'4', '2'}};
        auto owner = deserialize(mapper, kKey, data);
        int result = owner.cast_and_release<int>();
        EXPECT_EQ(result, 42);
    }

    TEST(FacadeChainTest, RoundTripThroughChain) {
        using SerChain = chain<Wrap, std::string>;
        using DeserChain = chain<std::string, Wrap>;

        using SrlzArgs = arg_pack<ChainTestFormat, SerChain, Vec, int>;
        using DsrlzArgs = arg_pack<ChainTestFormat, DeserChain, int, Vec>;

        SrlzFuncMapper<Vec, Key> srlz_mapper;
        DsrlzFuncMapper<Vec, Key> dsrlz_mapper;

        register_serializable<SrlzArgs>(srlz_mapper, kKey);
        register_deserializable<DsrlzArgs>(dsrlz_mapper, kKey);

        int original = 42;
        TypeErasedValuePtr wrapped(original);

        auto serialized = serialize(srlz_mapper, kKey, wrapped);
        auto owner = deserialize(dsrlz_mapper, kKey, serialized);
        int result = owner.cast_and_release<int>();

        EXPECT_EQ(result, original);
    }
}

#include <gtest/gtest.h>
#include <string>
#include <vector>

import recurseria.core;
import recurseria.target.string.intermediate_representation;

namespace {

    using recurseria::core::runtime::TypeErasedValuePtr;
    using recurseria::core::runtime::TypeErasedValueOwner;
    using recurseria::core::runtime::SrlzFuncMapper;
    using recurseria::core::runtime::DsrlzFuncMapper;
    using recurseria::core::runtime::serialize;
    using recurseria::core::runtime::deserialize;
    using recurseria::core::runtime::register_serializable_type;
    using recurseria::core::runtime::register_deserializable_type;

    using recurseria::target::string::StringIRTreeNode;
    using recurseria::target::string::string_intermediate_representation_format_tag;

    struct Point {
        int x;
        int y;
    };

    using SerializedType = StringIRTreeNode;
    using FormatTag = string_intermediate_representation_format_tag;
    using Key = std::string;

    const Key kPointKey = "point";
    const Key kMissingKey = "missing";

    TEST(FacadeIntegration, RegisterAndSerialize) {
        SrlzFuncMapper<SerializedType, Key> mapper;

        register_serializable_type<FormatTag, SerializedType, Point>(mapper, kPointKey);

        Point original{42, 13};
        TypeErasedValuePtr wrapped(original);

        auto result = serialize(mapper, kPointKey, wrapped);

        EXPECT_TRUE(result.is_branch());
        EXPECT_EQ(result.get_sequence()[0].get_string(), "42");
        EXPECT_EQ(result.get_sequence()[1].get_string(), "13");
    }

    TEST(FacadeIntegration, RegisterAndDeserialize) {
        DsrlzFuncMapper<SerializedType, Key> mapper;

        register_deserializable_type<FormatTag, Point, SerializedType>(mapper, kPointKey);

        StringIRTreeNode data(std::vector<StringIRTreeNode>{
            StringIRTreeNode("42"),
            StringIRTreeNode("13")
        });

        auto owner = deserialize(mapper, kPointKey, data);
        Point result = owner.cast_and_release<Point>();

        EXPECT_EQ(result.x, 42);
        EXPECT_EQ(result.y, 13);
    }

    TEST(FacadeIntegration, FullRoundTrip) {
        SrlzFuncMapper<SerializedType, Key> srlz_mapper;
        DsrlzFuncMapper<SerializedType, Key> dsrlz_mapper;

        register_serializable_type<FormatTag, SerializedType, Point>(srlz_mapper, kPointKey);
        register_deserializable_type<FormatTag, Point, SerializedType>(dsrlz_mapper, kPointKey);

        Point original{42, 13};
        TypeErasedValuePtr wrapped(original);

        auto serialized = serialize(srlz_mapper, kPointKey, wrapped);
        auto owner = deserialize(dsrlz_mapper, kPointKey, serialized);
        Point result = owner.cast_and_release<Point>();

        EXPECT_EQ(result.x, original.x);
        EXPECT_EQ(result.y, original.y);
    }

    TEST(FacadeIntegration, SerializeMissingKeyThrows) {
        SrlzFuncMapper<SerializedType, Key> mapper;

        Point p{1, 2};
        TypeErasedValuePtr wrapped(p);

        EXPECT_THROW(serialize(mapper, kMissingKey, wrapped), std::logic_error);
    }

    TEST(FacadeIntegration, DeserializeMissingKeyThrows) {
        DsrlzFuncMapper<SerializedType, Key> mapper;

        SerializedType data("test");

        EXPECT_THROW(deserialize(mapper, kMissingKey, data), std::logic_error);
    }

    TEST(FacadeIntegration, RegisterSerializableDuplicateThrows) {
        SrlzFuncMapper<SerializedType, Key> mapper;

        register_serializable_type<FormatTag, SerializedType, Point>(mapper, kPointKey);

        EXPECT_THROW(
            (register_serializable_type<FormatTag, SerializedType, Point>(mapper, kPointKey)),
            std::logic_error
        );
    }

    TEST(FacadeIntegration, RegisterDeserializableDuplicateThrows) {
        DsrlzFuncMapper<SerializedType, Key> mapper;

        register_deserializable_type<FormatTag, Point, SerializedType>(mapper, kPointKey);

        EXPECT_THROW(
            (register_deserializable_type<FormatTag, Point, SerializedType>(mapper, kPointKey)),
            std::logic_error
        );
    }

    TEST(FacadeIntegration, StringFieldRoundTrip) {
        SrlzFuncMapper<SerializedType, Key> srlz_mapper;
        DsrlzFuncMapper<SerializedType, Key> dsrlz_mapper;

        struct Person {
            int id;
            std::string name;
        };

        const Key kPersonKey = "person";
        register_serializable_type<FormatTag, SerializedType, Person>(srlz_mapper, kPersonKey);
        register_deserializable_type<FormatTag, Person, SerializedType>(dsrlz_mapper, kPersonKey);

        Person original{42, "Alice"};
        TypeErasedValuePtr wrapped(original);

        auto serialized = serialize(srlz_mapper, kPersonKey, wrapped);
        auto owner = deserialize(dsrlz_mapper, kPersonKey, serialized);
        Person result = owner.cast_and_release<Person>();

        EXPECT_EQ(result.id, original.id);
        EXPECT_EQ(result.name, original.name);
    }

}

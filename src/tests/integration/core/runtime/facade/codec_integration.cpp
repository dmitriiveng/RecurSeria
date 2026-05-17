#include <gtest/gtest.h>
#include <string>

import recurseria.core;
import recurseria.target.string.intermediate_representation;

namespace {

    using recurseria::core::runtime::TypeErasedValuePtr;
    using recurseria::core::runtime::SrlzFuncMapper;
    using recurseria::core::runtime::DsrlzFuncMapper;

    using recurseria::target::string::StringIRTreeNode;
    using recurseria::target::string::string_intermediate_representation_format_tag;

    struct Point {
        int x;
        int y;
    };

    using SerializedType = StringIRTreeNode;
    using FormatTag = string_intermediate_representation_format_tag;
    using Key = std::string;

    // --- Codec class tests ---

    TEST(CodecIntegration, RegisterAndRoundTrip) {
        SrlzFuncMapper<SerializedType, Key> srlz_mapper;
        DsrlzFuncMapper<SerializedType, Key> dsrlz_mapper;
        recurseria::core::runtime::Codec<SerializedType, Key> codec(srlz_mapper, dsrlz_mapper);

        codec.register_type<FormatTag, Point>("point");

        Point original{42, 13};
        TypeErasedValuePtr wrapped(original);

        auto serialized = codec.serialize("point", wrapped);
        auto owner = codec.deserialize("point", serialized);
        Point result = owner.cast_and_release<Point>();

        EXPECT_EQ(result.x, original.x);
        EXPECT_EQ(result.y, original.y);
    }

    TEST(CodecIntegration, HasType) {
        SrlzFuncMapper<SerializedType, Key> srlz_mapper;
        DsrlzFuncMapper<SerializedType, Key> dsrlz_mapper;
        recurseria::core::runtime::Codec<SerializedType, Key> codec(srlz_mapper, dsrlz_mapper);

        EXPECT_FALSE(codec.has_serializable("point"));
        EXPECT_FALSE(codec.has_deserializable("point"));

        codec.register_serializable<FormatTag, Point>("point");
        EXPECT_TRUE(codec.has_serializable("point"));
        EXPECT_FALSE(codec.has_deserializable("point"));

        codec.register_deserializable<FormatTag, Point>("point");
        EXPECT_TRUE(codec.has_deserializable("point"));
    }

    TEST(CodecIntegration, SerializeMissingKeyThrows) {
        SrlzFuncMapper<SerializedType, Key> srlz_mapper;
        DsrlzFuncMapper<SerializedType, Key> dsrlz_mapper;
        recurseria::core::runtime::Codec<SerializedType, Key> codec(srlz_mapper, dsrlz_mapper);

        Point p{1, 2};
        TypeErasedValuePtr wrapped(p);

        EXPECT_THROW(codec.serialize("missing", wrapped), std::logic_error);
    }

    TEST(CodecIntegration, DeserializeMissingKeyThrows) {
        SrlzFuncMapper<SerializedType, Key> srlz_mapper;
        DsrlzFuncMapper<SerializedType, Key> dsrlz_mapper;
        recurseria::core::runtime::Codec<SerializedType, Key> codec(srlz_mapper, dsrlz_mapper);

        SerializedType data("test");

        EXPECT_THROW(codec.deserialize("missing", data), std::logic_error);
    }

    TEST(CodecIntegration, RegisterDuplicateThrows) {
        SrlzFuncMapper<SerializedType, Key> srlz_mapper;
        DsrlzFuncMapper<SerializedType, Key> dsrlz_mapper;
        recurseria::core::runtime::Codec<SerializedType, Key> codec(srlz_mapper, dsrlz_mapper);

        codec.register_type<FormatTag, Point>("point");

        EXPECT_THROW((codec.register_type<FormatTag, Point>("point")), std::logic_error);
    }

    TEST(CodecIntegration, ReplaceMissingThrows) {
        SrlzFuncMapper<SerializedType, Key> srlz_mapper;
        DsrlzFuncMapper<SerializedType, Key> dsrlz_mapper;
        recurseria::core::runtime::Codec<SerializedType, Key> codec(srlz_mapper, dsrlz_mapper);

        EXPECT_THROW((codec.replace_serializable<FormatTag, Point>("point")), std::logic_error);
        EXPECT_THROW((codec.replace_deserializable<FormatTag, Point>("point")), std::logic_error);
    }

    TEST(CodecIntegration, RegisterAndRoundTripWithString) {
        SrlzFuncMapper<SerializedType, Key> srlz_mapper;
        DsrlzFuncMapper<SerializedType, Key> dsrlz_mapper;
        recurseria::core::runtime::Codec<SerializedType, Key> codec(srlz_mapper, dsrlz_mapper);

        struct Person {
            int id;
            std::string name;
        };

        codec.register_type<FormatTag, Person>("person");

        Person original{42, "Alice"};
        TypeErasedValuePtr wrapped(original);

        auto serialized = codec.serialize("person", wrapped);
        auto owner = codec.deserialize("person", serialized);
        Person result = owner.cast_and_release<Person>();

        EXPECT_EQ(result.id, original.id);
        EXPECT_EQ(result.name, original.name);
    }

    TEST(CodecIntegration, MultipleTypes) {
        SrlzFuncMapper<SerializedType, Key> srlz_mapper;
        DsrlzFuncMapper<SerializedType, Key> dsrlz_mapper;
        recurseria::core::runtime::Codec<SerializedType, Key> codec(srlz_mapper, dsrlz_mapper);

        codec.register_type<FormatTag, Point>("point");

        struct Circle {
            Point center;
            int radius;
        };

        codec.register_type<FormatTag, Circle>("circle");

        Point p{1, 2};
        TypeErasedValuePtr wrapped_p(p);
        auto sp = codec.serialize("point", wrapped_p);
        auto op = codec.deserialize("point", sp);
        Point rp = op.cast_and_release<Point>();
        EXPECT_EQ(rp.x, 1);
        EXPECT_EQ(rp.y, 2);

        Circle c{{10, 20}, 5};
        TypeErasedValuePtr wrapped_c(c);
        auto sc = codec.serialize("circle", wrapped_c);
        auto oc = codec.deserialize("circle", sc);
        Circle rc = oc.cast_and_release<Circle>();
        EXPECT_EQ(rc.center.x, 10);
        EXPECT_EQ(rc.center.y, 20);
        EXPECT_EQ(rc.radius, 5);
    }

}

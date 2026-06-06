#include <gtest/gtest.h>
import std;

import recurseria.core;
import recurseria.target.string.intermediate_representation;

namespace {

    using recurseria::core::runtime::TypeErasedValuePtr;
    using recurseria::core::runtime::TypeErasedValueOwner;
    using recurseria::core::runtime::SrlzFuncMapper;
    using recurseria::core::runtime::DsrlzFuncMapper;
    using recurseria::core::runtime::serialize;
    using recurseria::core::runtime::deserialize;
    using recurseria::core::runtime::register_serializable;
    using recurseria::core::runtime::register_deserializable;
    using recurseria::core::runtime::replace_serializable;
    using recurseria::core::runtime::replace_deserializable;
    using recurseria::core::runtime::has_serializable;
    using recurseria::core::runtime::has_deserializable;

    using recurseria::core::meta::arg_pack;

    using recurseria::target::string::StringIRTreeNode;
    using recurseria::target::string::string_intermediate_representation_format_tag;

    struct Point {
        int x;
        int y;
    };

    using SerializedType = StringIRTreeNode;
    using FormatTag = string_intermediate_representation_format_tag;
    using SrlzArgs = arg_pack<FormatTag, SerializedType, Point>;
    using DsrlzArgs = arg_pack<FormatTag, Point, SerializedType>;
    using Key = std::string;

    const Key kPointKey = "point";
    const Key kMissingKey = "missing";

    TEST(FacadeTest, RegisterAndSerialize) {
        SrlzFuncMapper<SerializedType, Key> mapper;

        register_serializable<SrlzArgs>(mapper, kPointKey);

        Point original{42, 13};
        TypeErasedValuePtr wrapped(original);

        auto result = serialize(mapper, kPointKey, wrapped);

        EXPECT_TRUE(result.is_branch());
        EXPECT_EQ(result.get_sequence()[0].get_string(), "42");
        EXPECT_EQ(result.get_sequence()[1].get_string(), "13");
    }

    TEST(FacadeTest, RegisterAndDeserialize) {
        DsrlzFuncMapper<SerializedType, Key> mapper;

        register_deserializable<DsrlzArgs>(mapper, kPointKey);

        StringIRTreeNode data(std::vector<StringIRTreeNode>{
            StringIRTreeNode("42"),
            StringIRTreeNode("13")
        });

        auto owner = deserialize(mapper, kPointKey, data);
        Point result = owner.cast_and_release<Point>();

        EXPECT_EQ(result.x, 42);
        EXPECT_EQ(result.y, 13);
    }

    TEST(FacadeTest, RoundTrip) {
        SrlzFuncMapper<SerializedType, Key> srlz_mapper;
        DsrlzFuncMapper<SerializedType, Key> dsrlz_mapper;

        register_serializable<SrlzArgs>(srlz_mapper, kPointKey);
        register_deserializable<DsrlzArgs>(dsrlz_mapper, kPointKey);

        Point original{42, 13};
        TypeErasedValuePtr wrapped(original);

        auto serialized = serialize(srlz_mapper, kPointKey, wrapped);
        auto owner = deserialize(dsrlz_mapper, kPointKey, serialized);
        Point result = owner.cast_and_release<Point>();

        EXPECT_EQ(result.x, original.x);
        EXPECT_EQ(result.y, original.y);
    }

    TEST(FacadeTest, HasType) {
        SrlzFuncMapper<SerializedType, Key> srlz_mapper;
        DsrlzFuncMapper<SerializedType, Key> dsrlz_mapper;

        EXPECT_FALSE(has_serializable(srlz_mapper, kPointKey));
        EXPECT_FALSE(has_deserializable(dsrlz_mapper, kPointKey));

        register_serializable<SrlzArgs>(srlz_mapper, kPointKey);
        EXPECT_TRUE(has_serializable(srlz_mapper, kPointKey));
        EXPECT_FALSE(has_deserializable(dsrlz_mapper, kPointKey));

        register_deserializable<DsrlzArgs>(dsrlz_mapper, kPointKey);
        EXPECT_TRUE(has_deserializable(dsrlz_mapper, kPointKey));
    }

    TEST(FacadeTest, SerializeMissingKeyThrows) {
        SrlzFuncMapper<SerializedType, Key> mapper;

        Point p{1, 2};
        TypeErasedValuePtr wrapped(p);

        EXPECT_THROW(serialize(mapper, kMissingKey, wrapped), std::logic_error);
    }

    TEST(FacadeTest, DeserializeMissingKeyThrows) {
        DsrlzFuncMapper<SerializedType, Key> mapper;

        SerializedType data("test");

        EXPECT_THROW(deserialize(mapper, kMissingKey, data), std::logic_error);
    }

    TEST(FacadeTest, RegisterSerializableDuplicateThrows) {
        SrlzFuncMapper<SerializedType, Key> mapper;

        register_serializable<SrlzArgs>(mapper, kPointKey);

        EXPECT_THROW(
            (register_serializable<SrlzArgs>(mapper, kPointKey)),
            std::logic_error
        );
    }

    TEST(FacadeTest, RegisterDeserializableDuplicateThrows) {
        DsrlzFuncMapper<SerializedType, Key> mapper;

        register_deserializable<DsrlzArgs>(mapper, kPointKey);

        EXPECT_THROW(
            (register_deserializable<DsrlzArgs>(mapper, kPointKey)),
            std::logic_error
        );
    }

    TEST(FacadeTest, RegisterBothDuplicateThrows) {
        SrlzFuncMapper<SerializedType, Key> srlz_mapper;
        DsrlzFuncMapper<SerializedType, Key> dsrlz_mapper;

        register_serializable<SrlzArgs>(srlz_mapper, kPointKey);
        register_deserializable<DsrlzArgs>(dsrlz_mapper, kPointKey);

        EXPECT_THROW((register_serializable<SrlzArgs>(srlz_mapper, kPointKey)), std::logic_error);
        EXPECT_THROW((register_deserializable<DsrlzArgs>(dsrlz_mapper, kPointKey)), std::logic_error);
    }

    TEST(FacadeTest, ReplaceMissingThrows) {
        SrlzFuncMapper<SerializedType, Key> srlz_mapper;
        DsrlzFuncMapper<SerializedType, Key> dsrlz_mapper;

        EXPECT_THROW((replace_serializable<SrlzArgs>(srlz_mapper, kPointKey)), std::logic_error);
        EXPECT_THROW((replace_deserializable<DsrlzArgs>(dsrlz_mapper, kPointKey)), std::logic_error);
    }

    TEST(FacadeTest, StringFieldRoundTrip) {
        SrlzFuncMapper<SerializedType, Key> srlz_mapper;
        DsrlzFuncMapper<SerializedType, Key> dsrlz_mapper;

        struct Person {
            int id;
            std::string name;
        };

        using PSrlzArgs = arg_pack<FormatTag, SerializedType, Person>;
        using PDsrlzArgs = arg_pack<FormatTag, Person, SerializedType>;

        const Key kPersonKey = std::string("person");
        register_serializable<PSrlzArgs>(srlz_mapper, kPersonKey);
        register_deserializable<PDsrlzArgs>(dsrlz_mapper, kPersonKey);

        Person original{42, "Alice"};
        TypeErasedValuePtr wrapped(original);

        auto serialized = serialize(srlz_mapper, kPersonKey, wrapped);
        auto owner = deserialize(dsrlz_mapper, kPersonKey, serialized);
        Person result = owner.cast_and_release<Person>();

        EXPECT_EQ(result.id, original.id);
        EXPECT_EQ(result.name, original.name);
    }

    TEST(FacadeTest, MultipleTypes) {
        SrlzFuncMapper<SerializedType, Key> srlz_mapper;
        DsrlzFuncMapper<SerializedType, Key> dsrlz_mapper;

        register_serializable<SrlzArgs>(srlz_mapper, kPointKey);
        register_deserializable<DsrlzArgs>(dsrlz_mapper, kPointKey);

        struct Circle {
            Point center;
            int radius;
        };

        using CSrlzArgs = arg_pack<FormatTag, SerializedType, Circle>;
        using CDsrlzArgs = arg_pack<FormatTag, Circle, SerializedType>;

        register_serializable<CSrlzArgs>(srlz_mapper, std::string("circle"));
        register_deserializable<CDsrlzArgs>(dsrlz_mapper, std::string("circle"));

        Point p{1, 2};
        TypeErasedValuePtr wrapped_p(p);
        auto sp = serialize(srlz_mapper, kPointKey, wrapped_p);
        auto op = deserialize(dsrlz_mapper, kPointKey, sp);
        Point rp = op.cast_and_release<Point>();
        EXPECT_EQ(rp.x, 1);
        EXPECT_EQ(rp.y, 2);

        Circle c{{10, 20}, 5};
        TypeErasedValuePtr wrapped_c(c);
        auto sc = serialize(srlz_mapper, std::string("circle"), wrapped_c);
        auto oc = deserialize(dsrlz_mapper, std::string("circle"), sc);
        Circle rc = oc.cast_and_release<Circle>();
        EXPECT_EQ(rc.center.x, 10);
        EXPECT_EQ(rc.center.y, 20);
        EXPECT_EQ(rc.radius, 5);
    }

}

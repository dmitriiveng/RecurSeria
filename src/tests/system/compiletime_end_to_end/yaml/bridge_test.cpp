#include <yaml-cpp/yaml.h>
#include <gtest/gtest.h>
#include <string>
#include <vector>

import recurseria.core.meta;
import recurseria.target.string.intermediate_representation;
import recurseria.target.string.yaml_bridge;

namespace {

    using recurseria::core::meta::serialize;
    using recurseria::core::meta::deserialize;
    using recurseria::target::string::StringIRTreeNode;
    using recurseria::target::string::yaml_bridge_format_tag;
    using recurseria::target::string::string_intermediate_representation_format_tag;

    // -- Leaf round-trip ---------------------------------------------------

    TEST(YamlBridgeTest, LeafToYamlAndBack)
    {
        StringIRTreeNode original("hello");

        auto yaml_node = serialize.as<yaml_bridge_format_tag, YAML::Node>(original);

        ASSERT_TRUE(yaml_node.IsScalar());
        EXPECT_EQ(yaml_node.as<std::string>(), "hello");

        auto result = deserialize.as<yaml_bridge_format_tag, StringIRTreeNode>(yaml_node);

        EXPECT_TRUE(result.is_leaf());
        EXPECT_EQ(result.get_string(), "hello");
    }

    // -- Branch round-trip -------------------------------------------------

    TEST(YamlBridgeTest, BranchToYamlAndBack)
    {
        StringIRTreeNode original(std::vector<StringIRTreeNode>{
            StringIRTreeNode("a"),
            StringIRTreeNode("b"),
            StringIRTreeNode("c")
        });

        auto yaml_node = serialize.as<yaml_bridge_format_tag, YAML::Node>(original);

        ASSERT_TRUE(yaml_node.IsSequence());
        ASSERT_EQ(yaml_node.size(), 3);
        EXPECT_EQ(yaml_node[0].as<std::string>(), "a");
        EXPECT_EQ(yaml_node[1].as<std::string>(), "b");
        EXPECT_EQ(yaml_node[2].as<std::string>(), "c");

        auto result = deserialize.as<yaml_bridge_format_tag, StringIRTreeNode>(yaml_node);

        ASSERT_TRUE(result.is_branch());
        const auto& seq = result.get_sequence();
        ASSERT_EQ(seq.size(), 3);
        EXPECT_EQ(seq[0].get_string(), "a");
        EXPECT_EQ(seq[1].get_string(), "b");
        EXPECT_EQ(seq[2].get_string(), "c");
    }

    // -- Nested branch -----------------------------------------------------

    TEST(YamlBridgeTest, NestedToYamlAndBack)
    {
        StringIRTreeNode original(std::vector<StringIRTreeNode>{
            StringIRTreeNode("top"),
            StringIRTreeNode(std::vector<StringIRTreeNode>{
                StringIRTreeNode("nested_a"),
                StringIRTreeNode("nested_b")
            })
        });

        auto yaml_node = serialize.as<yaml_bridge_format_tag, YAML::Node>(original);

        ASSERT_TRUE(yaml_node.IsSequence());
        ASSERT_EQ(yaml_node.size(), 2);
        EXPECT_EQ(yaml_node[0].as<std::string>(), "top");
        ASSERT_TRUE(yaml_node[1].IsSequence());
        EXPECT_EQ(yaml_node[1][0].as<std::string>(), "nested_a");
        EXPECT_EQ(yaml_node[1][1].as<std::string>(), "nested_b");

        auto result = deserialize.as<yaml_bridge_format_tag, StringIRTreeNode>(yaml_node);

        ASSERT_TRUE(result.is_branch());
        const auto& seq = result.get_sequence();
        ASSERT_EQ(seq.size(), 2);
        EXPECT_EQ(seq[0].get_string(), "top");
        ASSERT_TRUE(seq[1].is_branch());
        const auto& nested = seq[1].get_sequence();
        EXPECT_EQ(nested[0].get_string(), "nested_a");
        EXPECT_EQ(nested[1].get_string(), "nested_b");
    }

    // -- YAML map round-trip ----------------------------------------------

    TEST(YamlBridgeTest, YamlMapToBranchAndBack)
    {
        YAML::Node map(YAML::NodeType::Map);
        map["x"] = "10";
        map["y"] = "20";

        auto ir = deserialize.as<yaml_bridge_format_tag, StringIRTreeNode>(map);

        ASSERT_TRUE(ir.is_branch());
        ASSERT_EQ(ir.get_sequence().size(), 2);

        for (const auto& kv : ir.get_sequence()) {
            ASSERT_TRUE(kv.is_branch());
            ASSERT_EQ(kv.get_sequence().size(), 2);
        }

        auto back = serialize.as<yaml_bridge_format_tag, YAML::Node>(ir);

        ASSERT_TRUE(back.IsSequence());
        ASSERT_EQ(back.size(), 2);
        EXPECT_EQ(back[0][0].as<std::string>(), "x");
        EXPECT_EQ(back[0][1].as<std::string>(), "10");
        EXPECT_EQ(back[1][0].as<std::string>(), "y");
        EXPECT_EQ(back[1][1].as<std::string>(), "20");
    }

    // -- YAML null ---------------------------------------------------------

    TEST(YamlBridgeTest, NullYamlNode)
    {
        YAML::Node null_node;

        auto ir = deserialize.as<yaml_bridge_format_tag, StringIRTreeNode>(null_node);

        EXPECT_TRUE(ir.is_leaf());
        EXPECT_TRUE(ir.get_string().empty());
    }

    // -- End-to-end via StringIR -------------------------------------------

    struct Person {
        int id;
        double score;
        std::string name;
    };

    TEST(YamlBridgeTest, EndToEndViaStringIR)
    {
        Person original{42, 3.14, "Alice"};

        auto ir = serialize.as<string_intermediate_representation_format_tag, StringIRTreeNode>(original);

        auto yaml = serialize.as<yaml_bridge_format_tag, YAML::Node>(ir);

        ASSERT_TRUE(yaml.IsSequence());
        EXPECT_EQ(yaml[0].as<std::string>(), "42");
        EXPECT_EQ(yaml[1].as<std::string>(), "3.140000");
        EXPECT_EQ(yaml[2].as<std::string>(), "Alice");

        auto ir_back = deserialize.as<yaml_bridge_format_tag, StringIRTreeNode>(yaml);

        Person result = deserialize.as<string_intermediate_representation_format_tag, Person>(ir_back);

        EXPECT_EQ(result.id, original.id);
        EXPECT_DOUBLE_EQ(result.score, original.score);
        EXPECT_EQ(result.name, original.name);
    }

}

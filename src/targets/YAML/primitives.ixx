export module recurseria.yaml:primitives;

import std;
import yaml_cpp;

export import recurseria;
import :format_tag;

export namespace recurseria::yaml {
    using serialize_tag = recurseria::serialize_tag;
    using deserialize_tag = recurseria::deserialize_tag;

    template<typename T>
    using type_tag = recurseria::type_tag<T>;

    // serialize

    YAML::Node tag_invoke(yaml_format_tag, serialize_tag, type_tag<YAML::Node>, const int& input) {
        return YAML::Node(std::to_string(input));
    }

    YAML::Node tag_invoke(yaml_format_tag, serialize_tag, type_tag<YAML::Node>, const long& input) {
        return YAML::Node(std::to_string(input));
    }

    YAML::Node tag_invoke(yaml_format_tag, serialize_tag, type_tag<YAML::Node>, const long long& input) {
        return YAML::Node(std::to_string(input));
    }

    YAML::Node tag_invoke(yaml_format_tag, serialize_tag, type_tag<YAML::Node>, const unsigned int& input) {
        return YAML::Node(std::to_string(input));
    }

    YAML::Node tag_invoke(yaml_format_tag, serialize_tag, type_tag<YAML::Node>, const unsigned long& input) {
        return YAML::Node(std::to_string(input));
    }

    YAML::Node tag_invoke(yaml_format_tag, serialize_tag, type_tag<YAML::Node>, const unsigned long long& input) {
        return YAML::Node(std::to_string(input));
    }

    YAML::Node tag_invoke(yaml_format_tag, serialize_tag, type_tag<YAML::Node>, const float& input) {
        return YAML::Node(std::to_string(input));
    }

    YAML::Node tag_invoke(yaml_format_tag, serialize_tag, type_tag<YAML::Node>, const double& input) {
        return YAML::Node(std::to_string(input));
    }

    YAML::Node tag_invoke(yaml_format_tag, serialize_tag, type_tag<YAML::Node>, const long double& input) {
        return YAML::Node(std::to_string(input));
    }

    YAML::Node tag_invoke(yaml_format_tag, serialize_tag, type_tag<YAML::Node>, const bool& input) {
        return YAML::Node(input);
    }

    YAML::Node tag_invoke(yaml_format_tag, serialize_tag, type_tag<YAML::Node>, const char& input) {
        return YAML::Node(std::string(1, input));
    }

    YAML::Node tag_invoke(yaml_format_tag, serialize_tag, type_tag<YAML::Node>, const std::string& input) {
        return YAML::Node(input);
    }

    // deserialize

    int tag_invoke(yaml_format_tag, deserialize_tag, type_tag<int>, const YAML::Node& input) {
        return input.as<int>();
    }

    long tag_invoke(yaml_format_tag, deserialize_tag, type_tag<long>, const YAML::Node& input) {
        return input.as<long>();
    }

    long long tag_invoke(yaml_format_tag, deserialize_tag, type_tag<long long>, const YAML::Node& input) {
        return input.as<long long>();
    }

    unsigned int tag_invoke(yaml_format_tag, deserialize_tag, type_tag<unsigned int>, const YAML::Node& input) {
        return input.as<unsigned int>();
    }

    unsigned long tag_invoke(yaml_format_tag, deserialize_tag, type_tag<unsigned long>, const YAML::Node& input) {
        return input.as<unsigned long>();
    }

    unsigned long long tag_invoke(yaml_format_tag, deserialize_tag, type_tag<unsigned long long>, const YAML::Node& input) {
        return input.as<unsigned long long>();
    }

    float tag_invoke(yaml_format_tag, deserialize_tag, type_tag<float>, const YAML::Node& input) {
        return input.as<float>();
    }

    double tag_invoke(yaml_format_tag, deserialize_tag, type_tag<double>, const YAML::Node& input) {
        return input.as<double>();
    }

    long double tag_invoke(yaml_format_tag, deserialize_tag, type_tag<long double>, const YAML::Node& input) {
        return input.as<long double>();
    }

    bool tag_invoke(yaml_format_tag, deserialize_tag, type_tag<bool>, const YAML::Node& input) {
        return input.as<bool>();
    }

    char tag_invoke(yaml_format_tag, deserialize_tag, type_tag<char>, const YAML::Node& input) {
        auto s = input.as<std::string>();
        if (s.size() != 1) {
            throw std::invalid_argument(std::string("Invalid char value: ") + s);
        }
        return s[0];
    }

    std::string tag_invoke(yaml_format_tag, deserialize_tag, type_tag<std::string>, const YAML::Node& input) {
        return input.as<std::string>();
    }
}

import std;
import yaml_cpp;
import boost_ut;
import recurseria.core;
import recurseria.target.yaml;
import recurseria.core.meta.types_srlz_dsrlz;

struct Person {
    int id;
    double score;
    std::string name;
};

using recurseria::core::meta::srlz_dsrlz_round_trip;
using recurseria::core::meta::srlz_dsrlz_round_trip_validation;
using recurseria::core::meta::group_sequentially;
using recurseria::core::meta::decompose_sequentially;

using recurseria::target::yaml::yaml_format_tag;
using Tag = yaml_format_tag;

int main() {
    "scalar int round-trip"_test = [] {
        int original = 42;
        auto result = srlz_dsrlz_round_trip<Tag, int, YAML::Node>(original);
        expect(result == 42);
    };

    "scalar double round-trip"_test = [] {
        double original = 3.14;
        auto result = srlz_dsrlz_round_trip<Tag, double, YAML::Node>(original);
        expect(std::abs(result - 3.14) < 1e-10);
    };

    "scalar bool round-trip"_test = [] {
        bool original = true;
        auto result = srlz_dsrlz_round_trip<Tag, bool, YAML::Node>(original);
        expect(result == true);
    };

    "scalar string round-trip"_test = [] {
        std::string original = "hello";
        auto result = srlz_dsrlz_round_trip<Tag, std::string, YAML::Node>(original);
        expect(result == "hello");
    };

    "aggregate Person round-trip"_test = [] {
        Person original{42, 3.14, "hello"};
        auto result = srlz_dsrlz_round_trip<Tag, Person, YAML::Node>(original);
        expect(result.id == 42);
        expect(result.name == "hello");
    };

    "vector<int> round-trip"_test = [] {
        std::vector<int> original{1, 2, 3, 4, 5};
        auto result = srlz_dsrlz_round_trip<Tag, std::vector<int>, YAML::Node>(original);
        expect(result.size() == 5u);
        expect(result[0] == 1);
        expect(result[4] == 5);
    };

    "map<string,int> round-trip"_test = [] {
        std::map<std::string, int> original{{"a", 1}, {"b", 2}};
        auto result = srlz_dsrlz_round_trip<Tag, std::map<std::string, int>, YAML::Node>(original);
        expect(result.at("a") == 1);
        expect(result.at("b") == 2);
    };

    "tuple round-trip"_test = [] {
        using T = std::tuple<int, double, std::string>;
        T original{42, 3.14, "hi"};
        bool ok = srlz_dsrlz_round_trip_validation<Tag, T, YAML::Node>(original);
        expect(ok);
    };

    "vector<Person> round-trip"_test = [] {
        std::vector<Person> original{{1, 1.0, "a"}, {2, 2.0, "b"}};
        auto result = srlz_dsrlz_round_trip<Tag, std::vector<Person>, YAML::Node>(original);
        expect(result.size() == 2u);
        expect(result[0].id == 1);
        expect(result[1].name == "b");
    };

    "group_sequentially round-trip"_test = [] {
        YAML::Node n1("a");
        YAML::Node n2("b");
        YAML::Node n3("c");
        std::vector<YAML::Node> input{n1, n2, n3};

        auto grouped = group_sequentially(Tag{}, input);
        expect(grouped.IsSequence());
        expect(grouped.size() == 3u);

        auto decomposed = decompose_sequentially(Tag{}, grouped);
        expect(decomposed.size() == 3u);

        auto v0 = decomposed[0].as<std::string>();
        auto v1 = decomposed[1].as<std::string>();
        auto v2 = decomposed[2].as<std::string>();
        expect(v0 == "a");
        expect(v1 == "b");
        expect(v2 == "c");
    };

    "group_sequentially empty"_test = [] {
        std::vector<YAML::Node> empty;
        auto grouped = group_sequentially(Tag{}, empty);

        expect(grouped.IsSequence());
        expect(grouped.size() == 0u);

        auto decomposed = decompose_sequentially(Tag{}, grouped);
        expect(decomposed.size() == 0u);
    };

    "decompose_sequentially random access"_test = [] {
        YAML::Node seq(YAML::NodeType::Sequence);
        seq.push_back("x");
        seq.push_back("y");
        seq.push_back("z");

        auto decomposed = decompose_sequentially(Tag{}, seq);
        expect(decomposed.size() == 3u);

        auto v0 = decomposed[0].as<std::string>();
        auto v1 = decomposed[1].as<std::string>();
        auto v2 = decomposed[2].as<std::string>();
        expect(v0 == "x");
        expect(v1 == "y");
        expect(v2 == "z");
    };
}

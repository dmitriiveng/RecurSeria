export module recurseria.tests.yaml_target;

import std;
import yaml_cpp;
import boost.ut;
import recurseria;
import recurseria.yaml;

namespace recurseria::tests::yaml_target{
    struct Person {
        int id;
        double score;
        std::string name;
    };

    struct Address {
        std::string city;
        int zip;
    };

    struct Employee {
        std::string name;
        Address address;
        std::vector<int> tags;
    };

    using recurseria::srlz_dsrlz_round_trip;
    using recurseria::srlz_dsrlz_round_trip_validation;
    using recurseria::group_sequentially;
    using recurseria::decompose_sequentially;
    using recurseria::serialize;
    using recurseria::deserialize;

    using recurseria::yaml::yaml_format_tag;
    using Tag = yaml_format_tag;

    template <typename T>
    T rt_round_trip(T value) {
        return srlz_dsrlz_round_trip<Tag, T, YAML::Node>(value);
    }
}

export namespace recurseria::tests::yaml_target{
    using namespace boost::ut;

    void test_primitives(){
        "scalar int round-trip"_test = [] {
            int original = 42;
            auto result = srlz_dsrlz_round_trip<Tag, int, YAML::Node>(original);
            expect(result == 42);
        };

        "yaml int round-trips 0, 1, -1"_test = [] {
            expect(rt_round_trip(0) == 0);
            expect(rt_round_trip(1) == 1);
            expect(rt_round_trip(-1) == -1);
        };

        "yaml int boundary round-trips"_test = [] {
            expect(rt_round_trip(std::numeric_limits<int>::max()) == std::numeric_limits<int>::max());
            expect(rt_round_trip(std::numeric_limits<int>::min()) == std::numeric_limits<int>::min());
        };

        "yaml integer round-trips"_test = [] {
            expect(rt_round_trip(std::numeric_limits<long>::max()) == std::numeric_limits<long>::max());
            expect(rt_round_trip(std::numeric_limits<long>::min()) == std::numeric_limits<long>::min());
            expect(rt_round_trip(std::numeric_limits<long long>::max()) == std::numeric_limits<long long>::max());
            expect(rt_round_trip(std::numeric_limits<long long>::min()) == std::numeric_limits<long long>::min());
        };

        "yaml unsigned round-trips"_test = [] {
            expect(rt_round_trip(0u) == 0u);
            expect(rt_round_trip(std::numeric_limits<unsigned int>::max()) == std::numeric_limits<unsigned int>::max());
            expect(rt_round_trip(std::numeric_limits<unsigned long>::max()) == std::numeric_limits<unsigned long>::max());
            expect(rt_round_trip(std::numeric_limits<unsigned long long>::max()) == std::numeric_limits<unsigned long long>::max());
        };

        "scalar double round-trip"_test = [] {
            double original = 3.14;
            auto result = srlz_dsrlz_round_trip<Tag, double, YAML::Node>(original);
            expect(std::abs(result - 3.14) < 1e-10);
        };

        "yaml double round-trips"_test = [] {
            double zero = 0.0;
            expect(std::abs(srlz_dsrlz_round_trip<Tag, double, YAML::Node>(zero) - 0.0) < 1e-10);
            double neg = -1.5;
            expect(std::abs(srlz_dsrlz_round_trip<Tag, double, YAML::Node>(neg) + 1.5) < 1e-10);
            double pi = 3.14159265;
            expect(std::abs(srlz_dsrlz_round_trip<Tag, double, YAML::Node>(pi) - pi) < 1e-5);
        };

        "yaml float round-trips"_test = [] {
            float v = 1.5f;
            expect(std::abs(srlz_dsrlz_round_trip<Tag, float, YAML::Node>(v) - 1.5f) < 1e-4f);
        };

        "yaml long double round-trip"_test = [] {
            long double v = 3.14159265358979323846L;
            long double result = srlz_dsrlz_round_trip<Tag, long double, YAML::Node>(v);
            expect(std::abs(result - v) < 1e-12L);
        };

        "scalar bool round-trip"_test = [] {
            bool original = true;
            auto result = srlz_dsrlz_round_trip<Tag, bool, YAML::Node>(original);
            expect(result == true);
            bool f = false;
            expect(srlz_dsrlz_round_trip<Tag, bool, YAML::Node>(f) == false);
        };

        "yaml char round-trips"_test = [] {
            expect(rt_round_trip('A') == 'A');
            expect(rt_round_trip('z') == 'z');
            expect(rt_round_trip('0') == '0');
            expect(rt_round_trip('~') == '~');
        };

        "scalar string round-trip"_test = [] {
            std::string original = "hello";
            auto result = srlz_dsrlz_round_trip<Tag, std::string, YAML::Node>(original);
            expect(result == "hello");
        };

        "yaml string round-trips"_test = [] {
            expect(rt_round_trip(std::string("")) == "");
            expect(rt_round_trip(std::string("a")) == "a");
            expect(rt_round_trip(std::string("hello world with spaces")) == "hello world with spaces");
            expect(rt_round_trip(std::string("héllo wörld 你好")) == "héllo wörld 你好");
            std::string padded = "  leading and trailing  ";
            expect(rt_round_trip(padded) == padded);
            std::string multiline = "line1\nline2";
            expect(rt_round_trip(multiline) == multiline);
        };

        "yaml long string round-trip"_test = [] {
            std::string long_str(1000, 'y');
            expect(rt_round_trip(long_str) == long_str);
        };
    }

    void test_reflection(){
        "aggregate Person round-trip"_test = [] {
            Person original{42, 3.14, "hello"};
            auto result = srlz_dsrlz_round_trip<Tag, Person, YAML::Node>(original);
            expect(result.id == 42);
            expect(result.name == "hello");
        };

        "yaml aggregate Person edge values"_test = [] {
            Person original{std::numeric_limits<int>::min(), -1.5, ""};
            auto result = srlz_dsrlz_round_trip<Tag, Person, YAML::Node>(original);
            expect(result.id == std::numeric_limits<int>::min());
            expect(result.score == -1.5);
            expect(result.name == "");
        };

        "yaml nested aggregate round-trip"_test = [] {
            Employee original{"alice", {"moscow", 101000}, {1, 2, 3}};
            auto result = srlz_dsrlz_round_trip<Tag, Employee, YAML::Node>(original);
            expect(result.name == "alice");
            expect(result.address.city == "moscow");
            expect(result.address.zip == 101000);
            expect(result.tags == std::vector<int>({1, 2, 3}));
        };

        "yaml end-to-end via Dump/Load"_test = [] {
            Person original{42, 3.14, "hello"};
            auto node = serialize.as<Tag, YAML::Node>(original);
            std::string text = YAML::Dump(node);
            YAML::Node reloaded = YAML::Load(text);
            Person result = deserialize.as<Tag, Person, YAML::Node>(reloaded);
            expect(result.id == 42);
            expect(result.name == "hello");
        };
    }

    void test_containers(){
        "vector<int> round-trip"_test = [] {
            std::vector<int> original{1, 2, 3, 4, 5};
            auto result = srlz_dsrlz_round_trip<Tag, std::vector<int>, YAML::Node>(original);
            expect(result.size() == 5u);
            expect(result[0] == 1);
            expect(result[4] == 5);
        };

        "yaml empty vector round-trip"_test = [] {
            std::vector<int> empty;
            auto result = srlz_dsrlz_round_trip<Tag, std::vector<int>, YAML::Node>(empty);
            expect(result.empty());
        };

        "yaml vector<int> edge values"_test = [] {
            std::vector<int> v{std::numeric_limits<int>::min(), -1, 0, 1, std::numeric_limits<int>::max()};
            auto result = srlz_dsrlz_round_trip<Tag, std::vector<int>, YAML::Node>(v);
            expect(result == v);
        };

        "yaml vector<double> round-trip"_test = [] {
            std::vector<double> v{0.0, 1.5, -2.5, 3.14159265};
            auto result = srlz_dsrlz_round_trip<Tag, std::vector<double>, YAML::Node>(v);
            expect(result.size() == 4u);
            expect(std::abs(result[0] - 0.0) < 1e-10);
            expect(std::abs(result[1] - 1.5) < 1e-10);
            expect(std::abs(result[3] - 3.14159265) < 1e-5);
        };

        "yaml vector<string> round-trip"_test = [] {
            std::vector<std::string> v{"", "a", "hello", "héllo", "with spaces"};
            auto result = srlz_dsrlz_round_trip<Tag, std::vector<std::string>, YAML::Node>(v);
            expect(result == v);
        };

        "yaml nested vector<vector<int>> round-trip"_test = [] {
            std::vector<std::vector<int>> v{{}, {1}, {1, 2, 3}};
            auto result = srlz_dsrlz_round_trip<Tag, std::vector<std::vector<int>>, YAML::Node>(v);
            expect(result.size() == 3u);
            expect(result[0].empty());
            expect(result[1] == std::vector<int>({1}));
            expect(result[2] == std::vector<int>({1, 2, 3}));
        };

        "map<string,int> round-trip"_test = [] {
            std::map<std::string, int> original{{"a", 1}, {"b", 2}};
            auto result = srlz_dsrlz_round_trip<Tag, std::map<std::string, int>, YAML::Node>(original);
            expect(result.at("a") == 1);
            expect(result.at("b") == 2);
        };

        "yaml empty map round-trip"_test = [] {
            std::map<std::string, int> empty;
            auto result = srlz_dsrlz_round_trip<Tag, std::map<std::string, int>, YAML::Node>(empty);
            expect(result.empty());
        };

        "yaml map<int,string> round-trip"_test = [] {
            std::map<int, std::string> original{{1, "one"}, {2, "two"}};
            auto result = srlz_dsrlz_round_trip<Tag, std::map<int, std::string>, YAML::Node>(original);
            expect(result.at(1) == "one");
            expect(result.at(2) == "two");
        };
    }

    void test_tuple(){
        "tuple round-trip"_test = [] {
            using T = std::tuple<int, double, std::string>;
            T original{42, 3.14, "hi"};
            bool ok = srlz_dsrlz_round_trip_validation<Tag, T, YAML::Node>(original);
            expect(ok);
        };

        "yaml tuple edge values round-trip"_test = [] {
            using T = std::tuple<int, double, std::string>;
            T original{std::numeric_limits<int>::min(), 3.14159265, ""};
            auto result = srlz_dsrlz_round_trip<Tag, T, YAML::Node>(original);
            expect(std::get<0>(result) == std::numeric_limits<int>::min());
            expect(std::abs(std::get<1>(result) - 3.14159265) < 1e-5);
            expect(std::get<2>(result) == "");
        };

        "yaml single-element tuple round-trip"_test = [] {
            using T = std::tuple<int>;
            T original{42};
            auto result = srlz_dsrlz_round_trip<Tag, T, YAML::Node>(original);
            expect(std::get<0>(result) == 42);
        };

        "yaml nested tuple round-trip"_test = [] {
            using T = std::tuple<std::tuple<int, int>, std::string>;
            T original{{1, 2}, "ab"};
            auto result = srlz_dsrlz_round_trip<Tag, T, YAML::Node>(original);
            expect(std::get<0>(std::get<0>(result)) == 1);
            expect(std::get<1>(std::get<0>(result)) == 2);
            expect(std::get<1>(result) == "ab");
        };
    }

    void test_mixed(){
        "vector<Person> round-trip"_test = [] {
            std::vector<Person> original{{1, 1.0, "a"}, {2, 2.0, "b"}};
            auto result = srlz_dsrlz_round_trip<Tag, std::vector<Person>, YAML::Node>(original);
            expect(result.size() == 2u);
            expect(result[0].id == 1);
            expect(result[1].name == "b");
        };

        "yaml vector<Employee> round-trip"_test = [] {
            std::vector<Employee> v{{"a", {"x", 1}, {}}, {"b", {"y", 2}, {1, 2}}};
            auto result = srlz_dsrlz_round_trip<Tag, std::vector<Employee>, YAML::Node>(v);
            expect(result.size() == 2u);
            expect(result[0].name == "a");
            expect(result[0].address.city == "x");
            expect(result[0].tags.empty());
            expect(result[1].address.zip == 2);
            expect(result[1].tags == std::vector<int>({1, 2}));
        };

        "yaml map<string, vector<int>> round-trip"_test = [] {
            std::map<std::string, std::vector<int>> original{{"a", {1, 2}}, {"b", {}}};
            auto result = srlz_dsrlz_round_trip<Tag, std::map<std::string, std::vector<int>>, YAML::Node>(original);
            expect(result.at("a") == std::vector<int>({1, 2}));
            expect(result.at("b").empty());
        };

        "yaml vector<tuple<int,string>> round-trip"_test = [] {
            std::vector<std::tuple<int, std::string>> v{{1, "a"}, {2, "bb"}};
            auto result = srlz_dsrlz_round_trip<Tag, std::vector<std::tuple<int, std::string>>, YAML::Node>(v);
            expect(result.size() == 2u);
            expect(std::get<0>(result[0]) == 1);
            expect(std::get<1>(result[1]) == "bb");
        };

        "yaml end-to-end vector<Person> via Dump/Load"_test = [] {
            std::vector<Person> v{{1, 1.5, "a"}, {2, 2.5, "b"}};
            auto node = serialize.as<Tag, YAML::Node>(v);
            auto text = YAML::Dump(node);
            auto reloaded = YAML::Load(text);
            auto result = deserialize.as<Tag, std::vector<Person>, YAML::Node>(reloaded);
            expect(result.size() == 2u);
            expect(result[1].name == "b");
        };
    }

    void test_grouping_decomposing(){
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

        "yaml group single element"_test = [] {
            YAML::Node a("a");
            auto grouped = group_sequentially(Tag{}, std::vector<YAML::Node>{a});
            expect(grouped.size() == 1u);
            auto decomposed = decompose_sequentially(Tag{}, grouped);
            expect(decomposed.size() == 1u);
            expect(decomposed[0].as<std::string>() == "a");
        };

        "yaml group many elements preserves order"_test = [] {
            std::vector<YAML::Node> input;
            for (int i = 0; i < 100; ++i) {
                input.emplace_back(std::to_string(i));
            }
            auto grouped = group_sequentially(Tag{}, input);
            expect(grouped.size() == 100u);
            auto decomposed = decompose_sequentially(Tag{}, grouped);
            expect(decomposed.size() == 100u);
            expect(decomposed[0].as<std::string>() == "0");
            expect(decomposed[50].as<std::string>() == "50");
            expect(decomposed[99].as<std::string>() == "99");
        };

        "yaml group mixed node types"_test = [] {
            YAML::Node scalar("x");
            YAML::Node seq(YAML::NodeType::Sequence);
            seq.push_back("inner");
            YAML::Node map(YAML::NodeType::Map);
            map["k"] = "v";

            auto grouped = group_sequentially(Tag{}, std::vector<YAML::Node>{scalar, seq, map});
            expect(grouped.size() == 3u);

            auto decomposed = decompose_sequentially(Tag{}, grouped);
            expect(decomposed.size() == 3u);
            expect(decomposed[0].as<std::string>() == "x");
            expect(decomposed[1].IsSequence());
            expect(decomposed[1][0].as<std::string>() == "inner");
            expect(decomposed[2].IsMap());
            expect(decomposed[2]["k"].as<std::string>() == "v");
        };

        "yaml nested group/decompose"_test = [] {
            YAML::Node a("a");
            YAML::Node b("b");

            auto inner = group_sequentially(Tag{}, std::vector<YAML::Node>{a, b});
            expect(inner.size() == 2u);

            auto outer = group_sequentially(Tag{}, std::vector<YAML::Node>{inner});
            expect(outer.size() == 1u);

            auto decomposed_outer = decompose_sequentially(Tag{}, outer);
            expect(decomposed_outer.size() == 1u);

            auto decomposed_inner = decompose_sequentially(Tag{}, decomposed_outer[0]);
            expect(decomposed_inner.size() == 2u);
            expect(decomposed_inner[0].as<std::string>() == "a");
            expect(decomposed_inner[1].as<std::string>() == "b");
        };
    }

    void test_error_handling(){
        "yaml deserialize wrong type throws"_test = [] {
            YAML::Node n("not_a_number");
            expect(throws<std::exception>([&] {
                deserialize.as<Tag, int>(n);
            }));
        };

        "yaml deserialize invalid char throws"_test = [] {
            YAML::Node n("too long for a char");
            expect(throws<std::exception>([&] {
                deserialize.as<Tag, char>(n);
            }));
        };
    }

    void test_yaml(){
        test_primitives();
        test_reflection();
        test_containers();
        test_tuple();
        test_mixed();
        test_grouping_decomposing();
        test_error_handling();
    }
}

import std;
import boost_ut;
import recurseria.core;
import recurseria.target.string.intermediate_representation;

struct Person {
    int id;
    double score;
    std::string name;
};

using recurseria::core::meta::srlz_dsrlz_round_trip;
using recurseria::core::meta::srlz_dsrlz_round_trip_validation;
using recurseria::target::string::StringIRTreeNode;
using recurseria::target::string::string_intermediate_representation_format_tag;

int main() {
    "scalar int"_test = [] {
        int original = 42;
        int result = srlz_dsrlz_round_trip<
            string_intermediate_representation_format_tag, int, StringIRTreeNode
        >(original);
        expect(result == 42);
    };

    "aggregate Person"_test = [] {
        Person original{42, 3.14, "hello"};
        Person result = srlz_dsrlz_round_trip<
            string_intermediate_representation_format_tag, Person, StringIRTreeNode
        >(original);
        expect(result.id == 42);
        expect(result.name == "hello");
    };

    "vector<int>"_test = [] {
        std::vector<int> original{1, 2, 3, 4, 5};
        auto result = srlz_dsrlz_round_trip<
            string_intermediate_representation_format_tag, std::vector<int>, StringIRTreeNode
        >(original);
        expect(result.size() == 5u);
        expect(result[0] == 1);
        expect(result[4] == 5);
    };

    "map<string,int>"_test = [] {
        std::map<std::string, int> original{{"a", 1}, {"b", 2}};
        auto result = srlz_dsrlz_round_trip<
            string_intermediate_representation_format_tag, std::map<std::string, int>, StringIRTreeNode
        >(original);
        expect(result.at("a") == 1);
        expect(result.at("b") == 2);
    };

    "tuple<int,double,string>"_test = [] {
        using T = std::tuple<int, double, std::string>;
        T original{42, 3.14, "hi"};
        bool ok = srlz_dsrlz_round_trip_validation<
            string_intermediate_representation_format_tag, T, StringIRTreeNode
        >(original);
        expect(ok);
    };

    "vector<Person>"_test = [] {
        std::vector<Person> original{{1, 1.0, "a"}, {2, 2.0, "b"}};
        auto result = srlz_dsrlz_round_trip<
            string_intermediate_representation_format_tag, std::vector<Person>, StringIRTreeNode
        >(original);
        expect(result.size() == 2u);
        expect(result[0].id == 1);
        expect(result[1].name == "b");
    };
}

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
using recurseria::core::meta::group_sequentially;
using recurseria::core::meta::decompose_sequentially;
using recurseria::target::string::StringIRTreeNode;
using recurseria::target::string::string_intermediate_representation_format_tag;

using Tag = string_intermediate_representation_format_tag;

auto leaf(std::string s) {
    return StringIRTreeNode(std::move(s));
}

auto branch(std::vector<StringIRTreeNode> children) {
    return StringIRTreeNode(std::move(children));
}

auto make_leaves(std::initializer_list<std::string> values) {
    std::vector<StringIRTreeNode> v;
    v.reserve(values.size());
    for (auto& s : values)
        v.push_back(leaf(std::string(s)));
    return v;
}

int main() {
    "serialization scalar int"_test = [] {
        int original = 42;
        int result = srlz_dsrlz_round_trip<Tag, int, StringIRTreeNode>(original);
        expect(result == 42);
    };

    "serialization aggregate Person"_test = [] {
        Person original{42, 3.14, "hello"};
        Person result = srlz_dsrlz_round_trip<Tag, Person, StringIRTreeNode>(original);
        expect(result.id == 42);
        expect(result.name == "hello");
    };

    "serialization vector<int>"_test = [] {
        std::vector<int> original{1, 2, 3, 4, 5};
        auto result = srlz_dsrlz_round_trip<Tag, std::vector<int>, StringIRTreeNode>(original);
        expect(result.size() == 5u);
        expect(result[0] == 1);
        expect(result[4] == 5);
    };

    "serialization map<string,int>"_test = [] {
        std::map<std::string, int> original{{"a", 1}, {"b", 2}};
        auto result = srlz_dsrlz_round_trip<Tag, std::map<std::string, int>, StringIRTreeNode>(original);
        expect(result.at("a") == 1);
        expect(result.at("b") == 2);
    };

    "serialization tuple<int,double,string>"_test = [] {
        using T = std::tuple<int, double, std::string>;
        T original{42, 3.14, "hi"};
        bool ok = srlz_dsrlz_round_trip_validation<Tag, T, StringIRTreeNode>(original);
        expect(ok);
    };

    "serialization vector<Person>"_test = [] {
        std::vector<Person> original{{1, 1.0, "a"}, {2, 2.0, "b"}};
        auto result = srlz_dsrlz_round_trip<Tag, std::vector<Person>, StringIRTreeNode>(original);
        expect(result.size() == 2u);
        expect(result[0].id == 1);
        expect(result[1].name == "b");
    };

    "group_sequentially round-trip"_test = [] {
        auto leaves = make_leaves({"a", "b", "c"});
        auto grouped = group_sequentially(Tag{}, leaves);

        expect(grouped.is_branch());
        auto& children = grouped.get_sequence();
        expect(children.size() == 3u);
        expect(children[0].get_string() == "a");
        expect(children[1].get_string() == "b");
        expect(children[2].get_string() == "c");

        auto decomposed = decompose_sequentially(Tag{}, grouped);
        expect(decomposed.size() == 3u);
        expect(decomposed[0].get_string() == "a");
        expect(decomposed[1].get_string() == "b");
        expect(decomposed[2].get_string() == "c");
    };

    "group_sequentially nested"_test = [] {
        auto inner = branch(make_leaves({"x", "y"}));
        auto outer = group_sequentially(Tag{}, std::vector{leaf("top"), inner});

        expect(outer.is_branch());
        auto& children = outer.get_sequence();
        expect(children.size() == 2u);
        expect(children[0].get_string() == "top");
        expect(children[1].is_branch());

        auto decomposed = decompose_sequentially(Tag{}, outer);
        expect(decomposed.size() == 2u);
        expect(decomposed[0].get_string() == "top");

        auto inner_view = decompose_sequentially(Tag{}, decomposed[1]);
        expect(inner_view.size() == 2u);
        expect(inner_view[0].get_string() == "x");
        expect(inner_view[1].get_string() == "y");
    };

    "group_sequentially empty"_test = [] {
        std::vector<StringIRTreeNode> empty;
        auto grouped = group_sequentially(Tag{}, empty);

        expect(grouped.is_branch());
        expect(grouped.get_sequence().empty());

        auto decomposed = decompose_sequentially(Tag{}, grouped);
        expect(decomposed.empty());
    };


}

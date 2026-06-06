export module recurseria.target.string.intermediate_representation:tree_node;

import std;

export namespace recurseria::target::string {
    struct StringIRTreeNode final {
        using Sequence = std::vector<StringIRTreeNode>;

        std::variant<std::string, Sequence> data;

        StringIRTreeNode(std::string string) : data(std::move(string)) {}
        StringIRTreeNode(Sequence sequence) : data(std::move(sequence))  {}

        [[nodiscard]] bool is_leaf() const noexcept {
            return std::holds_alternative<std::string>(data);
        }

        [[nodiscard]] bool is_branch() const noexcept {
            return std::holds_alternative<Sequence>(data);
        }

        // throws exceptions
        [[nodiscard]] const Sequence& get_sequence() const {
            return std::get<Sequence>(data);
        }

        // throws exceptions
        [[nodiscard]] const std::string& get_string() const {
            return std::get<std::string>(data);
        }
    };
}

module;
#include <vector>

export module recurseria.core.meta.helpers.sequence_ops:default_groupable;

export import recurseria.core.meta.tag_invokable;

export namespace recurseria::core::meta {
    struct default_group_sequentially_tag{};

    template <typename T>
    concept DefaultSequentiallyGroupable = requires(T& out, const std::vector<T>& value) {
        tag_invoke(default_group_sequentially_tag{}, out, value);
    };
}

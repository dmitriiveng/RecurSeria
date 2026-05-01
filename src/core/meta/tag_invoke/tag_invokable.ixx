module;
#include <utility>

export module recurseria.core.meta.tag_invokable;

export namespace recurseria::core::meta {
    void tag_invoke();

    template<class Tag, class... Args>
    concept TagInvokable = requires(Tag tag, Args&&... args) {
        tag_invoke(tag, std::forward<Args>(args)...);
    };

    template<class Tag, class... Args>
    using tag_invoke_result_t = decltype(tag_invoke(std::declval<Tag>(), std::declval<Args>()...));
}

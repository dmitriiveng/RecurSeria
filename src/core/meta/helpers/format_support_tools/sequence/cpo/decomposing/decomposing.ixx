export module recurseria.core.meta.helpers.sequence_ops:decomposing;

import std;
export import :default_decomposable;

export import recurseria.core.meta.tag_invokable;
export import recurseria.core.meta.exceptions;

export namespace recurseria::core::meta {
    struct decompose_sequentially_tag {};

    template <typename FormatTag, typename T>
    concept SequentiallyDecomposable =
        requires(T&& value) {
            {
                tag_invoke(FormatTag{}, decompose_sequentially_tag{}, std::forward<T>(value))
            } -> std::ranges::view;
        };

    /// Decomposes a value into a view of its elements.
    ///
    /// @tparam FormatTag serialization format tag
    /// @tparam T         value type to decompose
    /// @param  value     value to decompose
    /// @return a view over the decomposed elements
    ///
    /// @throws tag_invoke_error if the underlying `tag_invoke` raises an exception.
    inline constexpr struct decompose_sequentially_fn {
        template<typename FormatTag, typename T>
            requires SequentiallyDecomposable<FormatTag, T> || DefaultSequentiallyDecomposable<FormatTag, T>
        constexpr auto operator()(FormatTag, const T& value) const {
            if constexpr (SequentiallyDecomposable<FormatTag, T>){
                try {
                    return tag_invoke(FormatTag{}, decompose_sequentially_tag{}, value);
                } catch (const std::exception& e) {
                    throw tag_invoke_error("decompose_sequentially", typeid(T).name(), e.what());
                }
            }
            else if constexpr (DefaultSequentiallyDecomposable<FormatTag, T>){
                try {
                    return tag_invoke(FormatTag{}, default_decompose_sequentially_tag{}, value);
                } catch (const std::exception& e) {
                    throw tag_invoke_error("decompose_sequentially", typeid(T).name(), e.what());
                }
            }
        }
    } decompose_sequentially;
}

export module recurseria.core.meta.helpers.associative_ops:decomposing;

import std;
export import :default_decomposable;
import :pair_concept;
export import :decomposable;
export import :decomposable_noexcept;

export import recurseria.core.meta.tag_invokable;
export import recurseria.core.meta.exceptions;

export namespace recurseria::core::meta {

    inline constexpr struct decompose_associatively_fn {
        template<typename FormatTag, typename T>
            requires AssociativelyDecomposableNoexcept<FormatTag, T>
        constexpr auto operator()(FormatTag, const T& value) const noexcept
        {
            if constexpr (AssociativelyDecomposable<FormatTag, T> &&
                          noexcept(tag_invoke(FormatTag{}, decompose_associatively_tag{}, value)))
            {
                return tag_invoke(FormatTag{}, decompose_associatively_tag{}, value);
            } else {
                return tag_invoke(FormatTag{}, default_decompose_associatively_tag{}, value);
            }
        }

        template<typename FormatTag, typename T>
            requires AssociativelyDecomposable<FormatTag, T> || DefaultAssociativelyDecomposable<FormatTag, T>
        constexpr auto operator()(FormatTag, const T& value) const {
            if constexpr (AssociativelyDecomposable<FormatTag, T>){
                try {
                    return tag_invoke(FormatTag{}, decompose_associatively_tag{}, value);
                } catch (const std::exception& e) {
                    throw tag_invoke_error("decompose_associatively", typeid(T).name(), e.what());
                }
            }
            else {
                try {
                    return tag_invoke(FormatTag{}, default_decompose_associatively_tag{}, value);
                } catch (const std::exception& e) {
                    throw tag_invoke_error("decompose_associatively", typeid(T).name(), e.what());
                }
            }
        }
    } decompose_associatively;
}

export module recurseria.core.meta.helpers.sequence_ops:decomposing;

import std;
export import :default_decomposable;
export import :decomposable;
export import :decomposable_noexcept;

export import recurseria.core.meta.tag_invokable;
export import recurseria.core.meta.exceptions;

export namespace recurseria {
    inline constexpr struct decompose_sequentially_fn {
        template<typename FormatTag, typename T>
            requires SequentiallyDecomposableNoexcept<FormatTag, T>
        constexpr auto operator()(FormatTag, const T& value) const noexcept
        {
            if constexpr (UserDefinedSequentiallyDecomposable<FormatTag, T> &&
                          noexcept(tag_invoke(FormatTag{}, decompose_sequentially_tag{}, value)))
            {
                return tag_invoke(FormatTag{}, decompose_sequentially_tag{}, value);
            } else {
                return tag_invoke(FormatTag{}, default_decompose_sequentially_tag{}, value);
            }
        }

        template<typename FormatTag, typename T>
            requires SequentiallyDecomposable<FormatTag, T>
        constexpr auto operator()(FormatTag, const T& value) const {
            if constexpr (UserDefinedSequentiallyDecomposable<FormatTag, T>){
                try {
                    return tag_invoke(FormatTag{}, decompose_sequentially_tag{}, value);
                } catch (const std::exception& e) {
                    throw tag_invoke_error("decompose_sequentially", typeid(T).name(), e.what());
                } catch (...) {
                    throw tag_invoke_error("decompose_sequentially", typeid(T).name(), "unknown exception");
                }
            }
            else {
                try {
                    return tag_invoke(FormatTag{}, default_decompose_sequentially_tag{}, value);
                } catch (const std::exception& e) {
                    throw tag_invoke_error("decompose_sequentially", typeid(T).name(), e.what());
                } catch (...) {
                    throw tag_invoke_error("decompose_sequentially", typeid(T).name(), "unknown exception");
                }
            }
        }
    } decompose_sequentially;
}

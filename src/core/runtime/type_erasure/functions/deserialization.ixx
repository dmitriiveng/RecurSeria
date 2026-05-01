module;
#include <functional>

export module recurseria.core.runtime.type_erasure.functions:dsrlz;

export import recurseria.core.runtime.type_erasure.types;

export namespace recurseria::core::runtime {
    template <typename InputV>
    using type_erased_dsrlz_func = std::function<TypeErasedValueOwner(const InputV&)>;
}

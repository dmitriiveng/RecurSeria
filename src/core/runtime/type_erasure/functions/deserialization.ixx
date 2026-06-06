export module recurseria.core.runtime.type_erasure.functions:dsrlz;

import std;

export import recurseria.core.runtime.type_erasure.types;

export namespace recurseria::core::runtime {
    template <typename InputV>
    using type_erased_dsrlz_func = std::function<TypeErasedValueOwner(const InputV&)>;
}

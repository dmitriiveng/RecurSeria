export module recurseria.core.runtime.type_erasure.functions:srlz;

import std;

export import recurseria.core.runtime.type_erasure.types;

export namespace recurseria::core::runtime {
    template <typename OutputV>
    using type_erased_srlz_func = std::function<OutputV(const TypeErasedValuePtr)>;
}

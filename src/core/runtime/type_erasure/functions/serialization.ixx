module;
#include <functional>

export module serde.core.runtime.type_erasure.functions.serialization;

export import serde.core.runtime.type_erasure.types;

export namespace serde {
    template <typename OutputV>
    using type_erased_srlz_func = std::function<OutputV(const TypeErasedValuePtr)>;
}

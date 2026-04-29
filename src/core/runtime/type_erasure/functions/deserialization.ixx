module;
#include <functional>

export module serde.core.runtime.type_erasure.functions.deserialization;

export import serde.core.runtime.type_erasure.types;

export namespace serde {
    template <typename InputV>
    using type_erased_dsrlz_func = std::function<TypeErasedValueOwner(const InputV&)>;
}

module;
#include <functional>
#include <any>

export module serde.core.runtime.type_erasure.functions.deserialization;

export namespace serde {
    template <typename InputV>
    using type_erased_dsrlz_func = std::function<std::any(const InputV&)>;
}
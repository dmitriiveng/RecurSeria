export module serde.core.runtime.type_erasure.functions.serialization;

import <functional>;

export namespace serde::srlz {
    template <typename OutputV>
    using type_erased_srlz_func = std::function<OutputV(const void*)>;
}
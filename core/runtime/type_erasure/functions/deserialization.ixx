export module serde.core.runtime.type_erasure.functions.deserialization;

import serde.core.runtime.type_erasure.void_ptr_any;
import <functional>;

export namespace serde::dsrlz {
    template <typename InputV>
    using type_erased_dsrlz_func = std::function<VoidPtrAny(const InputV&)>;
}
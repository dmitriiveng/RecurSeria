export module serde.core.runtime.functions_generator.deserialization;

import serde.core.meta.functions.types.deserialization;
import serde.core.runtime.type_erasure.functions.deserialization;
import serde.core.runtime.type_erasure.void_ptr_any;

namespace serde::dsrlz {
    template<
        typename Output,
        typename Input
    >
    [[nodiscard]] type_erased_dsrlz_func<Input>
    gen_simple_dsrlz_func() {
        auto deserialization = [](const Input& input) {
            Output result{};
            deserialize(result, input);

            return VoidPtrAny(result);
        };

        return deserialization;
    }
}

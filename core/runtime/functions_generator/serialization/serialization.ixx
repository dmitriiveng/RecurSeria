export module serde.core.runtime.functions_generator.serialization;

import serde.core.meta.functions.types.serialization;
import serde.core.runtime.type_erasure.functions.serialization;

namespace serde::srlz {
    template<
        typename Output,
        typename Input
    >
    [[nodiscard]] type_erased_srlz_func<Output>
    gen_simple_type_srlz_func() {
        auto serialization = [](const void* input) {
            Output result{};
            serialize(result, *static_cast<const Input*>(input));

            return result;
        };

        return serialization;
    }
}
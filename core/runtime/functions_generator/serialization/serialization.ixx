export module serde.core.runtime.functions_generator.serialization;

import serde.core.meta.functions.types.serialization;
import serde.core.runtime.type_erasure.functions.serialization;

export namespace serde {
    template<
        typename Output,
        typename T
    >
    [[nodiscard]] type_erased_srlz_func<Output>
    gen_simple_type_srlz_func() {
        auto serialization = [](const void* input) -> Output {
            Output result{};
            functions::serialize(result, *static_cast<const T*>(input));

            return result;
        };

        return serialization;
    }
}
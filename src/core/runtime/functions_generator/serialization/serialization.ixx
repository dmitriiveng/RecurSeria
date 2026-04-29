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
        auto serialization = [](const TypeErasedValuePtr input) -> Output {
            Output result{};
            functions::serialize(result, input.get<T>());

            return result;
        };

        return serialization;
    }
}

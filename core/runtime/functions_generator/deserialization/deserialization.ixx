module;
#include <any>

export module serde.core.runtime.functions_generator.deserialization;

import serde.core.meta.functions.types.deserialization;
import serde.core.runtime.type_erasure.functions.deserialization;

export namespace serde {
    template<
        typename T,
        typename Input
    >
    [[nodiscard]] type_erased_dsrlz_func<Input>
    gen_simple_dsrlz_func() {
        auto deserialization = [](const Input& input) -> std::any {
            T result{};
            functions::deserialize(result, input);

            return std::any(result);
        };

        return deserialization;
    }
}

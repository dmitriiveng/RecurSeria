export module recurseria.core.runtime.functions_generator:dsrlz;

import recurseria.core.meta.types_srlz_dsrlz;
import recurseria.core.runtime.type_erasure.functions;

export namespace recurseria::core::runtime {
    template<
        typename T,
        typename Input
    >
    [[nodiscard]] type_erased_dsrlz_func<Input>
    gen_simple_dsrlz_func() {
        auto deserialization = [](const Input& input) -> TypeErasedValueOwner {
            T result{};
            recurseria::core::meta::deserialize(result, input);

            return TypeErasedValueOwner(result);
        };

        return deserialization;
    }
}

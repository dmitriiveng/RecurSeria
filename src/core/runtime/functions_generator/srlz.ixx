export module recurseria.core.runtime.functions_generator:srlz;

import recurseria.core.meta.types_srlz_dsrlz;
import recurseria.core.runtime.type_erasure.functions;

export namespace recurseria::core::runtime {
    template<
        typename Output,
        typename T
    >
    [[nodiscard]] type_erased_srlz_func<Output>
    gen_simple_type_srlz_func() {
        auto serialization = [](const TypeErasedValuePtr input) -> Output {
            Output result{};
            recurseria::core::meta::serialize(result, input.get<T>());

            return result;
        };

        return serialization;
    }
}

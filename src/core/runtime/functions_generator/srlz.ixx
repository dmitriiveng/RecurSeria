export module recurseria.core.runtime.functions_generator:srlz;

import recurseria.core.meta.types_srlz_dsrlz;
import recurseria.core.runtime.type_erasure.functions;

export namespace recurseria::core::runtime {
    template<
        typename FormatTag,
        typename Output,
        typename Input
    >
    [[nodiscard]] type_erased_srlz_func<Output>
    gen_simple_srlz_func() {
        return [](const TypeErasedValuePtr input) -> Output {
            return recurseria::core::meta::serialize.as<FormatTag, Output, Input>(input.get<Input>());
        };
    }
}

export module recurseria.core.runtime.functions_generator:srlz;

import recurseria.core.meta;
import recurseria.core.runtime.type_erasure.functions;

namespace recurseria::core::runtime::detail {
    template <typename... Ts>
    auto call_serialize_as(
        recurseria::core::meta::arg_pack<Ts...>,
        const typename recurseria::core::meta::serialize_args<recurseria::core::meta::arg_pack<Ts...>>::input_type& value
    ) -> decltype(recurseria::core::meta::serialize.as<Ts...>(value)) {
        return recurseria::core::meta::serialize.as<Ts...>(value);
    }
}

export namespace recurseria::core::runtime {
    template<recurseria::core::meta::SerializeArgPack Args>
    [[nodiscard]] type_erased_srlz_func<typename recurseria::core::meta::serialize_args<Args>::output_type>
    gen_simple_srlz_func() {
        using traits = recurseria::core::meta::serialize_args<Args>;
        return [](const TypeErasedValuePtr input) -> typename traits::output_type {
            return detail::call_serialize_as(Args{}, input.template get<typename traits::input_type>());
        };
    }
}

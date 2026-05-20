export module recurseria.core.runtime.functions_generator:dsrlz;

import recurseria.core.meta;
import recurseria.core.runtime.type_erasure.functions;

namespace recurseria::core::runtime::detail {
    template <typename... Ts>
    auto call_deserialize_as(
        recurseria::core::meta::arg_pack<Ts...>,
        const typename recurseria::core::meta::deserialize_args<recurseria::core::meta::arg_pack<Ts...>>::input_type& value
    ) -> decltype(recurseria::core::meta::deserialize.as<Ts...>(value)) {
        return recurseria::core::meta::deserialize.as<Ts...>(value);
    }
}

export namespace recurseria::core::runtime {
    template<recurseria::core::meta::DeserializeArgPack Args>
    [[nodiscard]] type_erased_dsrlz_func<typename recurseria::core::meta::deserialize_args<Args>::input_type>
    gen_simple_dsrlz_func() {
        using traits = recurseria::core::meta::deserialize_args<Args>;
        return [](const typename traits::input_type& input) -> TypeErasedValueOwner {
            return TypeErasedValueOwner(detail::call_deserialize_as(Args{}, input));
        };
    }
}

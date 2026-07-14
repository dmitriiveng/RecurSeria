export module recurseria.core.meta.types_srlz_dsrlz:reflection_srlz;

import std;

import recurseria.core.meta.helpers.reflection_helpers;
import recurseria.core.meta.helpers.sequence_ops;
import :srlz;

namespace recurseria::core::meta {
    /*

    export template <typename FormatTag, typename Output, typename Input>
    requires ReflectionSupported<Input>
    Output tag_invoke(FormatTag, default_serialize_tag, type_tag<Output>, const Input& input){
        std::map<Output, Output> mp{};
        iterate_through_fields_nv(input, [&](std::string_view name, const auto& obj){
            mp.insert(std::pair<Output, Output>(
                serialize.as<FormatTag, Output, std::string>(name),
                serialize.as<FormatTag, Output, decltype(obj)>(obj)
            ));
        });
        return group_associatively(FormatTag{}, mp);
    }

    */
}

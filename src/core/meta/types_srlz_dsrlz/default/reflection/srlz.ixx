export module recurseria.core.meta.types_srlz_dsrlz:reflection_srlz;

import std;

import recurseria.core.meta.helpers.reflection_helpers;
import recurseria.core.meta.helpers.sequence_ops;
import recurseria.core.meta.helpers.associative_ops;
import :reflection_concepts;
import :srlz;

namespace recurseria::core::meta {
    // TODO: Remove extra copy.

    // sequence
    export template <typename FormatTag, typename Output, typename Input>
    requires ReflectionSerializableDeserializable<FormatTag, Input, Output> &&
        SequenceOpsSupported<FormatTag, Output> &&
        (!AssociativeOpsSupported<FormatTag, Output>)
    Output tag_invoke(FormatTag, default_serialize_tag, type_tag<Output>, const Input& input){
        std::vector<Output> output_vector;
        iterate_through_fields(input, [&](std::string_view name, const auto& obj){
            output_vector.push_back(
                serialize.as<FormatTag, Output, decltype(obj)>(obj)
            );
        });
        return group_sequentially(FormatTag{}, output_vector);
    }

    // associative
    export template <typename FormatTag, typename Output, typename Input>
    requires ReflectionSerializableDeserializable<FormatTag, Input, Output> &&
        AssociativeOpsSupported<FormatTag, Output>
    Output tag_invoke(FormatTag, default_serialize_tag, type_tag<Output>, const Input& input){
        std::vector<std::pair<Output, Output>> output_pairs;
        iterate_through_fields(input, [&](std::string_view name, const auto& obj){
            output_pairs.emplace_back(
                serialize.as<FormatTag, Output, std::string>(std::string(name)),
                serialize.as<FormatTag, Output, decltype(obj)>(obj)
            );
        });
        return group_associatively(FormatTag{}, output_pairs);
    }

}

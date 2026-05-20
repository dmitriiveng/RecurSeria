module;

#include <concepts>

export module recurseria.core.meta.validators:srlz_dsrlz;

import recurseria.core.meta.types_srlz_dsrlz;

export namespace recurseria::core::meta {
    template <typename FormatTag, typename OriginalType, typename SerializationType>
    OriginalType srlz_dsrlz_round_trip(OriginalType& object){
        return deserialize.as<FormatTag, OriginalType, SerializationType>(
            serialize.as<FormatTag, SerializationType, OriginalType>(object)
        );
    }

    template <typename FormatTag, typename OriginalType, typename SerializationType>
        requires std::equality_comparable<OriginalType>
    bool srlz_dsrlz_round_trip_validation(OriginalType& object){
        return object == deserialize.as<FormatTag, OriginalType, SerializationType>(
            serialize.as<FormatTag, SerializationType, OriginalType>(object)
        );
    }

    //TODO put chain inside template parameters
    template <typename FormatTag, typename OriginalType, typename SerializationType, typename... ChainTypes>
        requires (sizeof...(ChainTypes) > 0)
    OriginalType srlz_dsrlz_round_trip(OriginalType& object) {
        using SerChain = chain<ChainTypes...>;
        auto serialized = serialize.as<FormatTag, SerializationType, OriginalType, SerChain>(object);
        return deserialize.as<FormatTag, OriginalType, SerializationType, chain_reverse_t<SerChain>>(serialized);
    }

    template <typename FormatTag, typename OriginalType, typename SerializationType, typename... ChainTypes>
        requires (sizeof...(ChainTypes) > 0) && std::equality_comparable<OriginalType>
    bool srlz_dsrlz_round_trip_validation(OriginalType& object) {
        return object == srlz_dsrlz_round_trip<FormatTag, OriginalType, SerializationType, ChainTypes...>(object);
    }
}

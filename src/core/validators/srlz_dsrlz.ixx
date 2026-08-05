export module recurseria.validators:srlz_dsrlz;

import std;

import recurseria.srlz_dsrlz;

export namespace recurseria {
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
}

export module serde.targets.string.string_utils.to_structure.make_pair_of_tokens;

export import serde.targets.string.string_utils.to_structure.make_token;

export namespace str_utils {
    inline std::string make_pair_of_tokens(
        const std::pair<std::string, std::string>& pair,
        const char begin_of_token,
        const char end_of_token
    ){
        std::string result
            = make_token(pair.first, begin_of_token,end_of_token)
            + make_token(pair.second, begin_of_token,end_of_token);
        return result;
    }
}
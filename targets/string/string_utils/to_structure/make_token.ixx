export module serde.targets.string.string_utils.to_structure.make_token;

export import <optional>;
export import  <stdexcept>;

export namespace str_utils {
    inline std::string make_token(
        const std::string& token,
        const char begin_of_token,
        const char end_of_token
    ){
        std::string result = begin_of_token + token + end_of_token;
        return result;
    }
}
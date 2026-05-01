module;
#include <string>

export module recurseria.targets.string.utils:make_token;

export namespace recurseria::targets::string {
    inline std::string make_token(
        const std::string& token,
        const char begin_of_token,
        const char end_of_token
    ){
        std::string result;
        result += begin_of_token;
        result += token;
        result += end_of_token;
        return result;
    }
}

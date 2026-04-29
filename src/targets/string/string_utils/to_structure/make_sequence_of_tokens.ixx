module;
#include <vector>
#include <string>

export module serde.targets.string.string_utils.to_structure.make_sequence_of_tokens;

import serde.targets.string.string_utils.to_structure.make_token;

export namespace str_utils {
    inline std::string make_sequence_of_tokens(
        const std::vector<std::string>& sequence,
        const char begin_of_token,
        const char end_of_token
    ){
        std::string result;
        for (auto& token : sequence) {
            result.append(make_token(token, begin_of_token, end_of_token));
        }
        return result;
    }
}
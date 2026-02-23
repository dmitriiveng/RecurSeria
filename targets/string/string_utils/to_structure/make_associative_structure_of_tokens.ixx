export module serde.targets.string.string_utils.to_structure.make_associative_structure_of_tokens;

import <map>;
import <string>;
import serde.targets.string.string_utils.to_structure.make_pair_of_tokens;

export namespace str_utils {
    inline std::string make_associative_structure_of_tokens(
        const std::map<std::string, std::string>& associative,
        const char begin_of_token,
        const char end_of_token
    ){
        std::string result;
        for (const std::pair<std::string, std::string> pair : associative) {
            result.append(
                make_token(
                    make_pair_of_tokens(pair, begin_of_token, end_of_token),
                        begin_of_token,
                        end_of_token
                    )
                );
        }
        return result;
    }
}
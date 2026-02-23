export module serde.targets.string.string_utils.from_structure.split_by_tokens_pair;

import <string>;
import <stdexcept>;
import <utility>;
import <iterator>;

import targets.string.string_utils.from_structure.extract_token;

export namespace str_utils {
    template <std::input_iterator It, std::sentinel_for<It> End>
    std::pair<std::string, std::string> split_by_tokens_pair(
        It& it,
        End end,
        const char begin_of_token,
        const char end_of_token,
        const char quote
    ) {
        std::pair<std::string, std::string> result;

        std::optional<std::string> token;

        try {
            token = extract_token(it, end, begin_of_token, end_of_token, quote);
        }
        catch (const std::runtime_error& e) {
            throw std::runtime_error(
                std::string("Error: '") + e.what() +
                "' while splitting into pair by tokens"
            );
        }

        if (!token)
            throw std::runtime_error(
                "Error: token for first element in pair was not found"
            );

        result.first = std::move(*token);

        try {
            token = extract_token(it, end, begin_of_token, end_of_token, quote);
        }
        catch (const std::runtime_error& e) {
            throw std::runtime_error(
                std::string("Error: '") + e.what() +
                "' while splitting into pair by tokens"
            );
        }

        if (!token)
            throw std::runtime_error(
                "Error: token for second element in pair was not found"
            );

        result.second = std::move(*token);

        return result;
    }
}


//TODO remove inline and write cpp files
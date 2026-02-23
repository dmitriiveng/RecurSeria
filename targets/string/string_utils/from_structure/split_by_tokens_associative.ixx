export module serde.targets.string.string_utils.from_structure.split_by_tokens_associative;

import <map>;
import <stdexcept>;
import <vector>;
import <string>;
import <iterator>;

import serde.targets.string.string_utils.from_structure.split_by_tokens_sequence;
import serde.targets.string.string_utils.from_structure.split_by_tokens_pair;

export namespace str_utils {
    template <std::input_iterator It, std::sentinel_for<It> End>
    std::map<std::string, std::string> split_by_tokens_associative(
        It& it,
        End end,
        const char begin_of_token,
        const char end_of_token,
        const char quote
    ) {
        std::map<std::string, std::string> result_map;

        std::vector<std::string> pairs;
        try {
            pairs = split_by_tokens_sequence<It, End>(
                it, end, begin_of_token, end_of_token, quote
            );
        }
        catch (const std::runtime_error& e) {
            throw std::runtime_error(
                std::string("Error: '") + e.what() +
                "' while splitting into map by tokens"
            );
        }

        for (auto& pair_str : pairs) {
            auto it_pair = pair_str.begin();

            std::pair<std::string, std::string> pair_val;
            try {
                pair_val = split_by_tokens_pair<
                    decltype(it_pair),
                    decltype(pair_str.end())
                >(it_pair, pair_str.end(),
                  begin_of_token, end_of_token, quote);
            }
            catch (const std::runtime_error& e) {
                throw std::runtime_error(
                    std::string("Error: '") + e.what() +
                    "' while splitting into map by tokens"
                );
            }

            if (auto [_, inserted] = result_map.insert(pair_val); !inserted)
                throw std::runtime_error("Duplicate key in map: " + pair_val.first);
        }

        return result_map;
    }
}
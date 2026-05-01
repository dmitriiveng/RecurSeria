module;
#include <vector>
#include <string>
#include <stdexcept>

export module recurseria.targets.string.utils:split_by_tokens_sequence;

import :extract_token;

export namespace recurseria::targets::string {
    template <std::input_iterator It, std::sentinel_for<It> End>
    std::vector<std::string> split_by_tokens_sequence(
        It& it,
        End end,
        const char begin_of_token,
        const char end_of_token,
        const char quote
    ) {
        std::vector<std::string> result;

        try {
            while (true) {
                auto token = extract_token(it, end, begin_of_token, end_of_token, quote);
                if (!token.has_value())
                    break;

                result.push_back(std::move(token.value()));
            }
        }
        catch (const std::runtime_error& e) {
            throw std::runtime_error(std::string("Error: '") + e.what() + std::string("' while splitting into sequence by tokens"));
        }

        return result;
    }
}

module;
#include <optional>
#include <string>
#include <stdexcept>
#include <__iterator/concepts.h>

export module serde.targets.string.string_utils.from_structure.extract_token;

import serde.targets.string.string_utils.from_structure.copy_quoted;

export namespace str_utils {
    template <std::input_iterator It, std::sentinel_for<It> End>
    std::optional<std::string> extract_token(
        It& it,
        End end,
        const char begin_of_token,
        const char end_of_token,
        const char quote
    ){
        std::string result;

        // Skip until begin
        while (it != end && *it != begin_of_token)
            ++it;

        if (it == end)
            return std::nullopt;

        ++it; // Skip begin

        size_t begin_counter = 1;
        // Read until finish
        while (it != end) {
            if (*it == quote) {
                auto quoted = copy_quoted(it, end, quote);
                if (!quoted)
                    throw std::runtime_error("Quote expected but not found");
                result += *quoted;
                continue;
            }

            const char c = *it;
            ++it;

            if (c == begin_of_token) {
                begin_counter++;
            }

            if (c == end_of_token) {
                begin_counter--;
            }

            if (begin_counter == 0) {
                break;
            }
            result.push_back(c);
        }

        if (begin_counter != 0)
            throw std::runtime_error(std::string("Unclosed token: '") + end_of_token + "'");

        return result;
    }
}
module;
#include <optional>
#include <stdexcept>
#include <string>

export module recurseria.targets.string.utils:copy_quoted;

//TODO nested string
export namespace recurseria::targets::string {
    template <std::input_iterator It, std::sentinel_for<It> End>
    std::optional<std::string> copy_quoted(It& it, End end, char quote)
    {
        std::string result;

        // Skip until quote
        while (it != end && *it != quote)
            ++it;

        if (it == end)
            return std::nullopt;

        result.push_back(*it);
        ++it;

        while (it != end) {
            const char c = *it;
            ++it;
            result.push_back(c);

            if (c == quote)
                return result;
        }

        throw std::runtime_error("Unclosed quote");
    }
}

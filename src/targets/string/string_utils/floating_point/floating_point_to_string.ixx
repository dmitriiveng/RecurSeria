module;
#include <string>

export module serde.targets.string.string_utils.floating_point.floating_point_to_string;

export namespace str_utils {
    template<typename T>
    std::string floating_point_to_string(T value) {
        std::string s = std::to_string(value);
        if(s.find('.') != std::string::npos) {
            s.erase(s.find_last_not_of('0') + 1, std::string::npos);
            if(s.back() == '.') s.pop_back();
        }
        return s;
    }
}
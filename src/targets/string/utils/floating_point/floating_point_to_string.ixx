module;
#include <string>

export module recurseria.targets.string.utils:floating_point_to_string;

export namespace recurseria::targets::string {
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

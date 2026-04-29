module;
#include <unordered_map>
#include <stdexcept>

export module serde.core.runtime.mapper:implementation;

import :interface;

export namespace serde {

    template <typename Func, typename Key>
    class SerDeFuncMapper : public ISerDeFuncMapper<Func, Key> {
        std::unordered_map<Key, Func> data;

    public:

        [[nodiscard]] Func get_function(const Key& key) const override {
            const auto it = data.find(key);
            if (it == data.end()){
                throw std::logic_error("No serialization/deserialization function");
            }
            return it->second;
        }

        [[nodiscard]] bool has_function(const Key& key) const noexcept override {
            return data.contains(key);
        }

        void add_function(const Key& key, Func function) override {
            if (data.contains(key)) {
                throw std::logic_error("type already has serialization/deserialization function");
            }

            data.emplace(key, std::move(function));
        }

        void replace_function(const Key& key, Func function) override {
            const auto it = data.find(key);
            if (it == data.end()) {
                throw std::logic_error("No serialization/deserialization function");
            }
            it->second = std::move(function);
        }
    };
}
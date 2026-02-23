export module serde.core.runtime.mapper;

export import :serialization;
export import :deserialization;

import <unordered_map>;
import <stdexcept>;

export namespace serde {
    /**
     * @brief Maps types to functions.
     *
     * Each type is identified by a key.
     *
     * @tparam Func Function type.
     * @tparam Key Key type.
     */
    template <typename Func, typename Key>
    class SerDeFuncMapper final {
        std::unordered_map<Key, Func> data;

    public:

        /**
         * @brief Retrieve a function by key.
         *
         * @param key Identifier of the type.
         * @return Function.
         *
         * @throws std::logic_error if no function is registered for the given key.
         */
        [[nodiscard]] Func get_function(const Key& key) const {
            const auto it = data.find(key);
            if (it == data.end()){
                throw std::logic_error("No serialization/deserialization function");
            }
            return it->second;
        }

        /**
         * @brief Register a function for a type.
         *
         * @param key Identifier of the type.
         * @param function function.
         *
         * @throws std::logic_error if a function is already registered for the given key.
         */
        void add_function(const Key& key, Func function) {
            if (data.contains(key)) {
                throw std::logic_error("type already has serialization/deserialization function");
            }

            data.emplace(key, std::move(function));
        }

        /**
         * @brief Replaces a function for a type.
         *
         * @param key Identifier of the type.
         * @param function New function.
         *
         * @throws std::logic_error if a function is already registered for the given key.
         */
        void replace_function(const Key& key, Func function) {
            const auto it = data.find(key);
            if (it == data.end()) {
                throw std::logic_error("No serialization/deserialization function");
            }
            it->second = std::move(function);
        }
    };
}
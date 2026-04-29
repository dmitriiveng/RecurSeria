module;

export module serde.core.runtime.mapper:interface;

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
    class ISerDeFuncMapper {
    public:
        virtual ~ISerDeFuncMapper() = default;

        /**
         * @brief Retrieves a function by key.
         *
         * @param key Identifier of the type.
         * @return Function.
         *
         * @throws std::logic_error if no function is registered for the given key.
         */
        [[nodiscard]] virtual Func get_function(const Key& key) const = 0;

        /**
         * @brief Retrieves true if function for given key exists and false if not.
         *
         * @param key Identifier of the type.
         * @return bool.
         */
        [[nodiscard]] virtual bool has_function(const Key& key) const noexcept = 0;

        /**
         * @brief Registers a function for a type.
         *
         * @param key Identifier of the type.
         * @param function function.
         *
         * @throws std::logic_error if a function is already registered for the given key.
         */
        virtual void add_function(const Key& key, Func function) = 0;

        /**
         * @brief Replaces a function for a type.
         *
         * @param key Identifier of the type.
         * @param function New function.
         *
         * @throws std::logic_error if a function is already registered for the given key.
         */
        virtual void replace_function(const Key& key, Func function) = 0;
    };
}

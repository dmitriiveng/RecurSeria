module;
#include <string>

export module recurseria.core.runtime.codec;

export import recurseria.core.runtime.mapper;
export import recurseria.core.runtime.type_erasure.types;
export import recurseria.core.runtime.functions_generator;

export namespace recurseria::core::runtime {
    template <typename SerializedType, typename Key = std::string>
    class Codec {
    public:
        Codec(ISrlzFuncMapper<SerializedType, Key>& srlz_mapper, IDsrlzFuncMapper<SerializedType, Key>& dsrlz_mapper)
            : m_srlz_mapper(srlz_mapper), m_dsrlz_mapper(dsrlz_mapper) {}

        template <typename FormatTag, typename T>
        void register_serializable(const Key& key) {
            m_srlz_mapper.add_function(key, gen_simple_srlz_func<FormatTag, SerializedType, T>());
        }

        template <typename FormatTag, typename T>
        void register_deserializable(const Key& key) {
            m_dsrlz_mapper.add_function(key, gen_simple_dsrlz_func<FormatTag, T, SerializedType>());
        }

        template <typename FormatTag, typename T>
        void replace_serializable(const Key& key) {
            m_srlz_mapper.replace_function(key, gen_simple_srlz_func<FormatTag, SerializedType, T>());
        }

        template <typename FormatTag, typename T>
        void replace_deserializable(const Key& key) {
            m_dsrlz_mapper.replace_function(key, gen_simple_dsrlz_func<FormatTag, T, SerializedType>());
        }

        template <typename FormatTag, typename T>
        void register_type(const Key& key) {
            register_serializable<FormatTag, T>(key);
            register_deserializable<FormatTag, T>(key);
        }

        template <typename FormatTag, typename T>
        void replace_type(const Key& key) {
            replace_serializable<FormatTag, T>(key);
            replace_deserializable<FormatTag, T>(key);
        }

        [[nodiscard]] SerializedType serialize(const Key& key, const TypeErasedValuePtr data) const {
            return m_srlz_mapper.get_function(key)(data);
        }

        [[nodiscard]] TypeErasedValueOwner deserialize(const Key& key, const SerializedType& data) const {
            return m_dsrlz_mapper.get_function(key)(data);
        }

        [[nodiscard]] bool has_serializable(const Key& key) const noexcept {
            return m_srlz_mapper.has_function(key);
        }

        [[nodiscard]] bool has_deserializable(const Key& key) const noexcept {
            return m_dsrlz_mapper.has_function(key);
        }

    private:
        ISrlzFuncMapper<SerializedType, Key>& m_srlz_mapper;
        IDsrlzFuncMapper<SerializedType, Key>& m_dsrlz_mapper;
    };
}

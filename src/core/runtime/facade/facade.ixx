export module recurseria.core.runtime.facade;

export import recurseria.core.runtime.mapper;
export import recurseria.core.runtime.type_erasure.types;
export import recurseria.core.runtime.functions_generator;

import recurseria.core.meta;

export namespace recurseria::core::runtime {
    template <typename Output, typename Key>
    Output serialize(ISrlzFuncMapper<Output, Key>& mapper, const Key& key, const TypeErasedValuePtr data){
        return mapper.get_function(key)(data);
    }

    template <typename Input, typename Key>
    TypeErasedValueOwner deserialize(IDsrlzFuncMapper<Input, Key>& mapper, const Key& key, const Input& data){
        return mapper.get_function(key)(data);
    }

    template <recurseria::core::meta::SerializeArgPack Args, typename Key>
    void register_serializable(ISrlzFuncMapper<typename recurseria::core::meta::serialize_args<Args>::output_type, Key>& mapper, const Key& key) {
        mapper.add_function(key, gen_simple_srlz_func<Args>());
    }

    template <recurseria::core::meta::DeserializeArgPack Args, typename Key>
    void register_deserializable(IDsrlzFuncMapper<typename recurseria::core::meta::deserialize_args<Args>::input_type, Key>& mapper, const Key& key) {
        mapper.add_function(key, gen_simple_dsrlz_func<Args>());
    }

    template <recurseria::core::meta::SerializeArgPack Args, typename Key>
    void replace_serializable(ISrlzFuncMapper<typename recurseria::core::meta::serialize_args<Args>::output_type, Key>& mapper, const Key& key) {
        mapper.replace_function(key, gen_simple_srlz_func<Args>());
    }

    template <recurseria::core::meta::DeserializeArgPack Args, typename Key>
    void replace_deserializable(IDsrlzFuncMapper<typename recurseria::core::meta::deserialize_args<Args>::input_type, Key>& mapper, const Key& key) {
        mapper.replace_function(key, gen_simple_dsrlz_func<Args>());
    }

    template <
        recurseria::core::meta::SerializeArgPack SrlzArgs,
        recurseria::core::meta::DeserializeArgPack DsrlzArgs,
        typename Key
    >
    void register_type(
        ISrlzFuncMapper<typename recurseria::core::meta::serialize_args<SrlzArgs>::output_type, Key>& srlz_mapper,
        IDsrlzFuncMapper<typename recurseria::core::meta::deserialize_args<DsrlzArgs>::input_type, Key>& dsrlz_mapper,
        const Key& key
    ) {
        register_serializable<SrlzArgs>(srlz_mapper, key);
        register_deserializable<DsrlzArgs>(dsrlz_mapper, key);
    }

    template <typename Output, typename Key>
    [[nodiscard]] bool has_serializable(const ISrlzFuncMapper<Output, Key>& mapper, const Key& key) noexcept {
        return mapper.has_function(key);
    }

    template <typename Input, typename Key>
    [[nodiscard]] bool has_deserializable(const IDsrlzFuncMapper<Input, Key>& mapper, const Key& key) noexcept {
        return mapper.has_function(key);
    }
}

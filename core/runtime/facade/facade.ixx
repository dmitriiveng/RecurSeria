module;

export module serde.core.runtime.facade;

export import serde.core.runtime.mapper;

export import serde.core.runtime.type_erasure.types;

export namespace serde {
    // serde

    template <typename Output, typename Key>
    Output serialize(ISrlzFuncMapper<Output, Key>& mapper, Key& key, const TypeErasedValuePtr data){
        return mapper.get_function(key)(data);
    }

    template <typename Input, typename Key>
    TypeErasedValueOwner deserialize(IDsrlzFuncMapper<Input, Key>& mapper, Key& key, const Input& data){
        return mapper.get_function(key)(data);
    }

    // new types

    template <typename T, typename Output, typename Key, typename SrlzGenFunc>
    void register_serializable_type(IDsrlzFuncMapper<Output, Key>& mapper, Key& key) {
        mapper.add_function(key, gen_simple_type_srlz_func<Output, T>());
    }

    template <typename T, typename Input, typename Key, typename SrlzGenFunc>
    void register_deserializable_type(IDsrlzFuncMapper<Input, Key>& mapper, Key& key) {
        mapper.add_function(key, gen_simple_type_dsrlz_func<Input, T>());
    }
}

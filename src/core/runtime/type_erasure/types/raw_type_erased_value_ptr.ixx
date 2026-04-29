module;

export module serde.core.runtime.type_erasure.types:raw_type_erased_value_ptr;

export namespace serde {

    class RawTypeErasedValuePtr {
        void* ptr;
    public:
    template<typename U>
        RawTypeErasedValuePtr(U& object)
            : ptr(&object) {}

        template<typename U>
        U& get() const {
            return *static_cast<U*>(ptr);
        }
    };
}
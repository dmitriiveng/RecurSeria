export module serde.core.runtime.type_erasure.types;

export import :raw_type_erased_value_ptr;
export import :unique_ptr_type_erased_value_owner;
export import :type_erased_value_concepts;

export namespace serde{

    using TypeErasedValuePtr = RawTypeErasedValuePtr;

    using TypeErasedValueOwner = UniquePtrTypeErasedValueOwner;

    static_assert(TypeErasedPtr<TypeErasedValuePtr>);
    static_assert(TypeErasedOwner<TypeErasedValueOwner>);
}

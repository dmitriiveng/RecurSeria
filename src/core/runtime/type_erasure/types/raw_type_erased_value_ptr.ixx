export module recurseria.core.runtime.type_erasure.types:raw_type_erased_value_ptr;

import std;

export namespace recurseria::core::runtime {
    class RawTypeErasedValuePtr {
        void* ptr;
    public:
        template<typename U>
            requires (!std::same_as<U, RawTypeErasedValuePtr>) 
            // needed because this template constructor should not be a copy constructor
        RawTypeErasedValuePtr(U& object)
            : ptr(&object) {}

        template<typename U>
        U& get() const {
            return *static_cast<U*>(ptr);
        }
    };
}

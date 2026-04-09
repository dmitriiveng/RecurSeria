module;

#include <memory>

export module serde.core.runtime.type_erasure.types:unique_ptr_type_erased_value_owner;

export namespace serde {

    class UniquePtrTypeErasedValueOwner {
        std::unique_ptr<void, void(*)(void*)> value;
    public:
        template<typename U>
        UniquePtrTypeErasedValueOwner(U&& object)
            : value(
                new std::remove_cvref_t<U>(std::forward<U>(object)),
                [](void* ptr) { delete static_cast<std::remove_cvref_t<U>*>(ptr); }
            )
            {}

        template<typename U>
        U* get() const {
            return static_cast<U*>(value.get());
        }

        template<typename U>
        U cast_and_release() {
            U* ptr = get<U>();
            U result = std::move(*ptr);
            value.reset();
            return result;
        }
    };
}

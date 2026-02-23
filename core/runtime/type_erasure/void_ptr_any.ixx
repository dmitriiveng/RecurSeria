export module serde.core.runtime.type_erasure.void_ptr_any;

import <utility>;
import <memory>;


export namespace serde {
    class VoidPtrAny final {
        std::unique_ptr<void, void(*)(void*)> data;

    public:
        template <typename T>
        explicit VoidPtrAny(T&& value) :
            data(
                new std::decay_t<T>(std::forward<T>(value)),
                [](void* ptr){ delete static_cast<std::decay_t<T>*>(ptr); }
                ) {}

        template <typename T>
        T get_value() {
            return *static_cast<T*>(data.get());
        }

        template <typename T>
        const T& get_value() const {
            return *static_cast<const T*>(data.get());
        }

        VoidPtrAny(VoidPtrAny&&) noexcept = default;
        VoidPtrAny& operator=(VoidPtrAny&&) noexcept = default;

        VoidPtrAny(const VoidPtrAny&) = delete;
        VoidPtrAny& operator=(const VoidPtrAny&) = delete;
    };
}
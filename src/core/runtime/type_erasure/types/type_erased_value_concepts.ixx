module;
#include <concepts>

export module recurseria.core.runtime.type_erasure.types:type_erased_value_concepts;

namespace recurseria::core::runtime {
    // temporary solution
    // TODO make that without probe_type

    struct probe_type {};

    template<typename T>
    concept TypeErasedOwner =
        requires(T t) {
            {
                t.template get<probe_type>()
            } -> std::same_as<probe_type*>;

            {
                t.template cast_and_release<probe_type>()
            } -> std::same_as<probe_type>;
        };

    template<typename T>
    concept TypeErasedPtr =
        requires(T t) {
            {
                t.template get<probe_type>()
            } -> std::same_as<probe_type&>;
        };
}

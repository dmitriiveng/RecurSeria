export module recurseria.core.meta.helpers.associative_ops:associative_ops_supported_concept;

import std;
import :groupable;
import :decomposable;

export namespace recurseria::core::meta {
    template <typename FormatTag, typename T>
    concept AssociativeOpsSupported =
        AssociativelyGroupable<FormatTag, std::map<T, T>> &&
        AssociativelyDecomposable<FormatTag, T>;
}

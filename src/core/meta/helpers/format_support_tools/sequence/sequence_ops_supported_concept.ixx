export module recurseria.core.meta.helpers.sequence_ops:sequence_ops_supported_concept;

import std;
import :groupable;
import :decomposable;

export namespace recurseria {
    template <typename FormatTag, typename T>
    concept SequenceOpsSupported =
        SequentiallyGroupable<FormatTag, std::vector<T>> &&
        SequentiallyDecomposable<FormatTag, T>;
}

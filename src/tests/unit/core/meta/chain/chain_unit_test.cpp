#include <gtest/gtest.h>
import std;

import recurseria.core.meta.chain;

using namespace recurseria::core::meta;

namespace {

    TEST(ChainTest, ChainIsEmpty) {
        static_assert(std::is_empty_v<chain<int>>);
        static_assert(std::is_empty_v<chain<int, double, std::string>>);
    }

    TEST(ChainTest, ChainIsTrivial) {
        static_assert(std::is_trivial_v<chain<int>>);
    }

    TEST(ChainTest, IsChainAcceptsNonEmptyChain) {
        static_assert(IsChain<chain<int>>);
        static_assert(IsChain<chain<int, double>>);
        static_assert(IsChain<chain<int, double, std::string>>);
    }

    TEST(ChainTest, IsChainRejectsPlainTypes) {
        static_assert(!IsChain<int>);
        static_assert(!IsChain<std::string>);
        static_assert(!IsChain<void>);
    }

    // chain_reverse

    TEST(ChainTest, ReverseSingleType) {
        static_assert(std::same_as<chain_reverse_t<chain<int>>, chain<int>>);
    }

    TEST(ChainTest, ReverseTwoTypes) {
        static_assert(std::same_as<chain_reverse_t<chain<int, double>>, chain<double, int>>);
    }

    TEST(ChainTest, ReverseThreeTypes) {
        static_assert(std::same_as<chain_reverse_t<chain<int, double, std::string>>, chain<std::string, double, int>>);
    }

    TEST(ChainTest, ReverseIsSelfInverse) {
        using Original = chain<int, std::string, double>;
        using Reversed = chain_reverse_t<Original>;
        using Back    = chain_reverse_t<Reversed>;
        static_assert(std::same_as<Back, Original>);
    }

}

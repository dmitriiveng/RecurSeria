#include <string>
#include <type_traits>
#include <gtest/gtest.h>

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

    TEST(ChainTest, FoldLeftSingleType) {
        auto result = fold_left(
            []<typename T>(int x) -> int { return x + 1; },
            42,
            chain<int>{}
        );
        EXPECT_EQ(result, 43);
    }

    TEST(ChainTest, FoldLeftSingleTypeReturnsCorrectType) {
        auto result = fold_left(
            []<typename T>(int x) -> std::string { return std::to_string(x); },
            42,
            chain<std::string>{}
        );
        static_assert(std::same_as<decltype(result), std::string>);
        EXPECT_EQ(result, "42");
    }

    TEST(ChainTest, FoldLeftAppliesTypesLeftToRight) {
        auto result = fold_left(
            []<typename T>(std::string s) -> std::string {
                if constexpr (std::same_as<T, int>)          return s + "[int]";
                else if constexpr (std::same_as<T, double>)   return s + "[double]";
                else if constexpr (std::same_as<T, std::string>) return s + "[string]";
                else return s;
            },
            std::string{},
            chain<int, double, std::string>{}
        );
        EXPECT_EQ(result, "[int][double][string]");
    }

    namespace {

        struct FoldStep {
            template<typename T>
            auto operator()(const auto& v) const {
                if constexpr (std::same_as<T, std::string>)
                    return std::to_string(v);
                else
                    return static_cast<int>(v.size());
            }
        };

    }

    TEST(ChainTest, FoldLeftChangesTypeThroughChain) {
        auto result = fold_left(FoldStep{}, 42, chain<std::string, int>{});
        static_assert(std::same_as<decltype(result), int>);
        EXPECT_EQ(result, 2);
    }

    TEST(ChainTest, FoldLeftOrderDependent) {
        // chain<A, B> applies A then B — reversing produces different result
        auto step = []<typename T>(std::string s) -> std::string {
            if constexpr (std::same_as<T, std::string>)
                return s + "a";
            else
                return s + "b";
        };

        auto ab = fold_left(step, std::string{}, chain<std::string, int>{});
        auto ba = fold_left(step, std::string{}, chain<int, std::string>{});

        EXPECT_EQ(ab, "ab");
        EXPECT_EQ(ba, "ba");
    }

    TEST(ChainTest, FoldLeftIdentityPassThrough) {
        auto result = fold_left(
            []<typename T>(auto&& v) -> decltype(auto) {
                return std::forward<decltype(v)>(v);
            },
            42,
            chain<int, int, int>{}
        );
        EXPECT_EQ(result, 42);
    }

}

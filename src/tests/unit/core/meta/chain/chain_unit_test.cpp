#include <gtest/gtest.h>
import std;

import recurseria.core.meta.chain;

using namespace recurseria::core::meta;

namespace {

    struct FmtA {};
    struct FmtB {};

}

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

    // fold_left, single type

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

    // -- type_format --------------------------------------------------------

    TEST(TypeFormatTest, StoresTypeAndFormat) {
        static_assert(std::same_as<type_format<int, FmtA>::type, int>);
        static_assert(std::same_as<type_format<int, FmtA>::format, FmtA>);
    }

    TEST(TypeFormatTest, IsTypeFormatDetectsTypeFormat) {
        static_assert(is_type_format_v<type_format<int, FmtA>>);
        static_assert(is_type_format_v<type_format<std::string, FmtB>>);
        static_assert(!is_type_format_v<int>);
        static_assert(!is_type_format_v<std::string>);
        static_assert(!is_type_format_v<FmtA>);
    }

    TEST(TypeFormatTest, TypeFormatChainIsStillChain) {
        static_assert(IsChain<chain<type_format<int, FmtA>>>);
        static_assert(IsChain<chain<type_format<int, FmtA>, type_format<double, FmtB>>>);
    }

    TEST(TypeFormatTest, ChainReverseWithTypeFormat) {
        using Original = chain<type_format<int, FmtA>, type_format<double, FmtB>>;
        using Reversed = chain_reverse_t<Original>;
        static_assert(std::same_as<Reversed, chain<type_format<double, FmtB>, type_format<int, FmtA>>>);
    }

    TEST(TypeFormatTest, FoldLeftSingleTypeFormat) {
        auto result = fold_left(
            []<typename T, typename Fmt>(int x) -> int {
                if constexpr (std::same_as<Fmt, FmtA>) return x + 10;
                else return x;
            },
            5,
            chain<type_format<int, FmtA>>{}
        );
        EXPECT_EQ(result, 15);
    }

    TEST(TypeFormatTest, FoldLeftMultipleTypeFormats) {
        auto result = fold_left(
            []<typename T, typename Fmt>(std::string s) -> std::string {
                if constexpr (std::same_as<Fmt, FmtA>) return s + "[A]";
                else if constexpr (std::same_as<Fmt, FmtB>) return s + "[B]";
                else return s;
            },
            std::string{},
            chain<type_format<int, FmtA>, type_format<double, FmtB>>{}
        );
        EXPECT_EQ(result, "[A][B]");
    }

    TEST(TypeFormatTest, TypeFormatCanMixWithPlain) {
        auto result = fold_left(
            []<typename T, typename Fmt = void>(std::string s) -> std::string {
                if constexpr (std::same_as<Fmt, void>) {
                    return s + "[plain]";
                } else if constexpr (std::same_as<Fmt, FmtA>) {
                    return s + "[A]";
                } else {
                    return s;
                }
            },
            std::string{},
            chain<int, type_format<double, FmtA>>{}
        );
        EXPECT_EQ(result, "[plain][A]");
    }

}

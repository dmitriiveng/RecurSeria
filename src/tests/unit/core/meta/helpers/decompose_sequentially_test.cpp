#include <gtest/gtest.h>
import std;

import recurseria.core.meta.helpers.sequence_ops;

using namespace recurseria::core::meta;

namespace {

    struct Point {
        int x;
        int y;
    };

    struct PointFmt {};
    auto tag_invoke(PointFmt, decompose_sequentially_tag, const Point& p) {
        return std::views::single(p);
    }

    TEST(DecomposeSequentiallyTest, CustomPath) {
        Point p{1, 2};
        auto view = decompose_sequentially(PointFmt{}, p);
        auto it = std::ranges::begin(view);
        EXPECT_EQ(it->x, 1);
        EXPECT_EQ(it->y, 2);
    }

    // ---

    struct ThrowingFmt {};
    auto tag_invoke(ThrowingFmt, decompose_sequentially_tag, const int&)
        -> std::ranges::single_view<int>
    {
        throw std::runtime_error("custom decompose error");
    }

    TEST(DecomposeSequentiallyTest, CustomThrowsWrapsInTagInvokeError) {
        int x = 1;
        EXPECT_THROW(decompose_sequentially(ThrowingFmt{}, x), tag_invoke_error);
    }

    // ---

    struct NotDecomposable {};

    TEST(DecomposeSequentiallyTest, Concepts) {
        EXPECT_TRUE((SequentiallyDecomposable<PointFmt, Point>));
        EXPECT_FALSE((SequentiallyDecomposable<PointFmt, NotDecomposable>));
    }

}

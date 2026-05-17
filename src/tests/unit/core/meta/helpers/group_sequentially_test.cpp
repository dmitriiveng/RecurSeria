#include <ranges>
#include <stdexcept>
#include <vector>
#include <gtest/gtest.h>

import recurseria.core.meta.helpers.sequence_ops;

using namespace recurseria::core::meta;

namespace {

    struct Point {
        int x;
        int y;
    };

    struct PointFmt {};
    auto tag_invoke(PointFmt, group_sequentially_tag, std::ranges::input_range auto&& range) -> Point {
        return *std::ranges::begin(range);
    }

    TEST(GroupSequentiallyTest, CustomPath) {
        Point p{10, 20};
        auto single = std::views::single(p);
        auto result = group_sequentially(PointFmt{}, single);
        EXPECT_EQ(result.x, 10);
        EXPECT_EQ(result.y, 20);
    }

    // ---

    struct ThrowingFmt {};
    auto tag_invoke(ThrowingFmt, group_sequentially_tag, std::ranges::input_range auto&&) -> int {
        throw std::runtime_error("custom group error");
    }

    TEST(GroupSequentiallyTest, CustomThrowsWrapsInTagInvokeError) {
        std::vector<int> data{1};
        EXPECT_THROW(group_sequentially(ThrowingFmt{}, data), tag_invoke_error);
    }

    // ---

    struct NotGroupable {};

    TEST(GroupSequentiallyTest, Concepts) {
        Point p{1, 2};
        auto single = std::views::single(p);
        EXPECT_TRUE((SequentiallyGroupable<PointFmt, decltype(single)>));
        EXPECT_FALSE((SequentiallyGroupable<PointFmt, std::vector<NotGroupable>>));
    }

}

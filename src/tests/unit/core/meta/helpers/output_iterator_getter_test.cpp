#include <iterator>
#include <set>
#include <stdexcept>
#include <vector>
#include <gtest/gtest.h>

import recurseria.core.meta.helpers.output_iterator_getter;

using namespace recurseria::core::meta;

namespace {

    TEST(OutputIteratorGetterTest, VectorBackInserterDefaultPath) {
        std::vector<int> vec{1, 2, 3};
        auto it = get_output_iterator(vec);
        *it = 42;
        ASSERT_EQ(vec.size(), 4);
        EXPECT_EQ(vec.back(), 42);
    }

    TEST(OutputIteratorGetterTest, SetInserterDefaultPath) {
        std::set<int> s{10, 20};
        auto it = get_output_iterator(s);
        *it = 42;
        ASSERT_EQ(s.size(), 3);
        EXPECT_TRUE(s.contains(42));
    }

    struct CustomVec {
        std::vector<int> data;

        auto begin() { return data.begin(); }
        auto end() { return data.end(); }
    };

    static_assert(std::ranges::range<CustomVec>);

    auto tag_invoke(get_output_iterator_tag, CustomVec& cv) {
        return std::back_inserter(cv.data);
    }

    TEST(OutputIteratorGetterTest, CustomContainer) {
        CustomVec cv{{1, 2, 3}};
        auto it = get_output_iterator(cv);
        *it = 99;
        ASSERT_EQ(cv.data.size(), 4);
        EXPECT_EQ(cv.data.back(), 99);
    }

    struct ThrowingCustom {
        std::vector<int> data;

        auto begin() { return data.begin(); }
        auto end() { return data.end(); }
    };

    auto tag_invoke(get_output_iterator_tag, ThrowingCustom&) -> std::back_insert_iterator<std::vector<int>> {
        throw std::runtime_error("custom iterator error");
    }

    TEST(OutputIteratorGetterTest, CustomThrowsWrapsInTagInvokeError) {
        ThrowingCustom tc{{1}};
        EXPECT_THROW(get_output_iterator(tc), tag_invoke_error);
    }

}

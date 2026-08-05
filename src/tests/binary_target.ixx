export module recurseria.tests.binary_target;

import std;
import boost_ut;
import recurseria;
import recurseria.binary;

namespace recurseria::tests::binary_target{
    struct Person {
        int id;
        double score;
        std::string name;
    };

    struct Address {
        std::string city;
        int zip;
    };

    struct Employee {
        std::string name;
        Address address;
        std::vector<int> tags;
    };

    using recurseria::srlz_dsrlz_round_trip;
    using recurseria::srlz_dsrlz_round_trip_validation;
    using recurseria::group_sequentially;
    using recurseria::decompose_sequentially;
    using recurseria::serialize;
    using recurseria::deserialize;

    using recurseria::binary::binary_format_tag;
    using BinTag = binary_format_tag;
    using BinOut = std::vector<std::byte>;

    template <typename T>
    T rt_round_trip(T value) {
        return srlz_dsrlz_round_trip<BinTag, T, BinOut>(value);
    }
}

export namespace recurseria::tests::binary_target{
    void test_primitives(){
        "binary scalar int round-trip"_test = [] {
            int original = 42;
            auto result = srlz_dsrlz_round_trip<BinTag, int, BinOut>(original);
            expect(result == 42);
        };

        "binary int round-trips 0, 1, -1"_test = [] {
            expect(rt_round_trip(0) == 0);
            expect(rt_round_trip(1) == 1);
            expect(rt_round_trip(-1) == -1);
        };

        "binary int boundary round-trips"_test = [] {
            expect(rt_round_trip(std::numeric_limits<int>::max()) == std::numeric_limits<int>::max());
            expect(rt_round_trip(std::numeric_limits<int>::min()) == std::numeric_limits<int>::min());
        };

        "binary integer round-trips"_test = [] {
            long l = 123456789L;
            expect(srlz_dsrlz_round_trip<BinTag, long, BinOut>(l) == l);
            long long ll = 9876543210LL;
            expect(srlz_dsrlz_round_trip<BinTag, long long, BinOut>(ll) == ll);
            unsigned long ul = 1234567890UL;
            expect(srlz_dsrlz_round_trip<BinTag, unsigned long, BinOut>(ul) == ul);
            unsigned long long ull = 18446744073709551615ULL;
            expect(srlz_dsrlz_round_trip<BinTag, unsigned long long, BinOut>(ull) == ull);
        };

        "binary integer boundary round-trips"_test = [] {
            expect(rt_round_trip(std::numeric_limits<long>::max()) == std::numeric_limits<long>::max());
            expect(rt_round_trip(std::numeric_limits<long>::min()) == std::numeric_limits<long>::min());
            expect(rt_round_trip(std::numeric_limits<long long>::max()) == std::numeric_limits<long long>::max());
            expect(rt_round_trip(std::numeric_limits<long long>::min()) == std::numeric_limits<long long>::min());
        };

        "binary unsigned boundary round-trips"_test = [] {
            expect(rt_round_trip(0u) == 0u);
            expect(rt_round_trip(std::numeric_limits<unsigned int>::max()) == std::numeric_limits<unsigned int>::max());
            expect(rt_round_trip(std::numeric_limits<unsigned long>::max()) == std::numeric_limits<unsigned long>::max());
            expect(rt_round_trip(std::numeric_limits<unsigned long long>::max()) == std::numeric_limits<unsigned long long>::max());
        };

        "binary integer canonical widths"_test = [] {
            expect(serialize.as<BinTag, BinOut>(static_cast<int>(1)).size() == 4u);
            expect(serialize.as<BinTag, BinOut>(static_cast<long>(1)).size() == 8u);
            expect(serialize.as<BinTag, BinOut>(static_cast<long long>(1)).size() == 8u);
            expect(serialize.as<BinTag, BinOut>(static_cast<unsigned int>(1)).size() == 4u);
            expect(serialize.as<BinTag, BinOut>(static_cast<unsigned long>(1)).size() == 8u);
            expect(serialize.as<BinTag, BinOut>(static_cast<unsigned long long>(1)).size() == 8u);
        };

        "binary int wire bytes"_test = [] {
            expect(std::ranges::equal(serialize.as<BinTag, BinOut>(1),
                std::vector<std::byte>{std::byte{1}, std::byte{0}, std::byte{0}, std::byte{0}}));
            expect(std::ranges::equal(serialize.as<BinTag, BinOut>(0),
                std::vector<std::byte>{std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0}}));
            expect(std::ranges::equal(serialize.as<BinTag, BinOut>(-1),
                std::vector<std::byte>{std::byte{0xFF}, std::byte{0xFF}, std::byte{0xFF}, std::byte{0xFF}}));
            expect(std::ranges::equal(serialize.as<BinTag, BinOut>(std::numeric_limits<int>::max()),
                std::vector<std::byte>{std::byte{0xFF}, std::byte{0xFF}, std::byte{0xFF}, std::byte{0x7F}}));
            expect(std::ranges::equal(serialize.as<BinTag, BinOut>(std::numeric_limits<int>::min()),
                std::vector<std::byte>{std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0x80}}));
        };

        "binary long canonical 8-byte wire"_test = [] {
            expect(std::ranges::equal(serialize.as<BinTag, BinOut>(1L),
                std::vector<std::byte>{std::byte{1}, std::byte{0}, std::byte{0}, std::byte{0},
                                       std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0}}));
        };

        "binary scalar double round-trip"_test = [] {
            double original = 3.14;
            auto result = srlz_dsrlz_round_trip<BinTag, double, BinOut>(original);
            expect(std::abs(result - 3.14) < 1e-10);
        };

        "binary double round-trips"_test = [] {
            expect(rt_round_trip(0.0) == 0.0);
            expect(rt_round_trip(1.0) == 1.0);
            expect(rt_round_trip(-1.5) == -1.5);
            expect(rt_round_trip(2.718281828459045) == 2.718281828459045);
        };

        "binary double edge round-trips"_test = [] {
            expect(rt_round_trip(std::numeric_limits<double>::max()) == std::numeric_limits<double>::max());
            expect(rt_round_trip(std::numeric_limits<double>::min()) == std::numeric_limits<double>::min());
            expect(rt_round_trip(std::numeric_limits<double>::denorm_min()) == std::numeric_limits<double>::denorm_min());
        };

        "binary double special values round-trip"_test = [] {
            expect(rt_round_trip(std::numeric_limits<double>::infinity()) == std::numeric_limits<double>::infinity());
            expect(rt_round_trip(-std::numeric_limits<double>::infinity()) == -std::numeric_limits<double>::infinity());
            double neg_zero = -0.0;
            expect(std::signbit(rt_round_trip(neg_zero)));
            double nan = std::numeric_limits<double>::quiet_NaN();
            expect(std::isnan(rt_round_trip(nan)));
        };

        "binary double wire bytes"_test = [] {
            expect(std::ranges::equal(serialize.as<BinTag, BinOut>(1.0),
                std::vector<std::byte>{std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0},
                                       std::byte{0}, std::byte{0}, std::byte{0xF0}, std::byte{0x3F}}));
            expect(std::ranges::equal(serialize.as<BinTag, BinOut>(-1.0),
                std::vector<std::byte>{std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0},
                                       std::byte{0}, std::byte{0}, std::byte{0xF0}, std::byte{0xBF}}));
            expect(std::ranges::equal(serialize.as<BinTag, BinOut>(0.0),
                std::vector<std::byte>(8, std::byte{0})));
        };

        "binary float round-trips"_test = [] {
            expect(rt_round_trip(1.5f) == 1.5f);
            expect(rt_round_trip(-1.5f) == -1.5f);
            expect(rt_round_trip(0.0f) == 0.0f);
            expect(rt_round_trip(std::numeric_limits<float>::max()) == std::numeric_limits<float>::max());
            expect(rt_round_trip(std::numeric_limits<float>::min()) == std::numeric_limits<float>::min());
            expect(rt_round_trip(std::numeric_limits<float>::denorm_min()) == std::numeric_limits<float>::denorm_min());
        };

        "binary float special values round-trip"_test = [] {
            expect(rt_round_trip(std::numeric_limits<float>::infinity()) == std::numeric_limits<float>::infinity());
            expect(rt_round_trip(-std::numeric_limits<float>::infinity()) == -std::numeric_limits<float>::infinity());
            float neg_zero = -0.0f;
            expect(std::signbit(rt_round_trip(neg_zero)));
            float nan = std::numeric_limits<float>::quiet_NaN();
            expect(std::isnan(rt_round_trip(nan)));
        };

        "binary float wire bytes"_test = [] {
            expect(std::ranges::equal(serialize.as<BinTag, BinOut>(1.0f),
                std::vector<std::byte>{std::byte{0}, std::byte{0}, std::byte{0x80}, std::byte{0x3F}}));
            expect(std::ranges::equal(serialize.as<BinTag, BinOut>(-1.0f),
                std::vector<std::byte>{std::byte{0}, std::byte{0}, std::byte{0x80}, std::byte{0xBF}}));
        };

        "binary scalar long double round-trips via double"_test = [] {
            long double original = 3.14159265358979323846264338327950288L;
            long double expected = static_cast<long double>(static_cast<double>(original));
            auto result = srlz_dsrlz_round_trip<BinTag, long double, BinOut>(original);
            expect(result == expected);
        };

        "binary long double is downgraded to double"_test = [] {
            long double original = 1.0L + std::ldexp(1.0L, -60);
            auto result = srlz_dsrlz_round_trip<BinTag, long double, BinOut>(original);
            expect(result == static_cast<long double>(static_cast<double>(original)));
            expect(original != result);
        };

        "binary long double edge values round-trip"_test = [] {
            auto expect_rt = [](long double v) {
                long double expected = static_cast<long double>(static_cast<double>(v));
                auto result = srlz_dsrlz_round_trip<BinTag, long double, BinOut>(v);
                expect(result == expected);
            };
            expect_rt(static_cast<long double>(std::numeric_limits<double>::max()));
            expect_rt(static_cast<long double>(std::numeric_limits<double>::min()));
            expect_rt(-3.14159265358979323846264338327950288L);
            expect_rt(std::numeric_limits<long double>::infinity());
            expect_rt(-std::numeric_limits<long double>::infinity());
            long double neg_zero = -0.0L;
            expect(std::signbit(srlz_dsrlz_round_trip<BinTag, long double, BinOut>(neg_zero)));
            long double nan = std::numeric_limits<long double>::quiet_NaN();
            expect(std::isnan(srlz_dsrlz_round_trip<BinTag, long double, BinOut>(nan)));
        };

        "binary long double is canonical 8-byte double"_test = [] {
            auto s = serialize.as<BinTag, BinOut>(1.0L);
            expect(s.size() == 8u);
            std::vector<std::byte> expected{
                std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0},
                std::byte{0}, std::byte{0}, std::byte{0xF0}, std::byte{0x3F},
            };
            expect(std::ranges::equal(s, expected));
        };

        "binary scalar bool round-trip"_test = [] {
            bool original = true;
            auto result = srlz_dsrlz_round_trip<BinTag, bool, BinOut>(original);
            expect(result == true);
            bool f = false;
            expect(srlz_dsrlz_round_trip<BinTag, bool, BinOut>(f) == false);
        };

        "binary bool wire bytes"_test = [] {
            expect(std::ranges::equal(serialize.as<BinTag, BinOut>(true), std::vector<std::byte>{std::byte{1}}));
            expect(std::ranges::equal(serialize.as<BinTag, BinOut>(false), std::vector<std::byte>{std::byte{0}}));
        };

        "binary char round-trips"_test = [] {
            expect(rt_round_trip('A') == 'A');
            expect(rt_round_trip('z') == 'z');
            expect(rt_round_trip('~') == '~');
            expect(rt_round_trip('\0') == '\0');
        };

        "binary char wire bytes"_test = [] {
            expect(std::ranges::equal(serialize.as<BinTag, BinOut>('A'), std::vector<std::byte>{std::byte{0x41}}));
            expect(std::ranges::equal(serialize.as<BinTag, BinOut>('\0'), std::vector<std::byte>{std::byte{0}}));
        };

        "binary scalar string round-trips"_test = [] {
            expect(rt_round_trip(std::string("")) == "");
            expect(rt_round_trip(std::string("a")) == "a");
            expect(rt_round_trip(std::string("hello world")) == "hello world");
            expect(rt_round_trip(std::string("héllo wörld")) == "héllo wörld");
        };

        "binary string with special characters"_test = [] {
            std::string padded = "  leading and trailing  ";
            expect(rt_round_trip(padded) == padded);
            std::string multiline = "line1\nline2\r\n";
            expect(rt_round_trip(multiline) == multiline);
            std::string embedded_nul("a\0b", 3);
            expect(rt_round_trip(embedded_nul) == embedded_nul);
        };

        "binary long string round-trip"_test = [] {
            std::string long_str(4096, 'x');
            long_str[100] = '\xE9';
            expect(rt_round_trip(long_str) == long_str);
        };

        "binary string wire format"_test = [] {
            expect(std::ranges::equal(serialize.as<BinTag, BinOut>(std::string("")),
                std::vector<std::byte>(8, std::byte{0})));

            auto ab = serialize.as<BinTag, BinOut>(std::string("ab"));
            std::vector<std::byte> expected{
                std::byte{2}, std::byte{0}, std::byte{0}, std::byte{0},
                std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0},
                std::byte{0x61}, std::byte{0x62},
            };
            expect(std::ranges::equal(ab, expected));
        };
    }

    void test_reflection(){
        "binary aggregate Person round-trip"_test = [] {
            Person original{42, 3.14, "hello"};
            auto result = srlz_dsrlz_round_trip<BinTag, Person, BinOut>(original);
            expect(result.id == 42);
            expect(result.name == "hello");
        };

        "binary aggregate Person edge values"_test = [] {
            Person original{std::numeric_limits<int>::min(), -std::numeric_limits<double>::max(), ""};
            auto result = srlz_dsrlz_round_trip<BinTag, Person, BinOut>(original);
            expect(result.id == std::numeric_limits<int>::min());
            expect(result.score == -std::numeric_limits<double>::max());
            expect(result.name == "");
        };

        "binary nested aggregate round-trip"_test = [] {
            Employee original{"alice", {"moscow", 101000}, {1, 2, 3}};
            auto result = srlz_dsrlz_round_trip<BinTag, Employee, BinOut>(original);
            expect(result.name == "alice");
            expect(result.address.city == "moscow");
            expect(result.address.zip == 101000);
            expect(result.tags == std::vector<int>({1, 2, 3}));
        };
    }

    void test_containers(){
        "binary scalar string round-trip"_test = [] {
            std::string original = "hello";
            auto result = srlz_dsrlz_round_trip<BinTag, std::string, BinOut>(original);
            expect(result == "hello");
        };

        "binary vector<int> round-trip"_test = [] {
            std::vector<int> original{1, 2, 3, 4, 5};
            auto result = srlz_dsrlz_round_trip<BinTag, std::vector<int>, BinOut>(original);
            expect(result.size() == 5u);
            expect(result[0] == 1);
            expect(result[4] == 5);
        };

        "binary empty vector round-trip"_test = [] {
            std::vector<int> empty;
            auto result = srlz_dsrlz_round_trip<BinTag, std::vector<int>, BinOut>(empty);
            expect(result.empty());
            expect(serialize.as<BinTag, BinOut>(empty).empty());
        };

        "binary vector<int> edge values"_test = [] {
            std::vector<int> v{std::numeric_limits<int>::min(), -1, 0, 1, std::numeric_limits<int>::max()};
            auto result = srlz_dsrlz_round_trip<BinTag, std::vector<int>, BinOut>(v);
            expect(result == v);
        };

        "binary vector<double> round-trip"_test = [] {
            std::vector<double> v{0.0, -0.0, 3.14, -2.5, std::numeric_limits<double>::max()};
            auto result = srlz_dsrlz_round_trip<BinTag, std::vector<double>, BinOut>(v);
            expect(result.size() == v.size());
            expect(result[0] == 0.0);
            expect(std::signbit(result[1]));
            expect(result[2] == 3.14);
            expect(result[3] == -2.5);
            expect(result[4] == std::numeric_limits<double>::max());
        };

        "binary vector<string> round-trip"_test = [] {
            std::vector<std::string> v{"", "a", "hello", "héllo", "with spaces"};
            auto result = srlz_dsrlz_round_trip<BinTag, std::vector<std::string>, BinOut>(v);
            expect(result == v);
        };

        "binary nested vector<vector<int>> round-trip"_test = [] {
            std::vector<std::vector<int>> v{{}, {1}, {1, 2, 3}, {std::numeric_limits<int>::min()}};
            auto result = srlz_dsrlz_round_trip<BinTag, std::vector<std::vector<int>>, BinOut>(v);
            expect(result.size() == v.size());
            expect(result[0].empty());
            expect(result[1] == std::vector<int>({1}));
            expect(result[2] == std::vector<int>({1, 2, 3}));
            expect(result[3] == std::vector<int>({std::numeric_limits<int>::min()}));
        };

        "binary map<string,int> round-trip"_test = [] {
            std::map<std::string, int> original{{"a", 1}, {"b", 2}};
            auto result = srlz_dsrlz_round_trip<BinTag, std::map<std::string, int>, BinOut>(original);
            expect(result.at("a") == 1);
            expect(result.at("b") == 2);
        };

        "binary map<int,string> round-trip"_test = [] {
            std::map<int, std::string> original{{1, "one"}, {2, "two"}, {std::numeric_limits<int>::min(), "min"}};
            auto result = srlz_dsrlz_round_trip<BinTag, std::map<int, std::string>, BinOut>(original);
            expect(result.at(1) == "one");
            expect(result.at(2) == "two");
            expect(result.at(std::numeric_limits<int>::min()) == "min");
        };

        "binary empty map round-trip"_test = [] {
            std::map<std::string, int> empty;
            auto result = srlz_dsrlz_round_trip<BinTag, std::map<std::string, int>, BinOut>(empty);
            expect(result.empty());
        };
    }

    void test_tuple(){
        "binary tuple round-trip"_test = [] {
            using T = std::tuple<int, double, std::string>;
            T original{42, 3.14, "hi"};
            bool ok = srlz_dsrlz_round_trip_validation<BinTag, T, BinOut>(original);
            expect(ok);
        };

        "binary tuple edge values round-trip"_test = [] {
            using T = std::tuple<int, double, std::string>;
            T original{std::numeric_limits<int>::min(), std::numeric_limits<double>::max(), ""};
            auto result = srlz_dsrlz_round_trip<BinTag, T, BinOut>(original);
            expect(std::get<0>(result) == std::numeric_limits<int>::min());
            expect(std::get<1>(result) == std::numeric_limits<double>::max());
            expect(std::get<2>(result) == "");
        };

        "binary single-element tuple round-trip"_test = [] {
            using T = std::tuple<int>;
            T original{42};
            auto result = srlz_dsrlz_round_trip<BinTag, T, BinOut>(original);
            expect(std::get<0>(result) == 42);
        };

        "binary nested tuple round-trip"_test = [] {
            using T = std::tuple<std::tuple<int, int>, std::string>;
            T original{{1, 2}, "ab"};
            auto result = srlz_dsrlz_round_trip<BinTag, T, BinOut>(original);
            expect(std::get<0>(std::get<0>(result)) == 1);
            expect(std::get<1>(std::get<0>(result)) == 2);
            expect(std::get<1>(result) == "ab");
        };
    }

    void test_mixed(){
        "binary vector<Person> round-trip"_test = [] {
            std::vector<Person> original{{1, 1.0, "a"}, {2, 2.0, "b"}};
            auto result = srlz_dsrlz_round_trip<BinTag, std::vector<Person>, BinOut>(original);
            expect(result.size() == 2u);
            expect(result[0].id == 1);
            expect(result[1].name == "b");
        };

        "binary vector<Employee> round-trip"_test = [] {
            std::vector<Employee> v{{"a", {"x", 1}, {}}, {"b", {"y", 2}, {1, 2}}};
            auto result = srlz_dsrlz_round_trip<BinTag, std::vector<Employee>, BinOut>(v);
            expect(result.size() == 2u);
            expect(result[0].name == "a");
            expect(result[0].address.city == "x");
            expect(result[0].tags.empty());
            expect(result[1].address.zip == 2);
            expect(result[1].tags == std::vector<int>({1, 2}));
        };

        "binary map<string, vector<int>> round-trip"_test = [] {
            std::map<std::string, std::vector<int>> original{{"a", {1, 2}}, {"b", {}}};
            auto result = srlz_dsrlz_round_trip<BinTag, std::map<std::string, std::vector<int>>, BinOut>(original);
            expect(result.at("a") == std::vector<int>({1, 2}));
            expect(result.at("b").empty());
        };

        "binary vector<tuple<int,string>> round-trip"_test = [] {
            std::vector<std::tuple<int, std::string>> v{{1, "a"}, {2, "bb"}};
            auto result = srlz_dsrlz_round_trip<BinTag, std::vector<std::tuple<int, std::string>>, BinOut>(v);
            expect(result.size() == 2u);
            expect(std::get<0>(result[0]) == 1);
            expect(std::get<1>(result[1]) == "bb");
        };
    }

    void test_grouping_decomposing(){
        "binary group/decompose round-trip"_test = [] {
            auto s1 = serialize.as<BinTag, BinOut>(1);
            auto s2 = serialize.as<BinTag, BinOut>(2);
            std::vector<BinOut> input{s1, s2};

            auto grouped = group_sequentially(BinTag{}, input);
            expect(grouped.size() == 24u);

            auto decomposed = decompose_sequentially(BinTag{}, grouped);
            expect(decomposed.size() == 2u);
            expect(deserialize.as<BinTag, int>(decomposed[0]) == 1);
            expect(deserialize.as<BinTag, int>(decomposed[1]) == 2);
        };

        "binary group/decompose empty"_test = [] {
            std::vector<BinOut> empty;
            auto grouped = group_sequentially(BinTag{}, empty);
            expect(grouped.empty());

            auto decomposed = decompose_sequentially(BinTag{}, grouped);
            expect(decomposed.size() == 0u);
        };

        "binary group single element"_test = [] {
            auto s1 = serialize.as<BinTag, BinOut>(42);
            std::vector<BinOut> input{s1};

            auto grouped = group_sequentially(BinTag{}, input);
            expect(grouped.size() == 12u);

            auto decomposed = decompose_sequentially(BinTag{}, grouped);
            expect(decomposed.size() == 1u);
            expect(deserialize.as<BinTag, int>(decomposed[0]) == 42);
        };

        "binary group many elements"_test = [] {
            std::vector<BinOut> input;
            for (int i = 0; i < 100; ++i) {
                input.push_back(serialize.as<BinTag, BinOut>(i));
            }

            auto grouped = group_sequentially(BinTag{}, input);
            expect(grouped.size() == 100u * 12u);

            auto decomposed = decompose_sequentially(BinTag{}, grouped);
            expect(decomposed.size() == 100u);
            expect(deserialize.as<BinTag, int>(decomposed[0]) == 0);
            expect(deserialize.as<BinTag, int>(decomposed[99]) == 99);
        };

        "binary group/decompose mixed sizes"_test = [] {
            std::vector<BinOut> input{
                serialize.as<BinTag, BinOut>(42),
                serialize.as<BinTag, BinOut>(std::string("hello")),
                serialize.as<BinTag, BinOut>(true),
            };

            auto grouped = group_sequentially(BinTag{}, input);
            auto decomposed = decompose_sequentially(BinTag{}, grouped);
            expect(decomposed.size() == 3u);
            expect(deserialize.as<BinTag, int>(decomposed[0]) == 42);
            expect(deserialize.as<BinTag, std::string>(decomposed[1]) == "hello");
            expect(deserialize.as<BinTag, bool>(decomposed[2]) == true);
        };

        "binary zero-length frame decompose"_test = [] {
            std::vector<std::byte> input(8, std::byte{0});
            auto decomposed = decompose_sequentially(BinTag{}, input);
            expect(decomposed.size() == 1u);
            expect(decomposed[0].empty());
            auto v = deserialize.as<BinTag, std::vector<int>>(decomposed[0]);
            expect(v.empty());
        };
    }

    void test_raw_bytes_le(){
        "binary raw bytes little-endian"_test = [] {
            auto s1 = serialize.as<BinTag, BinOut>(1);
            auto s2 = serialize.as<BinTag, BinOut>(2);
            std::vector<BinOut> input{s1, s2};

            auto grouped = group_sequentially(BinTag{}, input);

            std::vector<std::byte> expected{
                std::byte{4}, std::byte{0}, std::byte{0}, std::byte{0},
                std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0},
                std::byte{1}, std::byte{0}, std::byte{0}, std::byte{0},
                std::byte{4}, std::byte{0}, std::byte{0}, std::byte{0},
                std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0},
                std::byte{2}, std::byte{0}, std::byte{0}, std::byte{0},
            };
            expect(std::ranges::equal(grouped, expected));
        };

        "binary group single-frame raw bytes"_test = [] {
            auto s = serialize.as<BinTag, BinOut>(std::string("ab"));
            auto grouped = group_sequentially(BinTag{}, std::vector<BinOut>{s});
            expect(grouped.size() == 18u);

            std::vector<std::byte> expected{
                std::byte{10}, std::byte{0}, std::byte{0}, std::byte{0},
                std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0},
                std::byte{2}, std::byte{0}, std::byte{0}, std::byte{0},
                std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0},
                std::byte{0x61}, std::byte{0x62},
            };
            expect(std::ranges::equal(grouped, expected));
        };
    }

    void test_error_handling(){
        "binary deserialize empty input throws"_test = [] {
            expect(throws<std::exception>([&] {
                deserialize.as<BinTag, int>(std::vector<std::byte>{});
            }));
        };

        "binary deserialize truncated int throws"_test = [] {
            expect(throws<std::exception>([&] {
                deserialize.as<BinTag, int>(std::vector<std::byte>{std::byte{1}});
            }));
        };

        "binary deserialize truncated long throws"_test = [] {
            expect(throws<std::exception>([&] {
                deserialize.as<BinTag, long>(std::vector<std::byte>(4, std::byte{0}));
            }));
        };

        "binary deserialize truncated string throws"_test = [] {
            std::vector<std::byte> input{
                std::byte{100}, std::byte{0}, std::byte{0}, std::byte{0},
                std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0},
                std::byte{0x61}, std::byte{0x62},
            };
            expect(throws<std::exception>([&] {
                deserialize.as<BinTag, std::string>(input);
            }));
        };

        "binary decompose malformed framing throws"_test = [] {
            std::vector<std::byte> input{
                std::byte{100}, std::byte{0}, std::byte{0}, std::byte{0},
                std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0},
                std::byte{1}, std::byte{2}, std::byte{3}, std::byte{4},
            };
            expect(throws<std::exception>([&] {
                decompose_sequentially(BinTag{}, input);
            }));
        };
    }

    void test_binary(){
        test_primitives();
        test_reflection();
        test_containers();
        test_tuple();
        test_mixed();
        test_grouping_decomposing();
        test_raw_bytes_le();
        test_error_handling();
    }
}

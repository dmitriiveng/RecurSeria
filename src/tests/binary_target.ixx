export module recurseria.tests.binary_target;

import std;
import yaml_cpp;
import boost_ut;
import recurseria.core;
import recurseria.target.binary;
import recurseria.core.meta.types_srlz_dsrlz;

namespace recurseria::tests::binary_target{
    struct Person {
        int id;
        double score;
        std::string name;
    };

    using recurseria::core::meta::srlz_dsrlz_round_trip;
    using recurseria::core::meta::srlz_dsrlz_round_trip_validation;
    using recurseria::core::meta::group_sequentially;
    using recurseria::core::meta::decompose_sequentially;
    using recurseria::core::meta::serialize;
    using recurseria::core::meta::deserialize;

    using recurseria::target::binary::binary_format_tag;
    using BinTag = binary_format_tag;
    using BinOut = std::vector<std::byte>;
}

export namespace recurseria::tests::binary_target{
    void test_primitives(){
        "binary scalar int round-trip"_test = [] {
            int original = 42;
            auto result = srlz_dsrlz_round_trip<BinTag, int, BinOut>(original);
            expect(result == 42);
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

        "binary integer canonical widths"_test = [] {
            expect(serialize.as<BinTag, BinOut>(static_cast<int>(1)).size() == 4u);
            expect(serialize.as<BinTag, BinOut>(static_cast<long>(1)).size() == 8u);
            expect(serialize.as<BinTag, BinOut>(static_cast<long long>(1)).size() == 8u);
            expect(serialize.as<BinTag, BinOut>(static_cast<unsigned int>(1)).size() == 4u);
            expect(serialize.as<BinTag, BinOut>(static_cast<unsigned long>(1)).size() == 8u);
            expect(serialize.as<BinTag, BinOut>(static_cast<unsigned long long>(1)).size() == 8u);
        };

        "binary scalar double round-trip"_test = [] {
            double original = 3.14;
            auto result = srlz_dsrlz_round_trip<BinTag, double, BinOut>(original);
            expect(std::abs(result - 3.14) < 1e-10);
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
        };
    }

    void test_reflection(){
        "binary aggregate Person round-trip"_test = [] {
            Person original{42, 3.14, "hello"};
            auto result = srlz_dsrlz_round_trip<BinTag, Person, BinOut>(original);
            expect(result.id == 42);
            expect(result.name == "hello");
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

        "binary map<string,int> round-trip"_test = [] {
            std::map<std::string, int> original{{"a", 1}, {"b", 2}};
            auto result = srlz_dsrlz_round_trip<BinTag, std::map<std::string, int>, BinOut>(original);
            expect(result.at("a") == 1);
            expect(result.at("b") == 2);
        };
    }

    void test_tuple(){
        "binary tuple round-trip"_test = [] {
            using T = std::tuple<int, double, std::string>;
            T original{42, 3.14, "hi"};
            bool ok = srlz_dsrlz_round_trip_validation<BinTag, T, BinOut>(original);
            expect(ok);
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
    }

    void test_binary(){
        test_primitives();
        test_reflection();
        test_containers();
        test_tuple();
        test_mixed();
        test_grouping_decomposing();
        test_raw_bytes_le();
    }
}

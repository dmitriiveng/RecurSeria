// TODO: replace with the original cppm module from Boost.UT after GCC Bug 122785 is fixed
module;

#if __has_include(<unistd.h>) and __has_include(<sys/wait.h>)
#include <sys/wait.h>
#include <unistd.h>
#endif

#include <boost/ut.hpp>

export module boost_ut;

export using boost::ut::operator""_test;
export using boost::ut::expect;
export using boost::ut::throws;
export using boost::ut::nothrow;

// Explicit template instantiations (same as original .cppm)
template class boost::ut::reporter_junit<boost::ut::printer>;
template void boost::ut::reporter_junit<boost::ut::printer>::on<bool>(boost::ut::events::log<bool>);
template void boost::ut::reporter_junit<boost::ut::printer>::on<bool>(boost::ut::events::assertion_pass<bool>);
template void boost::ut::reporter_junit<boost::ut::printer>::on<bool>(boost::ut::events::assertion_fail<bool>);
template auto boost::ut::expect<bool>(const bool& expr, const boost::ut::reflection::source_location&);
template void boost::ut::reporter_junit<>::on<boost::ut::detail::fatal_<bool>>(boost::ut::events::assertion_fail<boost::ut::detail::fatal_<bool>>);
template void boost::ut::reporter_junit<>::on<boost::ut::detail::fatal_<bool>>(boost::ut::events::assertion_pass<boost::ut::detail::fatal_<bool>>);
template void boost::ut::reporter_junit<>::on<boost::ut::detail::fatal_<bool>>(boost::ut::events::log<boost::ut::detail::fatal_<bool>>);

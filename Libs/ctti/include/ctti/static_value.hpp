#ifndef CTTI_DETAIL_STATIC_VALUE_HPP
#define CTTI_DETAIL_STATIC_VALUE_HPP

#include <type_traits>

namespace ctti
{

template<typename T, T Value>
struct static_value
{
    constexpr static_value() = default;
    using value_type = T;
    static constexpr value_type value = Value;
};

}

#define CTTI_STATIC_VALUE(x) ::ctti::static_value<typename std::remove_const<decltype(x)>::type, (x)>

#endif // CTTI_DETAIL_STATIC_VALUE_HPP

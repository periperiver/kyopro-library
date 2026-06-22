#pragma once
#include<type_traits>
template<typename T>constexpr std::enable_if_t<std::is_floating_point_v<T>,T> epsilon(){return 1e-10;}
template<typename T>constexpr std::enable_if_t<std::is_integral_v<T>,T>epsilon(){return 0;}
#pragma once
#include<type_traits>
#include<limits>
#include<cstdint>
template<typename T>
using safe_mul_t=std::conditional_t<
std::is_floating_point_v<T>,T,
std::conditional_t<(std::numeric_limits<T>::digits<=32),
std::conditional_t<std::is_signed_v<T>,int64_t,uint64_t>,
std::conditional_t<(std::numeric_limits<T>::digits<=64),
std::conditional_t<std::is_signed_v<T>,__int128_t,__uint128_t>,
void>>>;
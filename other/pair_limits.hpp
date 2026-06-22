#pragma once
#include<limits>
#include<functional>
template<typename T1,typename T2>
struct std::numeric_limits<std::pair<T1,T2>>{
  static constexpr std::pair<T1,T2>max(){return std::make_pair(std::numeric_limits<T1>::max(),std::numeric_limits<T2>::max());}
  static constexpr std::pair<T1,T2>min(){return std::make_pair(std::numeric_limits<T1>::min(),std::numeric_limits<T2>::min());}
};
#pragma once
template<typename T>
constexpr T round_int(T a,T b){
  if(b<0)a=-a,b=-b;
  if(a>=0)return (a+b/2)/b;
  else return (a-b/2)/b;
}
#pragma once
#include "online_mobius.hpp"
#include "online_zeta.hpp"
template<typename T>
struct OnlineOrConvolution{
private:
  OnlineZeta<T>a,b;
  OnlineMobius<T>c;
public:
  explicit OnlineOrConvolution(int n):a(n),b(n),c(n){}
  T query(T x,T y){return c.query(a.query(x)*b.query(y));}
};
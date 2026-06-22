#pragma once
#include<utility>
struct BarrettReduction{
private:
  using i64=long long;
  using u64=unsigned long long;
  using u32=unsigned int;
  using u128=__uint128_t;
  u32 m;
  u64 im;
public:
  BarrettReduction():m(0),im(0){}
  BarrettReduction(u32 n):m(n),im(u64(-1)/n+1){}
  inline i64 quo(u64 x)const{
    if(m==1)return x;
    u64 y=u64((u128(x)*im)>>64);
    u32 r=x-y*m;
    return m<=r?y-1:y;
  }
  inline u32 rem(u64 x)const{
    if(m==1)return 0;
    u64 y=u64((u128(x)*im)>>64);
    u32 r=x-y*m;
    return m<=r?r+m:r;
  }
  inline std::pair<u64,u32>quo_rem(u64 x)const{
    if(m==0)return std::make_pair(x,0);
    u64 y=u64((u128(x)*im)>>64);
    u32 r=x-y*m;
    return m<=r?std::make_pair(y-1,r+m):std::make_pair(y,r);
  }
  inline u32 pow(u32 a,u64 p)const{
    u32 res=m!=1;
    while(p){
      if(p&1)res=rem(u64(res)*a);
      a=rem(u64(a)*a);
      p>>=1;
    }
    return res;
  }
};
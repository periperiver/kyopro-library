#pragma once
template<unsigned m>
struct Montgomery{
  static_assert(m%2==1);
  static_assert(m>=3);
  static_assert(m<(1u<<30));
  static constexpr unsigned r=[](){
    unsigned res=m;
    for(int i=0;i<4;i++)res*=2-m*res;
    return res;
  }();
  static constexpr unsigned r2=-((unsigned long long)m)%m;
  static constexpr unsigned reduce(unsigned long long x){
    return (x+(unsigned long long)((unsigned)x*(unsigned)(-r))*m)>>32;
  }
  static constexpr unsigned mod(){return m;}
};
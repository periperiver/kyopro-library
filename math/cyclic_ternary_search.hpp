#pragma once
#include<utility>
#include<functional>
template<typename Func>
std::pair<int,decltype(std::declval<Func>()(0))>cyclic_ternary_search(int n,const Func&f){
  using T=decltype(std::declval<Func>()(0));
  int l=0,m=n,r=n*2;
  T val=f(0);
  while(r-l>2){
    int l2=(l+m)/2,r2=(m+r+1)/2;
    T lv=f(l2%n),rv=f(r2%n);
    if(lv<val){
      val=lv;
      r=m;
      m=l2;
    }
    else if(rv<val){
      val=rv;
      l=m;
      m=r2;
    }
    else{
      l=l2;
      r=r2;
    }
  }
  return std::make_pair(m%n,val);
}
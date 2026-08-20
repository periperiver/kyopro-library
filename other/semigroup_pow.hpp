#pragma once
#include<cassert>
#include<functional>
#include<concepts>
#include "../math/util.hpp"
template<typename T,typename U,typename Func>
T semigroup_pow(T a,U k,const Func&f){
  static_assert(std::is_convertible_v<Func,std::function<T(T,T)>>);
  assert(k>0);
  if(k<=8){
    if(k==1)return a;
    else if(k==2)return f(a,a);
    else if(k==3)return f(a,f(a,a));
    else if(k==4){
      a=f(a,a);
      return f(a,a);
    }
    else if(k==5){
      T a2=f(a,a);
      return f(a,f(a2,a2));
    }
    else if(k==6){
      a=f(a,f(a,a));
      return f(a,a);
    }
    else if(k==7){
      T a3=f(a,f(a,a));
      return f(a,f(a3,a3));
    }
    else{
      a=f(a,a);
      a=f(a,a);
      return f(a,a);
    }
  }
  int m=msb(k)/3;
  T a2=f(a,a);
  T a3=f(a,a2);
  T a5=f(a2,a3);
  T a7=f(a2,a5);
  T res=[&]()->T {
    switch(k>>(m*3)&7){
      case 1:return a;
      case 2:return a2;
      case 3:return a3;
      case 4:return f(a2,a2);
      case 5:return a5;
      case 6:return f(a3,a3);
      case 7:return a7;
    }
    __builtin_unreachable();
  }();
  while(m--){
    int x=k>>(m*3)&7;
    if(x==4){
      res=f(res,res);
      res=f(res,a);
      res=f(res,res);
      res=f(res,res);
    }
    else if(x==6){
      res=f(res,res);
      res=f(res,res);
      res=f(res,a3);
      res=f(res,res);
    }
    else{
      res=f(res,res);
      res=f(res,res);
      res=f(res,res);
      if(x==1)res=f(res,a);
      else if(x==2)res=f(res,a2);
      else if(x==3)res=f(res,a3);
      else if(x==5)res=f(res,a5);
      else if(x==7)res=f(res,a7);
    }
  }
  return res;
}
#pragma once
#include<vector>
namespace convex_floor_sum_impl{
template<typename T,typename F>
T exp_search(F f){
  T e=1;
  while(f(e))e*=2;
  T ok=e/2,ng=e;
  while(ng-ok>1){
    T mid=(ok+ng)/2;
    (f(mid)?ok:ng)=mid;
  }
  return ok;
}
//f'(x)≥a/b
template<typename T,typename T2=T,typename F1,typename F2>
T2 convex_floor_sum(T n,F1 f,F2 slope){
  if(n==0)return 0;
  T2 res=0;
  auto add=[&](T&x,T&y,T a,T b){
    T e=exp_search<T>([&](T d){
      T nx=x+d*a,ny=y+d*b;
      return nx<n&&f(nx)<ny;
    });
    T nx=x+e*a,ny=y+e*b;
    res+=T2(y-1)*T2(nx-x);
    res+=(T2(nx-x+1)*T2(ny-y+1)-(e+1))/2;
    x=nx,y=ny;
  };
  T x=0,y=f(0);
  res+=y++;
  add(x,y,1,0);
  std::vector<std::tuple<T,T,T,T>>sbt;
  sbt.emplace_back(1,0,0,1);
  while(x<n-1){
    auto [a,b,c,d]=sbt.back();
    if(x+c>=n||f(x+c)>=y+d){
      sbt.pop_back();
      continue;
    }
    {
      T e=exp_search<T>([&](T e){
        if(x+a+e*c>=n)return false;
        if(f(x+a+e*c)<y+b+e*d)return false;
        return !slope(x+a+e*c,d,c);
      });
      a+=e*c;
      b+=e*d;
    }
    if(x+a+c<n&&f(x+a+c)<y+b+d){
      T e=exp_search<T>([&](T e){
        if(x+c+e*a>=n)return false;
        return f(x+c+e*a)<y+d+e*b;
      });
      c+=e*a;
      d+=e*b;
      sbt.emplace_back(a,b,c,d);
      continue;
    }
    add(x,y,c,d);
  }
  return res;
}
}
using convex_floor_sum_impl::convex_floor_sum;
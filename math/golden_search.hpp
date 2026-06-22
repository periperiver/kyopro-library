#pragma once
#include<type_traits>
#include<cassert>
#include<functional>
#include<utility>
//min/max f(x) | l<=x<r
template<typename Key,bool MIN=true,typename Func>
std::enable_if_t<std::is_integral_v<Key>,std::pair<Key,decltype(std::declval<Func>()(std::declval<Key>()))>>golden_search(Key l,Key r,const Func&f){
  using Value=decltype(std::declval<Func>()(std::declval<Key>()));
  assert(l<r);
  r--;
  Key a=l-1,b,x;
  Key s=1,t=1;
  while(t<=r-l+1)std::swap(s+=t,t);
  x=a+t-s,b=a+t;
  Value fx=f(x),fy;
  while(x-a!=b-x){
    Key y=a+b-x;
    if(r<y||(fy=f(y),MIN?fx<fy:fx>fy))b=a,a=y;
    else a=x,x=y,fx=fy;
  }
  return std::make_pair(x,fx);
}
//min/max f(x) | l<=x<=r
template<typename Key,bool MIN=true,int loop=100,typename Func>
std::enable_if_t<std::is_floating_point_v<Key>,std::pair<Key,decltype(std::declval<Func>()(std::declval<Key>()))>>golden_search(Key l,Key r,const Func&f){
  using Value=decltype(std::declval<Func>()(std::declval<Key>()));
  assert(l<=r);
  constexpr Key s=0.3819660112501051;
  Key dx=(r-l)*s;
  Value fx=f(l+dx),fy;
  for(int i=0;i<loop;i++){
    fy=f(r-dx);
    if(MIN==(fx<fy))r-=dx,std::swap(l,r);
    else l+=dx,fx=fy;
    dx=(r-l)*s;
  }
  return std::make_pair(l,fx);
}
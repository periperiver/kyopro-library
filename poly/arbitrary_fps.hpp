#pragma once
#include<vector>
#include<type_traits>
#include<functional>
template<typename T,typename Func>
std::vector<T>arbitrary_fps_inv(const std::vector<T>&a,const Func&f,int deg=-1){
  static_assert(std::is_convertible_v<Func,std::function<std::vector<T>(std::vector<T>,std::vector<T>)>>);
  int n=a.size();
  if(deg==-1)deg=n;
  std::vector<T>b{a[0].inv()};
  T two=2;
  for(int i=1;i<deg;i<<=1){
    std::vector<T>h=f(b,b);
    h=f(h,std::vector<T>(a.begin(),a.begin()+std::min<int>(n,i*2)));
    b.resize(i*2);
    for(int j=0;j<i;j++)b[j]*=two;
    for(int j=0;j<i*2;j++)b[j]-=h[j];
  }
  b.resize(deg);
  return b;
}
template<typename T,typename Func>
std::vector<T>arbitrary_fps_log(const std::vector<T>&a,const Func&f,int deg=-1){
  static_assert(std::is_convertible_v<Func,std::function<std::vector<T>(std::vector<T>,std::vector<T>)>>);
  int n=a.size();
  if(deg==-1)deg=n;
  std::vector<T>inv=arbitrary_fps_inv(a,f,deg);
  std::vector<T>diff(n-1);
  for(int i=0;i<n-1;i++)diff[i]=T::raw(i+1)*a[i+1];
  diff=f(diff,inv);
  diff.resize(deg);
  for(int i=deg-1;i>=1;i--)diff[i]=diff[i-1]/T::raw(i);
  diff[0]=0;
  return diff;
}
template<typename T,typename Func>
std::vector<T>arbitrary_fps_exp(const std::vector<T>&a,const Func&f,int deg=-1){
  static_assert(std::is_convertible_v<Func,std::function<std::vector<T>(std::vector<T>,std::vector<T>)>>);
  int n=a.size();
  if(deg==-1)deg=n;
  std::vector<T>b{1};
  for(int m=1;m<deg;m<<=1){
    std::vector<T>nxt=arbitrary_fps_log(b,f,m*2);
    for(int i=0;i<m*2;i++)nxt[i]=-nxt[i];
    for(int i=0;i<std::min(n,m*2);i++)nxt[i]+=a[i];
    nxt[0]+=1;
    b=f(b,nxt);
    b.resize(m*2);
  }
  b.resize(deg);
  return b;
}
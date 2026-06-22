#pragma once
#include<vector>
#include<type_traits>
#include<cassert>
#include<functional>
#include<algorithm>
template<typename T,typename Func>
std::vector<T>arbitrary_chirp_z_transform(std::vector<T>a,T r,int m,const Func&f){
  if(std::min<int>(a.size(),m)<60){
    std::vector<T>res(m);
    T prod=1;
    for(int i=0;i<m;i++){
      T pw=1;
      for(int j=0;j<std::ssize(a);j++){
        res[i]+=pw*a[j];
        pw*=prod;
      }
      prod*=r;
    }
    return res;
  }
  static_assert(std::is_convertible_v<Func,std::function<std::vector<T>(std::vector<T>,std::vector<T>)>>);
  int n=a.size();
  auto fx=[&](T x)->T {
    T res=0,pw=1;
    for(int i=0;i<n;i++){
      res+=pw*a[i];
      pw*=x;
    }
    return res;
  };
  auto calc=[](T r,int m)->std::vector<T> {
    std::vector<T>res(m);
    T pw=1;
    res[0]=pw;
    for(int i=1;i<m;i++){
      res[i]=res[i-1]*pw;
      pw*=r;
    }
    return res;
  };
  std::vector<T>b=calc(r.inv(),std::max(n,m));
  for(int i=0;i<n;i++)a[i]*=b[i];
  std::reverse(a.begin(),a.end());
  a=f(calc(r,n+m-1),a);
  a.erase(a.begin(),a.begin()+n-1);
  a.erase(a.begin()+m,a.end());
  for(int i=0;i<m;i++)a[i]*=b[i];
  return a;
}
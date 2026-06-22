#pragma once
#include<vector>
#include<algorithm>
template<typename T>
std::vector<T> berlekamp_massey(const std::vector<T>&s){
  const int n=s.size();
  vector<T>b,c;
  b.reserve(n+1),c.reserve(n+1);
  b.emplace_back(1),c.emplace_back(1);
  T y=1;
  for(int i=1;i<=n;i++){
    int l=c.size(),m=b.size();
    T x=0;
    for(int j=0;j<l;j++)x+=c[j]*s[i-l+j];
    b.emplace_back(0);
    ++m;
    if(x==0)continue;
    T f=x/y;
    if(l<m){
      auto c2=c;
      c.insert(c.begin(),m-l,0);
      for(int j=0;j<m;j++)c[m-1-j]-=f*b[m-1-j];
      b=c2;
      y=x;
    }
    else{
      for(int j=0;j<m;j++)c[l-1-j]-=f*b[m-1-j];
    }
  }
  std::reverse(c.begin(),c.end());
  return c;
}
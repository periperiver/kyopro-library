#pragma once
#include<vector>
template<typename T>
struct OnlineMobius{
private:
  int n,p;
  std::vector<T>dat;
public:
  explicit OnlineMobius(int n_):n(n_),p(0),dat(1<<n){}
  T query(T x){
    dat[p++]=x;
    int i;
    for(i=1;!(p&i);i<<=1){
      for(int j=p-i;j<p;j++)dat[j]-=dat[j-i];
    }
    T res=dat[p-1];
    for(int j=(p-1)&-i;j>0;j-=j&-j){
      res-=dat[p-1-(j&-j)];
    }
    return res;
  }
};
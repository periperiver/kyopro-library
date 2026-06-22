#pragma once
#include<vector>
#include<numeric>
#include<cmath>
struct Quotients{
  std::vector<long long>dat;
  long long n,s;
  Quotients(){}
  explicit Quotients(long long n_):n(n_),s(std::sqrt(n_)){
    dat.resize(s*2-(s==n/s));
    std::iota(dat.begin(),dat.begin()+s,1);
    for(int i=s-(s==n/s);i>=1;i--)dat[dat.size()-i]=n/i;
  }
  const long long &operator[](int i)const{return dat[i];}
  int get_key(long long k)const{
    if(k<=s)return k-1;
    else return dat.size()-(int)((double)n/(double)k);
  }
  int size()const{return dat.size();}
};
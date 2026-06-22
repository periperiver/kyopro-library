#pragma once
#include<vector>
#include "rolling_hash_base.hpp"
struct RollingHash{
private:
  std::vector<mint61>dat;
public:
  RollingHash(){}
  template<typename T>
  RollingHash(const T&s){
    RollingHashBase::resize(s.size());
    dat.resize(s.size()+1);
    dat[0]=0;
    for(int i=1;i<dat.size();i++)dat[i]=dat[i-1]*RollingHashBase::base+s[i-1];
  }
  inline mint61 get(int l,int r)const{return dat[r]-dat[l]*RollingHashBase::power[r-l];}
};
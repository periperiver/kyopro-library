#pragma once
#include "template.hpp"
#include "string/rollinghash_base.hpp"
template<typename T,const ull base=10007>
struct multihash{
private:
  const int n;
  vector<ull>data;
  void add(int p,ull x){
    assert(0<=p&&p<n);
    p++;
    while(p<=n){
      data[p-1]+=x;
      if(data[p-1]>=RollingHashBase::MOD)data[p-1]-=RollingHashBase::MOD;
      p+=p&-p;
    }
  }
public:
  multihash(T s):n(s.size()),data(s.size(),0){
    rep(i,n){
      add(i,RollingHashBase::pow61(base,s[i]));
    }
  }
  ull get(int r){
    ull ret=0;
    while(r>0){
      ret+=data[r-1];
      if(ret>=RollingHashBase::MOD)ret-=RollingHashBase::MOD;
      r-=r&-r;
    }
    return ret;
  }
  inline ull get(int l,int r){
    ull sumr=get(r),suml=get(l);
    if(sumr>=suml)return sumr-suml;
    else return RollingHashBase::MOD+sumr-suml;
  }
  inline void set(int p,ull x){
    ull g=get(p,p+1);
    x=RollingHashBase::pow61(base,x);
    if(x>=g)add(p,x-g);
    else add(p,RollingHashBase::MOD+x-g);
  }
};
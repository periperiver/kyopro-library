#pragma once
#include<vector>
#include<limits>
#include<cassert>
#include "../math/util.hpp"
template<typename T>
struct KineticTournament{
private:
  static constexpr T inf=std::numeric_limits<T>::max();
  struct line{
    T a,b;
    line():a(0),b(inf){}
    line(T a_,T b_):a(a_),b(b_){}
    T operator()(const T&x)const{return a*x+b;}
  };
  int n,z;
  T nowt;
  std::vector<line>seg;
  std::vector<T>time;
  void update(int i){
    line f=seg[i*2],g=seg[i*2+1];
    if(f(nowt)>g(nowt))std::swap(f,g);
    seg[i]=f;
    T nxt=inf;
    if(f.a>g.a){
      nxt=safe_div<T>(g.b-f.b,f.a-g.a);
      if(nxt<=nowt)nxt=inf;
    }
    time[i]=std::min({time[i*2],time[i*2+1],nxt});
  }
public:
  KineticTournament(){}
  KineticTournament(int n_,T t):n(n_),z(ceil_pow2(n)),nowt(t),seg(z*2),time(z*2,inf){}
  KineticTournament(const std::vector<std::pair<T,T>>&init,T t):n(init.size()),z(ceil_pow2(n)),nowt(t),seg(z*2),time(z*2,inf){
    for(int i=0;i<n;i++)seg[i+z]=line(init[i].first,init[i].second);
    for(int i=z-1;i>=1;i--)update(i);
  }
  void set(int i,T a,T b){
    assert(0<=i&&i<n);
    seg[i+=z]=line(a,b);
    while(i>>=1)update(i);
  }
  T min(int l,int r,T t){
    assert(0<=l&&l<=r&&r<=n);
    assert(nowt<=t);
    if(nowt!=t){
      nowt=t;
      auto dfs=[&](auto self,int i)->void {
        if(i>=z||time[i]>nowt)return;
        self(self,i*2),self(self,i*2+1);
        update(i);
      };
      dfs(dfs,1);
    }
    T res=inf;
    if(r==n)r=z;
    l+=z,r+=z;
    while(l<r){
      if(l&1)res=std::min(res,seg[l++](t));
      if(r&1)res=std::min(res,seg[--r](t));
      l>>=1,r>>=1;
    }
    return res;
  }
};
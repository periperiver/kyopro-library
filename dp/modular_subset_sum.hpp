#pragma once
#include "../datastructure/shift_tree.hpp"
#include "../datastructure/csr_array.hpp"
struct ModularSubsetSum{
private:
  int mod;
  std::vector<int>dat,pre;
  static constexpr int bit_reverse(int x,int k){
    int res=0;
    for(int i=0;i<k;i++)if(x>>i&1)res|=(1<<(k-i-1));
    return res;
  }
public:
  ModularSubsetSum(const std::vector<int>&a,int mod_):mod(mod_),dat(a),pre(mod,-1){
    int n=ceil_pow2(mod*2);
    int k=msb(n);
    ShiftTree t1(k),t2(k);
    t1.set(0,1),t2.set(0,1);
    t2.set(n-mod,1);
    csr_array<int>idx(mod,a);
    std::vector<int>diff;
    for(int i=1;i<n;i++){
      int x=bit_reverse(i,k);
      if(x>=mod)continue;
      if(!idx[x].size())continue;
      t2.shift(x-t2.delta);
      for(int id:idx[x]){
        diff.clear();
        ShiftTree::diff(t1,t2,0,mod,diff);
        for(int d:diff)if(d){
          if(pre[d]==-1){
            pre[d]=id;
            t1.set(d,mint61::raw(1));
            t2.set((d+x)%n,mint61::raw(1));
            t2.set((n+d+x-mod)%n,mint61::raw(1));
          }
        }
      }
    }
  }
  inline bool operator[](int i)const{return i==0||pre[i]!=-1;}
  std::vector<bool> restore(int x)const{
    std::vector<bool>res(dat.size(),false);
    while(x){
      int i=pre[x];
      res[i]=true;
      x-=dat[i];
      if(x<0)x+=mod;
    }
    return res;
  }
};
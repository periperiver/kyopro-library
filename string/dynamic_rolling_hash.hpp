#pragma once
#include<utility>
#include "rolling_hash_base.hpp"
#include "datastructure/segmenttree.hpp"
struct DynamicRollingHash{
private:
  struct DynamicRollingHashMonoid{
    using S=std::pair<mint61,int>;
    static inline S op(const S&x,const S&y){return std::make_pair(x.first*RollingHashBase::power[y.second]+y.first,x.second+y.second);}
    static inline S e(){return std::make_pair(0,0);}
  };
  SegmentTree<DynamicRollingHashMonoid>seg;
public:
  DynamicRollingHash(){}
  template<typename T>
  DynamicRollingHash(const T&s){
    RollingHashBase::resize(s.size());
    std::vector<typename DynamicRollingHashMonoid::S>init(s.size());
    for(int i=0;i<s.size();i++)init[i]=std::make_pair(s[i],1);
    seg=SegmentTree<DynamicRollingHashMonoid>(init);
  }
  template<typename T>
  inline void set(int i,const T&c){seg.set(i,std::make_pair(c,1));}
  inline mint61 get(int l,int r)const{return seg.prod(l,r).first;}
  unsigned long long get(int i)const{return seg.get(i).first.val();}
};
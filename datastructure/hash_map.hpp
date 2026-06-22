#pragma once
#include<vector>
#include<type_traits>
#include<limits>
#include "random/prime.hpp"
#include "math/barrett_reduction.hpp"
template<typename Key,typename Val>
struct HashMap{
  static_assert(std::is_integral_v<Key>);
  static_assert(!std::is_same_v<Val,bool>);
private:
  const int p;
  const Val def;
  std::vector<Key>key;
  std::vector<Val>value;
  BarrettReduction brp,brp1;
  static constexpr Key none=std::numeric_limits<std::make_signed_t<Key>>::max();
public:
  struct Iterator{
    using k_itr=typename std::vector<Key>::const_iterator;
    using v_itr=typename std::vector<Val>::iterator;
    k_itr kp;
    v_itr vp;
    k_itr kp2;
    Iterator operator++(){
      ++kp,++vp;
      step();
      return *this;
    }
    inline void step(){
      while(kp!=kp2&&(*kp)==none)++kp,++vp;
    }
    std::pair<const Key&,Val&> operator*(){
      return {*kp,*vp};
    }
    bool operator!=(Iterator rhs){return kp!=rhs.kp;}
  };
  HashMap(){}
  HashMap(int n,Val def_=Val()):def(def_),p(Random::prime(n*4/3,n*4/3+1000)){
    key.resize(p,none);
    value.resize(p,def);
    brp=BarrettReduction(p);
    brp1=BarrettReduction(p-1);
  }
  Val get(const Key&k)const{
    int now=brp.rem(k);
    int of=brp1.rem(k)+1;
    while(key[now]!=none&&key[now]!=k){
      now+=of;
      if(now>=p)now-=p;
    }
    return key[now]==none?def:value[now];
  }
  Val& set(const Key&k){
    int now=brp.rem(k);
    int of=brp1.rem(k)+1;
    while(key[now]!=none&&key[now]!=k){
      now+=of;
      if(now>=p)now-=p;
    }
    key[now]=k;
    return value[now];
  }
  Val &operator[](const Key&k){return set(k);}
  const Val& exist_get(const Key&k)const{
    int now=brp.rem(k);
    int of=brp1.rem(k)+1;
    while(key[now]!=k){
      now+=of;
      if(now>=p)now-=p;
    }
    return value[now];
  }
  Iterator begin(){
    Iterator res;
    res.kp=key.begin();
    res.vp=value.begin();
    res.kp2=key.end();
    res.step();
    return res;
  }
  Iterator end(){
    Iterator res;
    res.kp=res.kp2=key.end();
    res.vp=value.end();
    return res;
  }
};
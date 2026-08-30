#pragma once
#include<vector>
#include<algorithm>
#include<cassert>
#include "../random/generator.hpp"
#include "../math/util.hpp"
template<typename M>
struct VantagePointTree{
private:
  using Data=typename M::Data;
  using Dist=decltype(M::norm(std::declval<Data>(),std::declval<Data>()));
  int n,z;
  std::vector<std::tuple<Data,Dist,int>>dat;
  std::vector<int>sz,pos;
  std::pair<int,Dist>rec(int id,const Data&d)const{
    static int f=0;
    if(sz[id]==0)return std::make_pair(-1,Dist{});
    if(id<z){
      if(sz[id*2]==0)return rec(id*2+1,d);
      if(sz[id*2+1]==0)return rec(id*2,d);
      Dist diff=M::norm(std::get<0>(dat[id]),d);
      if(diff<std::get<1>(dat[id])){
        auto [lid,lres]=rec(id*2,d);
        if(std::get<1>(dat[id])<diff+lres){
          auto [rid,rres]=rec(id*2+1,d);
          if(lres<rres)return std::make_pair(lid,lres);
          return std::make_pair(rid,rres);
        }
        else return std::make_pair(lid,lres);
      }
      else{
        auto [rid,rres]=rec(id*2+1,d);
        if(diff<std::get<1>(dat[id])+rres){
          auto [lid,lres]=rec(id*2,d);
          if(lres<rres)return std::make_pair(lid,lres);
          return std::make_pair(rid,rres);
        }
        else return std::make_pair(rid,rres);
      }
    }
    else return std::make_pair(std::get<2>(dat[id]),M::norm(std::get<0>(dat[id]),d));
  }
public:
  VantagePointTree(){}
  explicit VantagePointTree(const std::vector<Data>&init):n(init.size()),z(ceil_pow2(n)),dat(z*2),sz(z*2),pos(n){
    std::vector<std::pair<int,int>>lr(z*2);
    lr[1]=std::make_pair(0,n);
    std::vector<std::pair<Data,int>>a(n);
    for(int i=0;i<n;i++)a[i]=std::make_pair(init[i],i);
    for(int i=1;i<z;i++){
      auto [l,r]=lr[i];
      int m=(l+r)/2;
      auto [target,idx]=a[Random::range(l,r)];
      std::nth_element(a.begin()+l,a.begin()+m,a.begin()+r,[&](const std::pair<Data,int>&lhs,const std::pair<Data,int>&rhs){
        return M::norm(target,lhs.first)<M::norm(target,rhs.first);
      });
      dat[i]=std::make_tuple(target,M::norm(target,std::get<0>(a[m])),idx);
      lr[i*2]=std::make_pair(l,m);
      lr[i*2+1]=std::make_pair(m,r);
    }
    for(int i=z;i<z*2;i++){
      auto [l,r]=lr[i];
      if(l<r){
        pos[a[l].second]=i;
        dat[i]=std::make_tuple(a[l].first,Dist{},a[l].second);
      }
    }
  }
  void enable(int i){
    assert(0<=i&&i<n);
    i=pos[i];
    if(sz[i]==0)do sz[i]++;while(i>>=1);
  }
  void disable(int i){
    assert(0<=i&&i<n);
    i=pos[i];
    if(sz[i]==1)do sz[i]--;while(i>>=1);
  }
  std::pair<int,Dist>query(const Data&d)const{
    return rec(1,d);
  }
};
template<typename T,typename T4>
struct EuclidData{
  using Data=std::pair<T,T>;
  static constexpr T none=std::numeric_limits<T>::max();
  struct sqrt2{
    T a,b;
    sqrt2():a(none),b(none){}
    sqrt2 operator+(const sqrt2&rhs)const{
      sqrt2 res;
      res.a=a;
      res.b=rhs.a;
      return res;
    }
    bool operator<(const sqrt2&rhs)const{
      if(rhs.b==none){
        return a<rhs.a;
      }
      if(a<rhs.a+rhs.b)return true;
      T4 l=a-rhs.a-rhs.b;
      l=l*l;
      T4 r=T4(4)*T4(rhs.a)*T4(rhs.b);
      return l<r;
    }
  };
  static sqrt2 norm(Data lhs,Data rhs){
    T dx=lhs.first-rhs.first;
    T dy=lhs.second-rhs.second;
    sqrt2 res;
    res.a=dx*dx+dy*dy;
    return res;
  }
};
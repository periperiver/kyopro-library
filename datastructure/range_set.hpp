#pragma once
#include<set>
#include<cassert>
template<typename T>
struct RangeSet{
private:
  std::set<std::pair<T,T>>s;
public:
  T sum;
  RangeSet():s{},sum(0){}
  void insert(T l,T r){
    assert(l<=r);
    if(l==r)return;
    auto itr=s.lower_bound(std::make_pair(l,l));
    if(itr!=s.begin()){
      auto itr2=std::prev(itr);
      if(itr2->second>=l){
        sum-=itr2->second-itr2->first;
        l=itr2->first;
        r=std::max(r,itr2->second);
        itr=s.erase(itr2);
      }
    }
    while(itr!=s.end()&&itr->second<=r){
      sum-=itr->second-itr->first;
      itr=s.erase(itr);
    }
    if(itr==s.end()||itr->first>r){
      s.emplace(l,r);
      sum+=r-l;
      return;
    }
    sum+=itr->first-l;
    r=itr->second;
    s.erase(itr);
    s.emplace(l,r);
  }
  void erase(T l,T r){
    assert(l<=r);
    if(l==r)return;
    auto itr=s.lower_bound(std::make_pair(l,l));
    if(itr!=s.begin()){
      auto itr2=std::prev(itr);
      if(l<itr2->second){
        auto [l2,r2]=*itr2;
        s.erase(itr2);
        if(r<r2){
          s.emplace(l2,l);
          s.emplace(r,r2);
          sum-=r-l;
          return;
        }
        s.emplace(l2,l);
        sum-=r2-l;
      }
    }
    while(itr!=s.end()&&itr->second<=r){
      sum-=itr->second-itr->first;
      itr=s.erase(itr);
    }
    if(itr!=s.end()&&itr->first<r){
      auto [l2,r2]=*itr;
      s.erase(itr);
      sum-=r-l2;
      s.emplace(r,r2);
    }
  }
  bool contains(T l,T r)const{
    auto itr=s.lower_bound(std::make_pair(l+1,l));
    if(itr!=s.begin()){
      itr=std::prev(itr);
      return r<=itr->second;
    }
    return false;
  }
  std::set<std::pair<T,T>>::iterator begin(){return s.begin();}
  std::set<std::pair<T,T>>::iterator end(){return s.end();}
  std::set<std::pair<T,T>>::const_iterator begin()const{return s.begin();}
  std::set<std::pair<T,T>>::const_iterator end()const{return s.end();}
};
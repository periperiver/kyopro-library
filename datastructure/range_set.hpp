#pragma once
#include "template.hpp"
template<typename T>
struct range_set{
private:
  set<pair<T,T>>s;
  T INF;
  T SUM;
public:
  range_set():INF(numeric_limits<T>::max()/2),SUM(0){
    s.insert({INF,INF+1});
    s.insert({-INF,-INF+1});
  }
  T insert(T l,T r){
    if(l==r)return 0;
    assert(l<r);
    auto itr=prev(s.lower_bound({l+1,0}));
    if((*itr).first<=l&&r<=(*itr).second)return 0;
    T sum=0;
    if((*itr).first<=l&&l<=(*itr).second){
      l=(*itr).first;
      sum+=(*itr).second-(*itr).first;
      itr=s.erase(itr);
    }
    else itr=next(itr);
    while(r>(*itr).second){
      sum+=(*itr).second-(*itr).first;
      itr=s.erase(itr);
    }
    if((*itr).first<=r){
      sum+=(*itr).second-(*itr).first;
      r=(*itr).second;
      itr=s.erase(itr);
    }
    s.insert({l,r});
    SUM+=r-l-sum;
    return r-l-sum;
  }
  T insert(T x){
    return insert(x,x+1);
  }
  T erase(T l,T r){
    if(l==r)return 0;
    assert(l<r);
    auto itr=prev(s.lower_bound({l+1,0}));
    if((*itr).first<=l&&r<=(*itr).second){
      if((*itr).first!=l)s.insert({(*itr).first,l});
      if((*itr).second!=r)s.insert({r,(*itr).second});
      s.erase(itr);
      SUM-=r-l;
      return r-l;
    }
    T sum=0;
    if((*itr).first<=l&&l<(*itr).second){
      sum+=(*itr).second-l;
      if((*itr).first!=l)s.insert({(*itr).first,l});
      itr=s.erase(itr);
    }
    else itr=next(itr);
    while((*itr).second<=r){
      sum+=(*itr).second-(*itr).first;
      itr=s.erase(itr);
    }
    if((*itr).first<r){
      sum+=r-(*itr).first;
      if((*itr).second!=r)s.insert({r,(*itr).second});
      itr=s.erase(itr);
    }
    SUM-=sum;
    return sum;
  }
  T erase(T x){
    return erase(x,x+1);
  }
  int size()const{return s.size()-2;}
  T sum()const{return SUM;}
  bool contains(T l,T r)const{
    assert(l<r);
    auto itr=prev(s.lower_bound({l+1,0}));
    return (*itr).first<=l&&r<=(*itr).second;
  }
  bool contains(T x)const{
    return contains(x,x+1);
  }
  T p1(T x)const{
    auto itr=prev(s.lower_bound({x+1,0}));
    if((*itr).first<=x+1&&x+1<(*itr).second)return x+1;
    itr=next(itr);
    return (*itr).first;
  }
  T m1(T x)const{
    auto itr=prev(s.lower_bound({x,0}));
    if((*itr).first<=x-1&&x-1<(*itr).second)return x-1;
    return (*itr).second-1;
  }
};
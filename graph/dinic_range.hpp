#pragma once
#include<optional>
#include "dinic.hpp"
template<typename T>
struct DinicRange{
private:
  int n;
  T s;
  Dinic<T>g;
public:
  DinicRange(){}
  DinicRange(int n):n(n),s(0),g(n+2){}
  void add_edge(int u,int v,T lower,T upper){
    assert(0<=u&&u<n);
    assert(0<=v&&v<n);
    assert(0<=lower&&lower<=upper);
    if(lower>0)g.add_edge(n,v,lower),g.add_edge(u,n+1,lower);
    if(lower<upper)g.add_edge(u,v,upper-lower);
    s+=lower;
  }
  std::optional<T> flow(int u,int v){
    assert(0<=u&&u<n);
    assert(0<=v&&v<n);
    T res=-s;
    res+=g.flow(n,n+1);
    res+=g.flow(n,v);
    res+=g.flow(u,n+1);
    res+=g.flow(u,v);
    for(const auto&e:g.edges()){
      if(e.from==n||e.to==n+1){
        if(e.flow<e.cap)return std::nullopt;
      }
    }
    return std::make_optional(res);
  }
};
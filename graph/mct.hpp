#pragma once
#include "mcf.hpp"
#include<cassert>
template<typename Cap,typename Cost>
struct MinCostTension{
private:
  int n;
  Cap inf;
  MinCostFlow<Cap,Cost>mcf;
public:
  MinCostTension(){}
  explicit MinCostTension(int n,Cap inf=std::numeric_limits<Cap>::max()/2):n(n),inf(inf),mcf(n){}
  void add_single(int u,Cap bu){
    assert(0<=u&&u<n);
    mcf.add_source(u,bu);
  }
  //max(0,p_v-p_u-l)*c
  void add_cost(int u,int v,Cap c,Cost l){
    assert(0<=u&&u<n);
    assert(0<=v&&v<n);
    assert(0<=c);
    mcf.add_edge(u,v,0,c,l);
  }
  //p_u<p_v
  void less(int u,int v){
    assert(0<=u&&u<n);
    assert(0<=v&&v<n);
    mcf.add_edge(v,u,0,inf,-1);
  }
  //p_u≤p_v
  void less_equal(int u,int v){
    assert(0<=u&&u<n);
    assert(0<=v&&v<n);
    mcf.add_edge(v,u,0,inf,0);
  }
  //p_u+l=p_v
  void equal(int u,int v,Cost l){
    assert(0<=u&&u<n);
    assert(0<=v&&v<n);
    mcf.add_edge(u,v,0,inf,l);
    mcf.add_edge(v,u,0,inf,-l);
  }
  std::optional<Cost>solve(){
    std::optional<Cost>res=mcf.mincost_flow();
    if(res)return std::make_optional(-*res);
    else return std::nullopt;
  }
  std::vector<Cap>restore(){return mcf.get_potential();}
};
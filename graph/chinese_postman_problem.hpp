#pragma once
#include<optional>
#include<cassert>
#include "general_weighted_matching.hpp"
#include "graph_base.hpp"
template<typename T>
std::optional<T>chinese_postman_problem(const Graph<T>&g){
  assert(!g.is_directed());
  static constexpr T inf=std::numeric_limits<T>::max()/2;
  int n=g.size();
  std::vector<std::vector<T>>dst(n,std::vector<T>(n,inf));
  std::vector<int>deg(n,0);
  T sum=0;
  for(const Edge<T>&e:g){
    if(e.weight<0)return std::nullopt;
    if(dst[e.from][e.to]>e.weight)dst[e.from][e.to]=dst[e.to][e.from]=e.weight;
    deg[e.from]++,deg[e.to]++;
    sum+=e.weight;
  }
  for(int i=0;i<n;i++)dst[i][i]=0;
  for(int i=0;i<n;i++)for(int j=0;j<n;j++)for(int k=0;k<n;k++){
    T now=dst[j][i]+dst[i][k];
    if(dst[j][k]>now)dst[j][k]=now;
  }
  for(int i=0;i<n;i++)for(int j=0;j<n;j++)if(dst[i][j]==inf)return std::nullopt;
  std::vector<int>odd;
  for(int i=0;i<n;i++)if(deg[i]&1)odd.push_back(i);
  if(odd.empty())return std::make_optional(sum);
  std::vector<Edge<T>>edge;
  edge.reserve(odd.size()*(odd.size()-1)/2);
  T mx=0;
  for(int i=0;i<(int)odd.size()-1;i++)for(int j=i+1;j<(int)odd.size();j++){
    edge.emplace_back(i,j,dst[odd[i]][odd[j]]);
    if(mx<edge.back().weight)mx=edge.back().weight;
  }
  mx++;
  for(Edge<T>&e:edge)e.weight=mx-e.weight;
  std::vector<Edge<T>>match=general_weighted_matching(odd.size(),edge);
  for(const Edge<T>&e:match)sum+=mx-e.weight;
  return std::make_optional(sum);
}
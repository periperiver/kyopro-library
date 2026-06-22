#pragma once
#include<vector>
#include<numeric>
#include "graph_base.hpp"
template<typename T>
std::pair<std::vector<T>,std::vector<int>>shortest_path_faster_algorithm(const Graph<T>&g,int s=0){
  static constexpr T inf=std::numeric_limits<T>::max();
  static constexpr T minf=std::numeric_limits<T>::min();
  int n=g.size();
  std::vector<T>dst(n,inf);
  std::vector<int>pre(n,-1);
  dst[s]=0;
  std::vector<int>que(n+1);
  int p=0,q=0;
  std::vector<int>time(n,0);
  std::vector<bool>contain(n,false);
  que[q++]=s;
  contain[s]=true;
  while(p!=q){
    int x=que[p++];
    if(p==n+1)p=0;
    contain[x]=false;
    for(const Edge<T>&e:g[x]){
      T cost=dst[x]+e.weight;
      if(dst[e.to]>cost){
        dst[e.to]=cost;
        pre[e.to]=e.from;
        if(!contain[e.to]){
          if(++time[e.to]>=n)continue;
          contain[e.to]=true;
          que[q++]=e.to;
          if(q==n+1)q=0;
        }
      }
    }
  }
  std::fill(contain.begin(),contain.end(),false);
  p=q=0;
  for(int i=0;i<n;i++)for(const Edge<T>&e:g[i])if(dst[i]!=inf&&e.from==e.to&&e.weight<0)time[i]=n;
  for(int i=0;i<n;i++)if(time[i]>=n){
    que[q++]=i;
    contain[i]=true;
  }
  while(p!=q){
    int x=que[p++];
    dst[x]=minf;
    if(p==n+1)p=0;
    for(const Edge<T>&e:g[x])if(!contain[e.to]){
      contain[e.to]=true;
      que[q++]=e.to;
      if(q==n+1)q=0;
    }
  }
  return std::make_pair(dst,pre);
}
#pragma once
#include "graph_base.hpp"
#include "../math/gf2.hpp"
#include "../random/generator.hpp"
template<typename T,bool restore=false>
std::conditional_t<restore,std::vector<int>,int>shortest_path(const Graph<T>&g,int s,int t,std::vector<int>vs){
  int n=g.size();
  int m=g.edge_size();
  std::erase_if(vs,[&](int v){return s==v||t==v;});
  int k=vs.size();
  assert(0<=s&&s<n);
  assert(0<=t&&t<n);
  assert(k<=24);
  assert(!g.is_directed());
  std::vector<int>mask(n);
  for(int i=0;i<k;i++){
    assert(0<=vs[i]&&vs[i]<n);
    assert(mask[vs[i]]==0);
    mask[vs[i]]=1<<i;
  }
  if(k==0&&s==t){
    if constexpr(restore)return {s};
    else return 0;
  }
  std::vector<std::tuple<int,int,gf2>>edges;
  for(const Edge<T>&e:g)if(e.from!=e.to){
    edges.emplace_back(e.from,e.to,Random::get<unsigned long long>());
  }
  std::vector<int>res;
  std::vector<int>a;
  do{
    a.clear();
    int ptr=0;
    for(auto&[u,v,w]:edges){
      if(v==s)std::swap(u,v);
      if(u==s){
        if(s==t)a.push_back(ptr);
        else a.push_back(v);
      }
      ptr++;
    }
    if(s!=t)std::erase_if(edges,[&](const std::tuple<int,int,gf2>&tp){return std::get<0>(tp)==s||std::get<1>(tp)==s;});
    std::erase_if(vs,[&](int v){return s==v;});
    k=vs.size();
    std::fill(mask.begin(),mask.end(),0);
    for(int i=0;i<k;i++)mask[vs[i]]=1<<i;
    m=edges.size();
    std::vector<std::vector<gf2>>dpv(1<<k,std::vector<gf2>(n)),dpe(1<<k,std::vector<gf2>(m*2));
    dpv[0][t]=1;
    int d=std::numeric_limits<int>::max();
    int ns=-1;
    for(int i=0;i<n;i++){
      if(s==t){
        for(int id:a){
          auto [u,v,w]=edges[id];
          if(dpv.back()[v]!=dpe.back()[id*2]){
            if(ns==-1||ns>v)ns=v;
            d=i+1;
            break;
          }
        }
      }
      else{
        for(int a2:a)if(dpv.back()[a2].val()){
          if(ns==-1||ns>a2)ns=a2;
          d=i+1;
          break;
        }
      }
      if(ns!=-1){
        break;
      }
      std::vector<std::vector<gf2>>ndpv(1<<k,std::vector<gf2>(n)),ndpe(1<<k,std::vector<gf2>(m*2));
      for(int S=0;S<(1<<k);S++){
        for(int j=0;j<m;j++){
          auto [u,v,w]=edges[j];
          if(!(S&mask[v])){
            gf2 w1=(dpv[S][u]-dpe[S][j*2+1])*w;
            ndpv[S|mask[v]][v]+=w1;
            ndpe[S|mask[v]][j*2]+=w1;
          }
          if(!(S&mask[u])){
            gf2 w2=(dpv[S][v]-dpe[S][j*2])*w;
            ndpv[S|mask[u]][u]+=w2;
            ndpe[S|mask[u]][j*2+1]+=w2;
          }
        }
      }
      dpv=std::move(ndpv);
      dpe=std::move(ndpe);
    }
    if constexpr(!restore)return d;
    else if(ns==-1)return {};
    res.push_back(s);
    s=ns;
  }while(s!=t);
  if constexpr(restore){
    res.push_back(t);
    return res;
  }
  else return std::numeric_limits<int>::max();
}
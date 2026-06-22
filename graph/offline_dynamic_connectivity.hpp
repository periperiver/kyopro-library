#pragma once
#include<vector>
#include<numeric>
#include<algorithm>
#include "random/permutation.hpp"
template<typename M>
struct OfflineDynamicConnectivity{
private:
  using S=typename M::S;
  std::vector<int>par;
  std::vector<int>weight;
  std::vector<S>dat;
  std::vector<std::tuple<int,int,int,int>>event;
  int csize;
  std::vector<int>cntl,cntr;
  std::vector<int>perm;
  int find(int u){
    while(u!=par[u]){
      while(weight[par[u]]>=weight[u]){
        dat[par[u]]=M::op(dat[par[u]],M::inverse(dat[u]));
        par[u]=par[par[u]];
      }
      u=par[u];
    }
    return u;
  }
  void disconnect(int u){
    S pre=dat[u];
    while(true){
      if(u==par[u])break;
      u=par[u];
      S npre=dat[u];
      dat[u]=M::op(dat[u],M::inverse(pre));
      pre=npre;
    }
  }
  int connect(int u,int w){
    while(weight[u]>=w){
      dat[par[u]]=M::op(dat[par[u]],dat[u]);
      u=par[u];
    }
    return u;
  }
  void add(int u,int v,int w){
    disconnect(u),disconnect(v);
    while(u!=v){
      u=connect(u,w);
      v=connect(v,w);
      if(perm[u]<perm[v])std::swap(u,v);
      std::swap(par[v],u);
      std::swap(weight[v],w);
    }
    connect(u,0);
  }
  int min_edge(int u,int v){
    if(find(u)!=find(v))return -1;
    while(true){
      if(weight[u]<weight[v])std::swap(u,v);
      if(par[u]==v)break;
      u=par[u];
    }
    return u;
  }
  bool del(int u,int w){
    while(par[u]!=u){
      if(weight[u]==w){
        int v=u;
        while(par[v]!=v){
          v=par[v];
          dat[v]=M::op(dat[v],M::inverse(dat[u]));
        }
        csize++;
        par[u]=u;
        weight[u]=-1;
        return true;
      }
      while(weight[par[u]]>=weight[u]){
        dat[par[u]]=M::op(dat[par[u]],M::inverse(dat[u]));
        par[u]=par[par[u]];
      }
      u=par[u];
    }
    return false;
  }
public:
  OfflineDynamicConnectivity(){}
  OfflineDynamicConnectivity(int n,int q):par(n),weight(n,-1),dat(n,M::e()),csize(n),cntl(q+1,0),cntr(q+1,0){
    std::iota(par.begin(),par.end(),0);
    perm=Random::permutation(n);
  }
  void add_edge(int l,int r,int u,int v){
    if(l==r||u==v)return;
    event.emplace_back(l,r,u,v);
    cntl[l]++;
    cntl[r]++;
    cntr[r]++;
  }
  template<typename Func>
  void build(const Func&f){
    std::vector<std::tuple<int,int,int>>nevent(event.size()*2);
    for(int i=1;i<(int)cntl.size();i++)cntl[i]+=cntl[i-1];
    for(int i=1;i<(int)cntr.size();i++)cntr[i]+=cntr[i-1];
    for(const auto&[l,r,u,v]:event){
      int w=(cntr[r-1]++)+1;
      nevent[--cntl[l]]=std::make_tuple(u,v,w);
      nevent[--cntl[r]]=std::make_tuple(u,v,-w);
    }
    for(int i=0;i<(int)cntl.size()-1;i++){
      std::sort(nevent.begin()+cntl[i],nevent.begin()+cntl[i+1],[&](const std::tuple<int,int,int>&lhs,const std::tuple<int,int,int>&rhs){return std::get<2>(lhs)>std::get<2>(rhs);});
      for(int j=cntl[i];j<cntl[i+1];j++){
        const auto&[u,v,w]=nevent[j];
        if(w>0){
          int p=min_edge(u,v);
          csize--;
          if(p==-1){
            add(u,v,w);
          }
          else if(weight[p]<w){
            del(p,weight[p]);
            add(u,v,w);
          }
          else csize++;
        }
        else{
          if(!del(u,-w))del(v,-w);
        }
      }
      f(i);
    }
  }
  void modify(int u,S x){
    while(true){
      dat[u]=M::op(dat[u],x);
      if(par[u]==u)break;
      u=par[u];
    }
  }
  int size()const{return csize;}
  bool same(int u,int v){return find(u)==find(v);}
  S prod(int u){return dat[find(u)];}
};
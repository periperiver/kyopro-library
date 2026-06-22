#pragma once
#include "../datastructure/static_toptree.hpp"
#include "../convolution/ntt.hpp"
#include "../math/modint.hpp"
#include "../math/crt2.hpp"
template<typename T>
std::vector<long long>frequency_table_tree_distance(const Tree<T>&t){
  using mint1=mod_int<167772161>;
  using mint2=mod_int<998244353>;
  assert(!t.is_directed());
  int n=t.size();
  Tree<T>t2(t);
  t2.remove_parent();
  StaticTopTree stt(std::move(t2));
  std::vector<long long>res(n);
  std::vector<bool>vis(n,false);
  std::vector<std::pair<int,int>>que(n);
  int p=0,q=0;
  auto dfs=[&](auto self,int v)->std::vector<int> {
    if(v<n){
      vis[v]=true;
      return std::vector<int>{v};
    }
    int lv=stt.left[v],rv=stt.right[v];
    std::vector<int>lch=self(self,lv);
    int prep=p;
    bool boundaryA=false;
    if(stt.A[lv]==stt.A[rv]){
      for(int x:lch)que[q++]=std::make_pair(x,1);
      boundaryA=true;
    }
    else{
      que[q++]=std::make_pair(stt.B[lv],0);
    }
    std::vector<mint2>f2;
    while(p<q){
      auto [x,d]=que[p++];
      while(std::ssize(f2)<=d)f2.push_back(mint2());
      f2[d]++;
      vis[x]=false;
      for(const Edge<T>&e:t[x]){
        if(vis[e.to])que[q++]=std::make_pair(e.to,d+1);
        else if(!boundaryA&&e.to==stt.A[lv]){
          for(int y:lch)if(x!=y)que[q++]=std::make_pair(y,d+2);
          boundaryA=true;
        }
      }
    }
    int lvsc=p-prep;
    std::vector<int>rch=self(self,rv);
    for(int x:rch)que[q++]=std::make_pair(x,1);
    std::vector<mint2>g2;
    while(p<q){
      auto [x,d]=que[p++];
      while(std::ssize(g2)<=d)g2.push_back(mint2());
      g2[d]++;
      vis[x]=false;
      for(const Edge<T>&e:t[x]){
        if(vis[e.to])que[q++]=std::make_pair(e.to,d+1);
      }
    }
    int rvsc=p-prep-lvsc;
    if(lvsc<(mint2::mod()+rvsc-1)/rvsc){
      f2=ntt_convolution(f2,g2);
      for(int i=1;i<std::ssize(f2);i++)res[i]+=f2[i].val();
    }
    else{
      std::vector<mint1>f1(f2.size()),g1(g2.size());
      for(int i=0;i<std::ssize(f2);i++)f1[i]=mint1::raw(f2[i].val());
      for(int i=0;i<std::ssize(g2);i++)g1[i]=mint1::raw(g2[i].val());
      f1=ntt_convolution(f1,g1);
      f2=ntt_convolution(f2,g2);
      for(int i=1;i<std::ssize(f1);i++){
        long long now=crt2<mint1::mod(),mint2::mod()>(f1[i].val(),f2[i].val());
        res[i]+=now;
      }
    }
    while(prep<p)vis[que[--p].first]=true;
    q=p;
    if(stt.A[lv]==stt.A[rv]){
      if(std::ssize(lch)<std::ssize(rch))std::swap(lch,rch);
      lch.insert(lch.end(),rch.begin(),rch.end());
    }
    return std::move(lch);
  };
  dfs(dfs,n*2-2);
  return res;
}
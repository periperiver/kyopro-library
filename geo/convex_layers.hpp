#pragma once
#include "point.hpp"
#include "math/util.hpp"
#include<vector>
#include<algorithm>
#include<numeric>
namespace convex_layers_impl{
template<typename T>
struct DynamicLowerHull{
  std::vector<Point<T>>p;
  std::vector<std::pair<int,int>>seg;
  std::vector<bool>lazy;
  int n,log2n;
  static bool convex(const Point<T>&a,const Point<T>&b,const Point<T>&c){
    return cross(b-a,c-a)>=0;
  }
  inline int getl(int i){
    int d=msb(i);
    int len=n>>d;
    return len*(i-(1<<d));
  }
  inline bool hantei(const Point<T>&a,const Point<T>&b,const Point<T>&c,const Point<T>&d,T midx){
    if(((b.y-a.y)*(midx-a.x)+(a.y)*(b.x-a.x))*(d.x-c.x)<((d.y-c.y)*(midx-c.x)+(c.y)*(d.x-c.x))*(b.x-a.x))return true;
    return false;
  }
  inline void update(int i){
    int lnd=i*2,rnd=i*2+1;
    if(seg[lnd].first==-1||seg[rnd].first==-1){
      if(seg[lnd].first==-1&&seg[rnd].first==-1)seg[i].first=-1;
      else if(seg[lnd].first==-1)seg[i]=seg[rnd];
      else seg[i]=seg[lnd];
      return;
    }
    T midx=p[getl(rnd)].x;
    while(lnd<n||rnd<n){
      if(lnd<n){
        if(seg[lnd*2].first==-1){
          lnd=lnd*2+1;
          continue;
        }
        if(seg[lnd*2+1].first==-1){
          lnd=lnd*2;
          continue;
        }
      }
      if(rnd<n){
        if(seg[rnd*2].first==-1){
          rnd=rnd*2+1;
          continue;
        }
        if(seg[rnd*2+1].first==-1){
          rnd=rnd*2;
          continue;
        }
      }
      if(seg[lnd].first!=seg[lnd].second&&!convex(p[seg[lnd].first],p[seg[lnd].second],p[seg[rnd].first]))lnd=lnd*2;
      else if(seg[rnd].first!=seg[rnd].second&&!convex(p[seg[lnd].second],p[seg[rnd].first],p[seg[rnd].second]))rnd=rnd*2+1;
      else if(seg[lnd].first==seg[lnd].second)rnd=rnd*2;
      else if(seg[rnd].first==seg[rnd].second)lnd=lnd*2+1;
      else{
        if(hantei(p[seg[lnd].first],p[seg[lnd].second],p[seg[rnd].first],p[seg[rnd].second],midx))lnd=lnd*2+1;
        else rnd=rnd*2;
      }
    }
    seg[i]=std::make_pair(lnd-n,rnd-n);
  }
  DynamicLowerHull(std::vector<Point<T>>init):p(std::move(init)){
    n=ceil_pow2(p.size());
    log2n=msb(n);
    seg.resize(n*2);
    lazy.resize(n*2);
    for(int i=0;i<(int)p.size();i++)seg[i+n]=std::make_pair(i,i);
    std::fill(seg.begin()+n+p.size(),seg.end(),std::make_pair(-1,-1));
    for(int i=n-1;i>=1;i--){
      update(i);
    }
  }
  void erase(int i){
    i+=seg.size()>>1;
    seg[i].first=-1;
    while(i>>=1)lazy[i]=true;
  }
  void dfs(int x,int l,int r,std::vector<int>&res){
    if(x>=n){
      if(l<=x-n&&x-n<r)res.push_back(x-n);
      return;
    }
    if(seg[x*2].first==-1)dfs(x*2+1,l,r,res);
    else if(seg[x*2+1].first==-1)dfs(x*2,l,r,res);
    else if(seg[x].second<=l)dfs(x*2+1,l,r,res);
    else if(seg[x].first>=r)dfs(x*2,l,r,res);
    else{
      dfs(x*2,l,seg[x].first+1,res);
      dfs(x*2+1,seg[x].second,r,res);
    }
  }
  void dfs2(int x){
    if(lazy[x]){
      dfs2(x*2);
      dfs2(x*2+1);
      update(x);
      lazy[x]=false;
    }
  }
  std::vector<int>convex_hull(){
    dfs2(1);
    std::vector<int>res;
    dfs(1,0,p.size(),res);
    return res;
  }
};
template<typename T>
std::vector<int>convex_layers(const std::vector<Point<T>>&p){
  std::vector<int>ord(p.size());
  std::iota(ord.begin(),ord.end(),0);
  std::sort(ord.begin(),ord.end(),[&](int i,int j){return p[i].x!=p[j].x?p[i].x<p[j].x:p[i].y<p[j].y;});
  std::vector<Point<T>>init(p.size());
  for(int i=0;i<(int)p.size();i++)init[i]=p[ord[i]];
  DynamicLowerHull<T>lower(init);
  for(int i=0;i<(int)p.size();i++){
    init[i]=p[ord[p.size()-i-1]];
    init[i].x*=-1;
    init[i].y*=-1;
  }
  DynamicLowerHull<T>upper(init);
  std::vector<int>res(p.size());
  for(int i=1,cnt=0;cnt<(int)p.size();i++){
    auto cl=lower.convex_hull();
    auto cu=upper.convex_hull();
    if(cnt+(int)cl.size()==(int)p.size()){
      for(int v:cl)res[ord[v]]=i;
      break;
    }
    for(int v:cl){
      res[ord[v]]=i;
      lower.erase(v);
      upper.erase(p.size()-v-1);
    }
    for(int j=1;j<(int)cu.size()-1;j++){
      res[ord[p.size()-cu[j]-1]]=i;
      lower.erase(p.size()-cu[j]-1);
      upper.erase(cu[j]);
    }
    cnt+=cl.size()+cu.size()-2;
  }
  return res;
}
}
using convex_layers_impl::convex_layers;
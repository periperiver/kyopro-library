#pragma once
#include "graph_base.hpp"
#include "../datastructure/csr_array.hpp"
#include<cassert>
#include<limits>
#include<concepts>
#include<queue>
namespace negative_sssp_impl{
template<typename T>
struct Q{
  int p,q;
  std::vector<T>dat;
  Q():p(0),q(0){}
  Q(int n):p(0),q(0),dat(n){}
  void push(const T&x){dat[q++]=x;}
  const T&pop(){return dat[p++];}
  void clear(){p=q=0;}
};
template<typename T>
struct S{
  int p;
  std::vector<T>dat;
  S():p(0){}
  S(int n):p(0),dat(n){}
  void push(const T&x){dat[p++]=x;}
  const T&pop(){return dat[--p];}
  void clear(){p=0;}
};
template<std::integral T>
struct ShortestPath{
  Graph<T>g,grev;
  std::vector<T>potential;
  std::vector<int>deg;
  Q<int>que;
  S<int>st;
  T eps;
  std::vector<int>scc;
  ShortestPath(Graph<T>g_):g(std::move(g_)),potential(g.size()),deg(g.size()),que(g.size()),st(g.edge_size()+1),eps(1),scc(g.size()){
    grev=g;
    grev.reverse();
  }
  bool topological_sort(){
    for(const Edge<T>&e:g){
      T cost=e.weight+potential[e.from]-potential[e.to];
      if(cost<0)deg[e.to]++;
    }
    que.clear();
    for(int i=0;i<g.size();i++)if(deg[i]==0)que.push(i);
    while(que.p<que.q){
      int x=que.pop();
      for(const Edge<T>&e:g[x]){
        T cost=e.weight+potential[e.from]-potential[e.to];
        if(cost<0){
          if(!--deg[e.to])que.push(e.to);
        }
      }
    }
    return que.q<g.size();
  }
  bool refine(){
    std::vector<int>pre(g.size()),cnt(g.size()),dp(g.size());
    std::vector<bool>reachable(g.size());
    std::vector<int>path;
    path.reserve(g.size());
    std::vector<std::vector<int>>dial(g.size());
    std::vector<int>dst(g.size());
    while(true){
      if(topological_sort())return true;
      std::fill(pre.begin(),pre.end(),-1);
      std::fill(cnt.begin(),cnt.end(),0);
      std::fill(dp.begin(),dp.end(),0);
      for(int x:que.dat){
        cnt[dp[x]]++;
        for(const Edge<T>&e:g[x]){
          T cost=e.weight+potential[e.from]-potential[e.to];
          if(cost>=0)continue;
          int nxt=dp[x]+(cost<-eps);
          if(dp[e.to]<nxt){
            dp[e.to]=nxt;
            pre[e.to]=x;
          }
        }
      }
      int ecidx=std::max_element(dp.begin(),dp.end())-dp.begin();
      if(dp[ecidx]==0)break;
      int cridx=std::max_element(cnt.begin()+1,cnt.end())-cnt.begin();
      if(cnt[cridx]>=dp[ecidx]){//cut relabel
        std::fill(reachable.begin(),reachable.end(),false);
        for(int i=0;i<g.size();i++)if(dp[i]==cridx)reachable[i]=true;
        for(int x:que.dat)if(reachable[x]){
          for(const Edge<T>&e:g[x]){
            T cost=e.weight+potential[e.from]-potential[e.to];
            if(cost<0)reachable[e.to]=true;
          }
        }
        for(int i=0;i<g.size();i++)if(reachable[i])potential[i]-=eps;
      }
      else{//eliminate chain
        path.clear();
        path.push_back(ecidx);
        while(pre[ecidx]!=-1){
          ecidx=pre[ecidx];
          path.push_back(ecidx);
        }
        std::reverse(path.begin(),path.end());
        for(std::vector<int>&d:dial)d.clear();
        std::fill(dst.begin(),dst.end(),g.size());
        for(int i=0;i<(int)path.size()-1;i++){
          dst[path[i+1]]=dst[path[i]]-(dp[path[i]]!=dp[path[i+1]]);
          if(dst[path[i+1]]<g.size())dial[dst[path[i+1]]].push_back(path[i+1]);
        }
        for(int i=0;i<(int)dial.size();i++){
          for(int j=0;j<(int)dial[i].size();j++){
            int x=dial[i][j];
            if(dst[x]!=i)continue;
            for(const Edge<T>&e:g[x]){
              T cost=e.weight+potential[e.from]-potential[e.to];
              int w;
              if(cost<0)w=0;
              else if(cost/eps<=g.size())w=cost/eps+1;
              else continue;
              if(dst[e.to]>dst[x]+w){
                dst[e.to]=dst[x]+w;
                dial[dst[e.to]].push_back(e.to);
              }
            }
          }
        }
        for(int i=0;i<g.size();i++)potential[i]+=dst[i]*eps;
      }
    }
    return false;
  }
  bool decycle(){
    std::vector<bool>seen(g.size(),false);
    std::vector<int>vis;
    vis.reserve(g.size());
    st.clear();
    for(int i=0;i<g.size();i++)if(!seen[i]){
      st.push(i);
      while(st.p){
        int x=st.pop();
        if(x<0){
          vis.push_back(~x);
          continue;
        }
        if(seen[x])continue;
        seen[x]=true;
        st.push(~x);
        for(const Edge<T>&e:g[x])if(!seen[e.to]){
          T cost=e.weight+potential[e.from]-potential[e.to];
          if(cost<=0)st.push(e.to);
        }
      }
    }
    std::fill(scc.begin(),scc.end(),-1);
    int sp=0;
    for(int i=g.size()-1;i>=0;i--)if(scc[vis[i]]==-1){
      st.push(vis[i]);
      scc[vis[i]]=sp;
      while(st.p){
        int x=st.pop();
        for(const Edge<T>&e:grev[x]){
          T cost=e.weight-potential[e.from]+potential[e.to];
          if(cost==0){
            if(scc[e.to]==-1){
              scc[e.to]=sp;
              st.push(e.to);
            }
          }
          else if(cost==-1){
            if(scc[e.to]==-1)return true;
          }
        }
      }
      sp++;
    }
    for(const Edge<T>&e:g){
      T cost=e.weight+potential[e.from]-potential[e.to];
      if(cost==-1&&scc[e.from]==scc[e.to])return true;
    }
    return false;
  }
  bool refine1(){
    csr_array<std::pair<int,bool>>h;
    std::vector<std::pair<int,std::pair<int,bool>>>edges;
    edges.reserve(g.edge_size());
    std::vector<int>pre(g.size()),cnt(g.size()),dp(g.size());
    std::vector<bool>reachable(g.size());
    std::vector<int>path;
    path.reserve(g.size());
    std::vector<std::vector<int>>dial(g.size());
    std::vector<int>dst(g.size());
    while(true){
      if(decycle())return true;
      edges.clear();
      int n=*std::max_element(scc.begin(),scc.end())+1;
      for(const Edge<T>&e:g)if(scc[e.from]!=scc[e.to]){
        T cost=e.weight+potential[e.from]-potential[e.to];
        if(cost<=0)edges.emplace_back(scc[e.from],std::make_pair(scc[e.to],cost==-1));
      }
      h=csr_array<std::pair<int,bool>>(n,edges);
      std::fill(pre.begin(),pre.begin()+n,-1);
      std::fill(cnt.begin(),cnt.begin()+n,0);
      std::fill(dp.begin(),dp.begin()+n,0);
      for(int i=0;i<n;i++){
        cnt[dp[i]]++;
        for(const auto&[v,c]:h[i]){
          int nxt=dp[i]+c;
          if(dp[v]<nxt){
            dp[v]=nxt;
            pre[v]=i;
          }
        }
      }
      int ecidx=std::max_element(dp.begin(),dp.begin()+n)-dp.begin();
      if(dp[ecidx]==0)break;
      int cridx=std::max_element(cnt.begin()+1,cnt.begin()+n)-cnt.begin();
      if(cnt[cridx]>=dp[ecidx]){//cut relabel
        std::fill(reachable.begin(),reachable.begin()+n,false);
        for(int i=0;i<n;i++){
          if(dp[i]==cridx)reachable[i]=true;
          if(reachable[i]){
            for(const auto&[v,c]:h[i])reachable[v]=true;
          }
        }
        for(int i=0;i<g.size();i++)if(reachable[scc[i]])potential[i]--;
      }
      else{//eliminate chain
        path.clear();
        path.push_back(ecidx);
        while(pre[ecidx]!=-1){
          ecidx=pre[ecidx];
          path.push_back(ecidx);
        }
        std::reverse(path.begin(),path.end());
        std::fill(dst.begin(),dst.begin()+n,n);
        for(std::vector<int>&d:dial)d.clear();
        for(int i=0;i<(int)path.size()-1;i++){
          dst[path[i+1]]=dst[path[i]]-(dp[path[i]]!=dp[path[i+1]]);
          if(dst[path[i+1]]<n)dial[dst[path[i+1]]].push_back(path[i+1]);
        }
        std::vector<std::pair<int,std::pair<int,int>>>edges2;
        for(const Edge<T>&e:g)if(scc[e.from]!=scc[e.to]){
          T cost=e.weight+potential[e.from]-potential[e.to];
          if(cost<n)edges2.emplace_back(scc[e.from],std::make_pair(scc[e.to],std::max<int>(cost,0)));
        }
        csr_array<std::pair<int,int>>h2(n,edges2);
        for(int i=0;i<n;i++){
          for(int j=0;j<(int)dial[i].size();j++){
            int x=dial[i][j];
            if(dst[x]!=i)continue;
            for(const auto&[v,c]:h2[x]){
              if(dst[v]>i+c){
                dst[v]=i+c;
                dial[dst[v]].push_back(v);
              }
            }
          }
        }
        for(int i=0;i<g.size();i++){
          potential[i]+=dst[scc[i]];
        }
      }
    }
    return false;
  }
  bool scale(){
    for(const Edge<T>&e:g){
      while(eps<-e.weight)eps<<=1;
    }
    while(eps>1){
      eps>>=1;
      if(refine())return true;
    }
    if(refine1())return true;
    return false;
  }
  std::pair<std::vector<T>,std::vector<int>>solve(int s){
    if(scale())return {{},{}};
    std::vector<int>pre(g.size(),-1);
    std::vector<T>dst(g.size(),std::numeric_limits<T>::max());
    dst[s]=0;
    std::priority_queue<std::pair<T,int>,std::vector<std::pair<T,int>>,std::greater<std::pair<T,int>>>pq;
    pq.emplace(0,s);
    while(!pq.empty()){
      auto[d,x]=pq.top();
      pq.pop();
      if(dst[x]!=d)continue;
      for(const Edge<T>&e:g[x]){
        T cost=e.weight+potential[e.from]-potential[e.to];
        if(dst[e.to]>d+cost){
          dst[e.to]=d+cost;
          pre[e.to]=x;
          pq.emplace(dst[e.to],e.to);
        }
      }
    }
    for(int i=0;i<(int)dst.size();i++)if(dst[i]!=std::numeric_limits<T>::max()){
      dst[i]=dst[i]-potential[s]+potential[i];
    }
    return std::make_pair(dst,pre);
  }
};
template<typename T>
std::pair<std::vector<T>,std::vector<int>>negative_sssp(Graph<T>g,int s){
  static_assert(std::is_signed_v<T>);
  static_assert(std::is_integral_v<T>);
  assert(g.is_directed());
  return ShortestPath(std::move(g)).solve(s);
}
}
using negative_sssp_impl::negative_sssp;
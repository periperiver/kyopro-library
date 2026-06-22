#pragma once
#include<vector>
#include<limits>
#include<numeric>
#include "datastructure/csr_array.hpp"
template<typename T>
struct Dinic{
private:
  struct edge{
    int to,rev;
    T cap;
  };
  int n;
  csr_array<edge>g;
  std::vector<std::pair<int,edge>>init_csr;
  std::vector<std::pair<int,int>>pos;
  std::vector<int>deg;
  template<typename T2>
  struct Q{
    std::vector<T2>data;
    int p;
    Q():p(0){}
    void push(T2 x){
      data.push_back(x);
    }
    int front()const{
      return data[p];
    }
    void pop(){p++;}
    void clear(){
      data.clear();
      p=0;
    }
    bool empty()const{
      return p==data.size();
    }
  };
public:
  Dinic():n(0){}
  Dinic(int n):n(n),deg(n){}
  void add_edge(int u,int v,T cap){
    pos.push_back({u,deg[u]});
    int f=deg[u];
    int t=deg[v];
    if(u==v)t++;
    init_csr.emplace_back(u,edge{v,t,cap});
    init_csr.emplace_back(v,edge{u,f,0});
    deg[u]++,deg[v]++;
  }
  struct E{
    int from,to;
    T cap,flow;
  };
  std::vector<E>edges()const{
    int m=pos.size();
    std::vector<E>ret(m);
    for(int i=0;i<m;i++){
      edge e=g[pos[i].first][pos[i].second];
      edge r=g[e.to][e.rev];
      ret[i]={pos[i].first,e.to,e.cap+r.cap,r.cap};
    }
    return ret;
  }
  T flow(int s,int t,T limit){
    g=csr_array<edge>(n,init_csr);
    std::vector<int>level(n),iter(n);
    Q<int>que;
    auto bfs=[&](){
      std::fill(level.begin(),level.end(),-1);
      level[s]=0;
      que.clear();
      que.push(s);
      while(!que.empty()){
        int x=que.front();
        que.pop();
        for(auto e:g[x]){
          if(e.cap==0||level[e.to]>=0)continue;
          level[e.to]=level[x]+1;
          if(e.to==t)return;
          que.push(e.to);
        }
      }
    };
    auto dfs=[&](auto self,int v,T up)->T {
      if(v==s)return up;
      T ret=0;
      int lv=level[v];
      for(int &i=iter[v];i<(int)g[v].size();i++){
        edge e=g[v][i];
        if(lv<=level[e.to]||g[e.to][e.rev].cap==0)continue;
        T d=self(self,e.to,std::min(up-ret,g[e.to][e.rev].cap));
        if(d<=0)continue;
        g[v][i].cap+=d;
        g[e.to][e.rev].cap-=d;
        ret+=d;
        if(ret==up)return ret;
      }
      level[v]=n;
      return ret;
    };
    T flow=0;
    while(flow<limit){
      bfs();
      if(level[t]==-1)break;
      std::fill(iter.begin(),iter.end(),0);
      T f=dfs(dfs,t,limit-flow);
      if(!f)break;
      flow+=f;
    }
    return flow;
  }
  T flow(int s,int t){
    return flow(s,t,std::numeric_limits<T>::max());
  }
  std::vector<bool>min_cut(int s){
    std::vector<bool>ret(n,false);
    Q<int>que;
    que.push(s);
    while(!que.empty()){
      int x=que.front();
      que.pop();
      ret[x]=true;
      for(auto e:g[x]){
        if(e.cap&&!ret[e.to]){
          ret[e.to]=true;
          que.push(e.to);
        }
      }
    }
    return ret;
  }
};
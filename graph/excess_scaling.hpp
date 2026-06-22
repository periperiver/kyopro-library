#pragma once
#include<concepts>
#include<queue>
#include<limits>
#include<cassert>
#include "../datastructure/csr_array.hpp"
#include "../math/util.hpp"
template<std::integral T>
struct ExcessScaling{
private:
  struct edge{
    int to,rev;
    T cap;
  };
  struct MultiQueue{
    std::vector<std::queue<int>>que;
    int pos;
    MultiQueue(int n):que(n),pos(0){}
    void push(int i,int x){
      que[i].push(x);
      if(pos<i)pos=i;
    }
    void fit(){
      while(pos>=0&&que[pos].empty())pos--;
    }
    int get(){return que[pos].front();}
  };
  int n,s,t;
  std::vector<int>level;
  csr_array<edge>g;
  std::vector<std::pair<int,int>>pos;
  std::vector<std::pair<int,edge>>init_csr;
  std::vector<T>e;
  T delta;
  MultiQueue mque;
  void push(edge&x){
    edge&rev=g[x.to][x.rev];
    T c=std::min(x.cap,e[rev.to]);
    if(c==0)return;
    if(x.to!=s&&x.to!=t&&e[x.to]<delta&&e[x.to]+c>=delta){
      mque.push(level[x.to],x.to);
    }
    e[rev.to]-=c;
    e[x.to]+=c;
    x.cap-=c;
    rev.cap+=c;
  }
  void relabel(int x){
    int mn=n*2;
    for(const edge&v:g[x])if(v.cap>0&&mn>level[v.to])mn=level[v.to];
    level[x]=mn+1;
    mque.push(level[x],x);
  }
public:
  ExcessScaling(){}
  explicit ExcessScaling(int n_,int s_,int t_):n(n_),s(s_),t(t_),level(n),e(n),delta(0),mque(n*2){
    assert(0<=s&&s<n);
    assert(0<=t&&t<n);
  }
  void add_edge(int u,int v,T cap){
    assert(0<=cap);
    int d=level[v];
    pos.emplace_back(u,level[u]);
    init_csr.emplace_back(u,edge{v,d,cap});
    d=level[u];
    level[u]++;
    init_csr.emplace_back(v,edge{u,d,0});
    level[v]++;
  }
  T flow(){
    g=csr_array<edge>(n,init_csr);
    delta=[&](){
      std::vector<T>sum(n);
      for(const edge&x:g[s])sum[x.to]+=x.cap;
      return ceil_pow2(*std::max_element(sum.begin(),sum.end()));
    }();
    std::fill(level.begin(),level.end(),-1);
    std::queue<int>que;
    que.push(t);
    level[t]=0;
    while(!que.empty()){
      int x=que.front();que.pop();
      for(const edge&e:g[x])if(g[e.to][e.rev].cap>0&&level[e.to]==-1){
        level[e.to]=level[x]+1;
        que.push(e.to);
      }
    }
    for(int&x:level)if(x==-1)x=0;
    level[s]=n;
    e[s]=std::numeric_limits<T>::max();
    for(edge&x:g[s])push(x);
    while(delta>=1){
      mque.fit();
      if(mque.pos==-1){
        delta/=2;
        if(delta==0)break;
        std::fill(level.begin(),level.end(),-1);
        level[t]=0;
        que.push(t);
        while(!que.empty()){
          int x=que.front();que.pop();
          for(const edge&e:g[x])if(g[e.to][e.rev].cap>0&&level[e.to]==-1){
            level[e.to]=level[x]+1;
            que.push(e.to);
          }
        }
        for(int&x:level)if(x==-1)x=0;
        level[s]=n;
        for(int i=0;i<n;i++)if(i!=s&&i!=t&&e[i]>=delta){
          mque.push(level[i],i);
        }
        continue;
      }
      int npos=mque.pos;
      int x=mque.get();
      if(e[x]<delta){
        mque.que[npos].pop();
        continue;
      }
      bool ok=false;
      for(edge&v:g[x])if(v.cap>0&&level[v.to]+1==level[x]){
        push(v);
        ok=true;
      }
      if(!ok)relabel(x);
      if(e[x]<delta)mque.que[npos].pop();
    }
    return e[t];
  }
  struct E{
    int from,to;
    T cap,flow;
  };
  std::vector<E>edges(){
    std::vector<E>res;
    res.reserve(pos.size());
    for(auto [i,j]:pos){
      edge v=g[i][j];
      res.push_back({i,v.to,v.cap+g[v.to][v.rev].cap,g[v.to][v.rev].cap});
    }
    return res;
  }
};
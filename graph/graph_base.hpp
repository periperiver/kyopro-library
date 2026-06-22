#pragma once
#include "edge_base.hpp"
#include<iostream>
#include<vector>
#include<cassert>
template<typename T=int>
struct Graph{
private:
  int n;
  std::vector<Edge<T>>edge;
  std::vector<Edge<T>>g;
  std::vector<int>ptr;
  bool directed;
  struct graph_range{
    using iterator=typename std::vector<Edge<T>>::iterator;
    iterator l,r;
    iterator begin()const{return l;}
    iterator end()const{return r;}
    int size()const{return r-l;}
    Edge<T> &operator[](int i)const{return l[i];}
  };
  struct const_graph_range{
    using iterator=typename std::vector<Edge<T>>::const_iterator;
    iterator l,r;
    iterator begin()const{return l;}
    iterator end()const{return r;}
    int size()const{return r-l;}
    const Edge<T> &operator[](int i)const{return l[i];}
  };
public:
  Graph(int n_,bool dir_):n(n_),directed(dir_){}
  Graph():n(0){}
  Graph(int n_,bool dir_,const std::vector<Edge<T>>&e):n(n_),directed(dir_),edge(e){build();}
  template<bool weighted=false,bool index=1>
  void read(int m){
    edge.reserve(m);
    for(int i=0;i<m;i++){
      int u,v;
      std::cin>>u>>v;
      T w;
      if constexpr(index)u--,v--;
      if constexpr(weighted)std::cin>>w;
      else w=1;
      edge.emplace_back(u,v,w,i);
    }
    build();
  }
  void add_edge(int u,int v){
    assert(0<=u&&u<n);
    assert(0<=v&&v<n);
    int id=edge.size();
    edge.emplace_back(u,v,1,id);
  }
  void add_edge(int u,int v,T w){
    assert(0<=u&&u<n);
    assert(0<=v&&v<n);
    int id=edge.size();
    edge.emplace_back(u,v,w,id);
  }
  void add_edge(int u,int v,T w,int index){
    assert(0<=u&&u<n);
    assert(0<=v&&v<n);
    edge.emplace_back(u,v,w,index);
  }
  void add_edge(Edge<T>e){
    edge.emplace_back(e);
  }
  void build(){
    std::vector<int>cnt(n+1,0);
    for(const Edge<T>&e:edge){
      cnt[e.from+1]++;
      if(!directed)cnt[e.to+1]++;
    }
    for(int i=1;i<=n;i++)cnt[i]+=cnt[i-1];
    ptr=cnt;
    g.resize(cnt[n]);
    for(const Edge<T>&e:edge){
      g[cnt[e.from]++]=e;
      if(!directed)g[cnt[e.to]++]=Edge<T>(e.to,e.from,e.weight,e.index);
    }
  }
  void reverse(){
    if(directed){
      for(Edge<T>&e:edge)std::swap(e.from,e.to);
      build();
    }
  }
  inline void to_directed(){
    directed=true;
    build();
  }
  inline void to_undirected(){
    directed=false;
    build();
  }
  void reserve(int m){edge.reserve(m);}
  graph_range operator[](int i){return graph_range{g.begin()+ptr[i],g.begin()+ptr[i+1]};}
  const_graph_range operator[](int i)const{return const_graph_range{g.begin()+ptr[i],g.begin()+ptr[i+1]};}
  const Edge<T>& get_edge(int i)const{return edge[i];}
  std::vector<Edge<T>>get_edges()const{return edge;}
  inline bool is_directed()const{return directed;}
  inline int size()const{return n;}
  inline int edge_size()const{return edge.size();}
  typename std::vector<Edge<T>>::iterator begin(){return edge.begin();}
  typename std::vector<Edge<T>>::iterator end(){return edge.end();}
  typename std::vector<Edge<T>>::const_iterator begin()const{return edge.begin();}
  typename std::vector<Edge<T>>::const_iterator end()const{return edge.end();}
};
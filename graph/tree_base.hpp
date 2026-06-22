#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<cassert>
#include<algorithm>
#include<limits>
#include "edge_base.hpp"
template<typename T=int>
struct Tree{
  int n,r;
  std::vector<Edge<T>>edge;
  std::vector<Edge<T>>g;
  std::vector<int>ptr;
  struct tree_range{
    using iterator=typename std::vector<Edge<T>>::iterator;
    iterator l,r;
    iterator begin()const{return l;}
    iterator end()const{return r;}
    int size()const{return r-l;}
    bool empty()const{return !size();}
    Edge<T> &operator[](int i)const{return l[i];}
  };
  struct const_tree_range{
    using iterator=typename std::vector<Edge<T>>::const_iterator;
    iterator l,r;
    iterator begin()const{return l;}
    iterator end()const{return r;}
    int size()const{return r-l;}
    bool empty()const{return !size();}
    const Edge<T> &operator[](int i)const{return l[i];}
  };
  explicit Tree(int n_):n(n_){
    edge.reserve(n-1);
  }
  Tree():n(0){}
  Tree(int n_,const std::vector<Edge<T>>&e,bool dir=false):n(n_),r(-1),edge(e){
    if(!dir)build();
    else{
      std::vector<bool>seen(n,false);
      ptr.resize(n+1);
      for(const Edge<T>&i:edge)ptr[i.from]++,ptr[i.to]++,seen[i.to]=true;
      for(int i=1;i<=n;i++)ptr[i]+=ptr[i-1];
      r=std::find(seen.begin(),seen.end(),false)-seen.begin();
      assert(ptr[n]==n*2-2);
      g.resize(ptr[n]);
      for(const Edge<T>&i:edge)g[--ptr[i.to]]=Edge<T>(i.to,i.from,i.weight,i.index);
      for(const Edge<T>&i:edge)g[--ptr[i.from]]=i;
    }
  }
  template<bool weighted=false,bool index=1>
  void read(){
    for(int i=0;i<n-1;i++){
      int u,v;
      T w=T();
      std::cin>>u>>v;
      if constexpr(index)u--,v--;
      if constexpr(weighted)std::cin>>w;
      else w=1;
      edge.emplace_back(u,v,w,i);
    }
    build();
  }
  template<bool index=1>
  void readp(){
    ptr.resize(n+1);
    for(int i=1;i<n;i++){
      int p;
      std::cin>>p;
      if constexpr(index)p--;
      edge.emplace_back(p,i,1,i-1);
      ptr[p]++;
      ptr[i]++;
    }
    for(int i=1;i<=n;i++)ptr[i]+=ptr[i-1];
    g.resize(n*2-2);
    for(auto&&[u,v,w,i]:edge)g[--ptr[v]]=Edge<T>(v,u,w,i);
    for(int i=0;i<n-1;i++){
      g[--ptr[edge[i].from]]=edge[i];
    }
    r=0;
  }
  void add_edge(int u,int v){edge.emplace_back(u,v,1,edge.size());}
  void add_edge(int u,int v,T w){edge.emplace_back(u,v,w,edge.size());}
  void add_edge(int u,int v,T w,int idx){edge.emplace_back(u,v,w,idx);}
  inline bool is_directed()const{return r!=-1;}
  void build(){
    r=-1;
    ptr.assign(n+1,0);
    for(auto&&[u,v,w,i]:edge)ptr[u]++,ptr[v]++;
    for(int i=1;i<=n;i++)ptr[i]+=ptr[i-1];
    assert(ptr[n]==n*2-2);
    g.resize(n*2-2);
    for(auto&&[u,v,w,i]:edge){
      g[--ptr[u]]=Edge(u,v,w,i);
      g[--ptr[v]]=Edge(v,u,w,i);
    }
  }
  void remove_parent(int root=0){
    if(r!=-1){
      if(r!=root)build();
      else return;
    }
    edge.resize(n-1);
    std::vector<int>par(n,-1);
    par[root]=-1;
    std::queue<int>que;
    que.push(root);
    while(!que.empty()){
      int x=que.front();
      que.pop();
      for(int i=ptr[x];i<ptr[x+1];){
        const Edge<T>&e=g[i];
        if(e.to!=par[x]){
          par[e.to]=x;
          assert(e.index<n-1);
          edge[e.index]=e;
          que.push(e.to);
          i++;
        }
        else{
          if(i+1==ptr[x+1])break;
          std::swap(g[i],g[ptr[x+1]-1]);
        }
      }
    }
    r=root;
  }
  void re_index(){
    assert((int)edge.size()==n-1);
    for(int i=0;i<n-1;i++)edge[i].index=i;
    build();
    if(r!=-1)remove_parent(r);
  }
  std::vector<int>bfs_order()const{
    assert(is_directed());
    std::vector<int>bfs(n);
    int p=0,q=0;
    bfs[q++]=root();
    while(p<q){
      int x=bfs[p++];
      for(const Edge<T>&e:(*this)[x])bfs[q++]=e.to;
    }
    return bfs;
  }
  std::vector<int>dfs_order()const{
    assert(is_directed());
    std::vector<int>res;
    res.reserve(n);
    std::vector<int>st(n);
    int p=0;
    st[p++]=root();
    while(p){
      int x=st[--p];
      res.push_back(x);
      p+=(*this)[x].size();
      for(const Edge<T>&e:(*this)[x])st[--p]=e.to;
      p+=(*this)[x].size();
    }
    return res;
  }
  std::vector<int>rbfs_order()const{
    std::vector<int>bfs=bfs_order();
    std::reverse(bfs.begin(),bfs.end());
    return bfs;
  }
  void hld(){
    assert(is_directed());
    std::vector<int>sub(n);
    for(int x:rbfs_order()){
      sub[x]=1;
      int mx=-1;
      for(Edge<T>&e:(*this)[x]){
        sub[x]+=sub[e.to];
        if(mx<sub[e.to]){
          mx=sub[e.to];
          std::swap((*this)[x][0],e);
        }
      }
    }
  }
  std::pair<std::vector<int>,std::vector<int>>in_out_order(){
    assert(is_directed());
    std::vector<int>in(n),out(n);
    int p=0;
    auto dfs=[&](auto self,int x)->void {
      in[x]=p++;
      for(const Edge<T>&e:(*this)[x]){
        self(self,e.to);
      }
      out[x]=p;
    };
    dfs(dfs,root());
    return std::make_pair(in,out);
  }
  std::pair<T,std::vector<int>>diameter()const{
    assert(!is_directed());
    static constexpr T inf=std::numeric_limits<T>::max();
    std::vector<T>dst(n,inf);
    dst[0]=0;
    std::vector<int>que(n);
    int p=0,q=1;
    que[0]=0;
    while(p<q){
      int x=que[p++];
      for(const Edge<T>&e:(*this)[x])if(dst[e.to]==inf){
        dst[e.to]=dst[x]+e.weight;
        que[q++]=e.to;
      }
    }
    int u=std::max_element(dst.begin(),dst.end())-dst.begin();
    std::fill(dst.begin(),dst.end(),inf);
    dst[u]=0;
    p=0,q=1;
    que[0]=u;
    while(p<q){
      int x=que[p++];
      for(const Edge<T>&e:(*this)[x])if(dst[e.to]==inf){
        dst[e.to]=dst[x]+e.weight;
        que[q++]=e.to;
      }
    }
    int v=std::max_element(dst.begin(),dst.end())-dst.begin();
    T weight=dst[v];
    std::vector<int>res;
    while(u!=v){
      res.push_back(v);
      for(const Edge<T>&e:(*this)[v])if(dst[e.to]<dst[v]){
        v=e.to;
        break;
      }
    }
    res.push_back(u);
    return std::make_pair(weight,res);
  }
  int size()const{return n;}
  tree_range operator[](int i){return tree_range{g.begin()+ptr[i],g.begin()+ptr[i+1]-(r!=-1&&r!=i)};}
  const_tree_range operator[](int i)const{return const_tree_range{g.begin()+ptr[i],g.begin()+ptr[i+1]-(r!=-1&&r!=i)};}
  const Edge<T>& get_edge(int i)const{return edge[i];}
  inline int parent(int i)const{return i==r?-1:g[ptr[i+1]-1].to;}
  const Edge<T>& parent_edge(int i)const{
    assert(r!=-1&&i!=r);
    return g[ptr[i+1]-1];
  }
  inline int root()const{return r;}
  typename std::vector<Edge<T>>::iterator begin(){return edge.begin();}
  typename std::vector<Edge<T>>::iterator end(){return edge.end();}
  typename std::vector<Edge<T>>::const_iterator begin()const{return edge.begin();}
  typename std::vector<Edge<T>>::const_iterator end()const{return edge.end();}
};
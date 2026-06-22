#pragma once
#include<vector>
#include<cassert>
#include "../datastructure/dynamic/toptree.hpp"
template<typename DP>
struct TopTreeDP{
private:
  using Data=typename DP::Data;
  using Path=typename DP::Path;
  using Point=typename DP::Point;
  struct node{
    node *left,*right,*par,*middle;
    bool vertex,rev;
    Data d;
    Path sum,mus;
    Point p;
    node():left(nullptr),right(nullptr),par(nullptr),middle(nullptr),vertex(true),rev(false){}
    node(Data d):left(nullptr),right(nullptr),par(nullptr),middle(nullptr),vertex(true),rev(false),d(d),sum(d),mus(d),p(DP::add_edge(d)){}
    void reverse(){
      rev^=1;
      std::swap(sum,mus);
      std::swap(left,right);
    }
    void push(){
      if(rev){
        if(left)left->reverse();
        if(right)right->reverse();
      }
      rev=false;
    }
    void update(){
      if(vertex){
        if(middle)sum=mus=DP::add_vertex(middle->p,d);
        else sum=mus=d;
        if(left)sum=DP::compress(left->sum,sum),mus=DP::compress(mus,left->mus);
        if(right)sum=DP::compress(sum,right->sum),mus=DP::compress(right->mus,mus);
      }
      else{
        p=DP::add_edge(middle->sum);
        if(left)p=DP::rake(left->p,p);
        if(right)p=DP::rake(p,right->p);
      }
    }
  };
  std::vector<node>nds;
public:
  TopTreeDP(){}
  explicit TopTreeDP(int n):nds(n){}
  void link(int u,int v){
    assert(0<=u&&u<(int)nds.size());
    assert(0<=v&&v<(int)nds.size());
    assert(u!=v);
    reroot(v);
    tt_expose(&nds[u]);
    nds[u].right=&nds[v];
    nds[v].par=&nds[u];
    nds[u].update();
  }
  //rを根としてuとpar(u)をcut
  void cut(int r,int u){
    assert(0<=r&&r<(int)nds.size());
    assert(0<=u&&u<(int)nds.size());
    assert(r!=u);
    reroot(r);
    tt_expose(&nds[u]);
    assert(nds[u].left);
    node *lnd=nds[u].left;
    lnd->par=nullptr;
    nds[u].left=nullptr;
    nds[u].update();
  }
  void set(int i,Data d){
    assert(0<=i&&i<(int)nds.size());
    tt_expose(&nds[i]);
    nds[i].d=d;
    nds[i].update();
  }
  void reroot(int r){
    assert(0<=r&&r<(int)nds.size());
    tt_expose(&nds[r]);
    nds[r].reverse();
  }
  Path get(int r){
    assert(0<=r&&r<(int)nds.size());
    reroot(r);
    return nds[r].sum;
  }
  Data raw(int v){
    assert(0<=v&&v<(int)nds.size());
    return nds[v].d;
  }
};
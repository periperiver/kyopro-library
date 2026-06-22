#pragma once
#include "template.hpp"
struct PersistentUnionFind{
private:
  struct node{
    int v;
    node* child[16];
    node(){
      rep(i,16)child[i]=nullptr;
    }
  };
  int get(int id)const{
    node *nd=rooot;
    while(id){
      nd=nd->child[(id-1)&15];
      id=(id-1)>>4;
    }
    return nd->v;
  }
  node* set(int id,int v){
    node *nd=rooot;
    node *nx=new node();
    node *ret=nx;
    while(true){
      if(nd){
        rep(i,16)nx->child[i]=nd->child[i];
        nx->v=nd->v;
      }
      if(id==0){
        nx->v=v;
        return ret;
      }
      int nid=(id-1)&15;
      nx->child[nid]=new node();
      nx=nx->child[nid];
      nd=nd->child[nid];
      id=(id-1)>>4;
    }
    return nullptr;
  }
  void set2(int id,int v){
    node *nd=rooot;
    while(id){
      int nid=(id-1)&15;
      if(!nd->child[nid])nd->child[nid]=new node();
      nd=nd->child[nid];
      id=(id-1)>>4;
    }
    nd->v=v;
  }
  node *rooot;
  PersistentUnionFind(node *r):rooot(r){}
public:
  PersistentUnionFind(int n):rooot(new node()){
    rep(i,n)set2(i,-1);
  }
  PersistentUnionFind():rooot(nullptr){}
  int root(int u)const{
    while(true){
      int par=get(u);
      if(par<0)return u;
      else u=par;
    }
    return -1;
  }
  bool same(int u,int v)const{
    return root(u)==root(v);
  }
  int size(int u)const{
    while(true){
      int par=get(u);
      if(par<0)return -par;
      else u=par;
    }
    return -1;
  }
  PersistentUnionFind merge(int u,int v){
    int ru=root(u),rv=root(v);
    if(ru==rv)return *this;
    int su=get(ru),sv=get(rv);
    if(su>sv){
      swap(u,v);
      swap(ru,rv);
      swap(su,sv);
    }
    node *ret=set(ru,su+sv);
    return PersistentUnionFind(PersistentUnionFind(ret).set(rv,ru));
  }
};
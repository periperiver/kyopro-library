#pragma once
#include<limits>
#include<cstdlib>
template<typename T,int dep=std::numeric_limits<T>::digits>
struct PersistentBinaryTrie{
  struct node{
    node *ch[2]={nullptr,nullptr};
    int cnt=0;
  };
  node *root;
  [[nodiscard]]PersistentBinaryTrie insert(T x){
    PersistentBinaryTrie res;
    res.root=new node();
    node *old=this->root,*nw=res.root;
    for(int i=dep-1;i>=0;i--){
      if(old)nw->ch[!(x>>i&1)]=old->ch[!(x>>i&1)];
      nw->cnt=(nw->ch[!(x>>i&1)]?nw->ch[!(x>>i&1)]->cnt:0)+(old&&old->ch[x>>i&1]?old->ch[x>>i&1]->cnt:0)+1;
      nw->ch[x>>i&1]=new node();
      nw=nw->ch[x>>i&1];
      if(old)old=old->ch[x>>i&1];
    }
    nw->cnt=(old?old->cnt:0)+1;
    return res;
  }
  T xor_max(T x){
    T res=0;
    node *nd=this->root;
    if(!nd)std::abort();
    for(int i=dep-1;i>=0;i--){
      if(nd->ch[!(x>>i&1)])res|=T(1)<<i,nd=nd->ch[!(x>>i&1)];
      else nd=nd->ch[x>>i&1];
    }
    return res;
  }
};
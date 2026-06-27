#pragma once
#include<vector>
#include<array>
#include<cassert>
template<typename T>
struct PersistentArray16{
private:
  struct node{
    std::array<node*,16>ch;
    T dat;
    node():dat(){
      ch.fill(nullptr);
    }
  };
  int n;
  node *root;
public:
  PersistentArray16():n(0),root(nullptr){}
  explicit PersistentArray16(std::vector<T>init):n(init.size()),root(new node()){
    for(int i=0;i<n;i++){
      node *nd=root;
      for(int x=i;x>0;x=(x-1)>>4){
        if(!nd->ch[(x-1)&15])nd->ch[(x-1)&15]=new node();
        nd=nd->ch[(x-1)&15];
      }
      nd->dat=init[i];
    }
  }
  PersistentArray16(int n,T init):PersistentArray16(std::vector<T>(n,init)){}
  T get(int i)const{
    assert(0<=i&&i<n);
    node *nd=root;
    while(i>0){
      nd=nd->ch[(i-1)&15];
      i=(i-1)>>4;
    }
    return nd->dat;
  }
  [[nodiscard]]PersistentArray16 set(int i,T v){
    assert(0<=i&&i<n);
    node *nroot=new node();
    node *ndpre=root;
    node *ndnxt=nroot;
    while(i>0){
      ndnxt->ch=ndpre->ch;
      ndnxt->dat=ndpre->dat;
      int ni=(i-1)&15;
      ndnxt->ch[ni]=new node();
      ndnxt=ndnxt->ch[ni];
      ndpre=ndpre->ch[ni];
      i=(i-1)>>4;
    }
    ndnxt->ch=ndpre->ch;
    ndnxt->dat=v;
    PersistentArray16 res;
    res.n=n;
    res.root=nroot;
    return res;
  }
};
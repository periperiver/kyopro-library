#pragma once
#include "template.hpp"
template<typename T>
struct PersistentStack{
private:
  struct node{
    node *pre;
    T v;
    node(T vv):pre(nullptr),v(vv){}
  };
  node *r;
  PersistentStack(node *rr):r(rr){}
public:
  PersistentStack():r(nullptr){}
  T top()const{return r->v;}
  bool empty()const{return !r;}
  PersistentStack push(T v){
    node *nd=new node(v);
    nd->pre=r;
    return PersistentStack(nd);
  }
  PersistentStack pop(){
    return PersistentStack(r->pre);
  }
};
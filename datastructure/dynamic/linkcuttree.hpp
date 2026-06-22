#pragma once
#include "splay.hpp"
template<typename T>
void lct_expose(T*nd){
  while(true){
    splay(nd);
    nd->right=nullptr;
    if constexpr(has_update_v<T>)nd->update();
    if(!nd->par)break;
    splay(nd->par);
    nd->par->right=nd;
    if constexpr(has_update_v<T>)nd->par->update();
  }
}
template<typename T>
void lct_link(T*u,T*v){
  lct_expose(v);
  v->reverse();
  v->par=u;
}
template<typename T>
void lct_cut(T*u,T*v){
  lct_expose(u);
  u->reverse();
  lct_expose(v);
  v->left->par=nullptr;
  v->left=nullptr;
  v->update();
}
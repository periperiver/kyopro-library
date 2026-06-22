#pragma once
#include "splay.hpp"
template<typename T>
void tt_expose(T*nd){
  while(true){
    splay(nd);
    if(nd->right){
      nd->right->par=nullptr;
      T *light=new T();
      light->vertex=false;
      light->middle=nd->right;
      if(nd->middle){
        light->left=nd->middle;
        light->left->par=light;
      }
      nd->middle=light;
      light->par=nd;
      nd->right->par=light;
      nd->right=nullptr;
      if constexpr(has_update_v<T>)light->update(),nd->update();
    }
    if(!nd->par)break;
    T *pon=nd->par;
    splay(pon);
    splay(pon->par);
    if constexpr(has_push_v<T>)pon->push();
    if(pon->par->right){
      pon->middle=pon->par->right;
      pon->middle->par=pon;
      pon->par->right=nd;
      nd->par=pon->par;
      if constexpr(has_update_v<T>)pon->update(),pon->par->update();
    }
    else{
      pon->par->right=nd;
      nd->par=pon->par;
      T *l=pon->left,*r=pon->right;
      if(l&&r){
        l->par=nullptr;
        r->par=nullptr;
        l=merge(l,r);
        l->par=pon->par;
        l->par->middle=l;
      }
      else if(!l&&!r){
        pon->par->middle=nullptr;
      }
      else{
        if(!l)std::swap(l,r);
        l->par=pon->par;
        l->par->middle=l;
      }
      if constexpr(has_update_v<T>)nd->par->update();
      delete(pon);
    }
  }
}
template<typename T>
void tt_link(T*u,T*v){
  tt_expose(u);
  tt_expose(v);
  v->reverse();
  u->right=v;
  v->par=u;
  if constexpr(has_update_v<T>)u->update();
}
template<typename T>
void tt_cut(T*u,T*v){
  tt_expose(u);
  u->reverse();
  tt_expose(v);
  v->left=nullptr;
  u->par=nullptr;
  if constexpr(has_update_v<T>)v->update();
}
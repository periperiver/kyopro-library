#pragma once
#include "splay.hpp"
template<typename T>
T* splay_meld(T*lnd,T*rnd){
  static_assert(std::is_integral_v<decltype(T::key)>);
  if(!lnd)return rnd;
  if(!rnd)return lnd;
  T *res=nullptr;
  lnd=left_most(lnd);
  rnd=left_most(rnd);
  if(lnd->key>rnd->key)std::swap(lnd,rnd);
  while(true){
    decltype(T::key) k=rnd->key;
    while(true){
      if constexpr(has_push_v<T>)lnd->push();
      if(k<lnd->key){
        if(lnd->left)lnd=lnd->left;
        else break;
      }
      else{
        if(lnd->right)lnd=lnd->right;
        else break;
      }
    }
    splay<T,true>(lnd);
    T *llnd,*rlnd;
    if(lnd->key<=k){
      llnd=lnd;
      rlnd=lnd->right;
      llnd->right=nullptr;
      if(rlnd)rlnd->par=nullptr;
      if constexpr(has_update_v<T>)llnd->update();
    }
    else{
      llnd=lnd->left;
      rlnd=lnd;
      rlnd->left=nullptr;
      llnd->par=nullptr;
      if constexpr(has_update_v<T>)rlnd->update();
    }
    res=merge(res,llnd);
    lnd=rlnd;
    std::swap(lnd,rnd);
    if(!rnd){
      res=merge(res,lnd);
      break;
    }
    rnd=left_most(rnd);
  }
  return res;
}
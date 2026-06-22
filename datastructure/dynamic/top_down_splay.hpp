#pragma once
#include "splay.hpp"
#include<functional>
template<typename T,typename Func>
[[nodiscard]]T* top_down_splay(T*root,const Func&f){
  static_assert(!has_update_v<T>);
  static_assert(std::is_convertible_v<Func,std::function<int(T*)>>);
  T nil;
  T*lnd=&nil,*rnd=&nil;
  while(true){
    if constexpr(has_push_v<T>)root->push();
    int op=f(root);
    if(op==0)break;
    else if(op==-1){
      T*ch=root->left;
      if constexpr(has_push_v<T>)ch->push();
      int op2=f(ch);
      if(op2==0){
        rnd->left=root;
        rnd=rnd->left;
        root=ch;
        break;
      }
      else if(op2==-1){
        T*ch2=ch->left;
        rnd->left=ch;
        root->left=ch->right;
        ch->right=root;
        rnd=rnd->left;
        root=ch2;
      }
      else if(op2==1){
        T*ch2=ch->right;
        rnd->left=root;
        lnd->right=ch;
        rnd=rnd->left;
        lnd=lnd->right;
        root=ch2;
      }
    }
    else if(op==1){
      T*ch=root->right;
      if constexpr(has_push_v<T>)ch->push();
      int op2=f(ch);
      if(op2==0){
        lnd->right=root;
        lnd=lnd->right;
        root=ch;
        break;
      }
      else if(op2==-1){
        T*ch2=ch->left;
        lnd->right=root;
        rnd->left=ch;
        lnd=lnd->right;
        rnd=rnd->left;
        root=ch2;
      }
      else if(op2==1){
        T*ch2=ch->right;
        lnd->right=ch;
        root->right=ch->left;
        ch->left=root;
        lnd=lnd->right;
        root=ch2;
      }
    }
  }
  lnd->right=root->left;
  root->left=nil.right;
  rnd->left=root->right;
  root->right=nil.left;
  return root;
}
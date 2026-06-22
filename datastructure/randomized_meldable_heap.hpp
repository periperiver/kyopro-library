#pragma once
#include "random/generator.hpp"
template<typename T>
struct RandomizedMeldableHeap{
  struct node{
    node *left=nullptr,*right=nullptr;
    T val;
    node():val(){}
    node(const T&v):val(v){}
    static node* meld(node *lhs,node *rhs){
      if(!lhs)return rhs;
      if(!rhs)return lhs;
      if(lhs->val>rhs->val)std::swap(lhs,rhs);
      if(Random::get<unsigned long long>()&1)lhs->left=meld(lhs->left,rhs);
      else lhs->right=meld(lhs->right,rhs);
      return lhs;
    }
  };
  node *root;
  RandomizedMeldableHeap():root(nullptr){}
  RandomizedMeldableHeap(node *root):root(root){}
  void meld(RandomizedMeldableHeap rhs){
    this->root=node::meld(this->root,rhs.root);
  }
  void push(const T&v){
    this->root=node::meld(this->root,new node(v));
  }
  T top()const{return root->val;}
  T pop(){
    node *d=node::meld(root->left,root->right);
    T res=root->val;
    delete(root);
    root=d;
    return res;
  }
  inline bool empty()const{return !root;}
};
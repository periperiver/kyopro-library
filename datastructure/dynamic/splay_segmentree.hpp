#pragma once
#include<cassert>
#include "splay.hpp"
template<typename I,typename M>
struct DynamicSegmentTree{
private:
  using S=typename M::S;
  struct node{
    node *left,*right,*par;
    I key;
    S v,sum;
    node(I key,S v):left(nullptr),right(nullptr),par(nullptr),key(key),v(v),sum(v){}
    void update(){
      sum=v;
      if(left)sum=M::op(left->sum,sum);
      if(right)sum=M::op(sum,right->sum);
    }
    inline void copy_monoid(node *pp){sum=pp->sum;}
    ~node(){
      if(left)delete left;
      if(right)delete right;
    }
  };
  node *root;
public:
  DynamicSegmentTree():root(nullptr){}
  void set(I key,S v){
    if(!root){
      root=new node(key,v);
      return;
    }
    root=near(root,key);
    if(root->key==key){
      root->v=v;
      root->update();
    }
    else{
      node *nd=new node(key,v);
      if(key<root->key){
        nd->left=root->left;
        if(nd->left)nd->left->par=nd;
        root->left=nd;
        nd->par=root;
      }
      else{
        nd->right=root->right;
        if(nd->right)nd->right->par=nd;
        root->right=nd;
        nd->par=root;
      }
      nd->update();
      root->update();
    }
  }
  S get(I key){
    root=near(root,key);
    return root->key==key?root->v:M::e();
  }
  S prod(I l,I r){
    assert(l<=r);
    if(!root||l==r)return M::e();
    root=near(root,l);
    S res=M::e();
    if(l<=root->key&&root->key<r)res=root->v;
    if(root->right){
      node *rnd=root->right;
      rnd->par=nullptr;
      rnd=near(rnd,r);
      if(rnd->left)res=M::op(res,rnd->left->sum);
      if(rnd->key<r)res=M::op(res,rnd->v);
      root->right=rnd;
      rnd->par=root;
    }
    return res;
  }
  S all_prod(){return root?root->sum:M::e();}
  void deallocate(){
    if(root)delete root;
  }
};
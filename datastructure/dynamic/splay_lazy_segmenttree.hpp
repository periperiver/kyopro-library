#pragma once
#include<utility>
#include<limits>
#include "splay.hpp"
template<typename I,typename M>
struct DynamicLazySegmentTree{
private:
  using S=typename M::S;
  using F=typename M::F;
  struct node{
    node *left,*right,*par;
    I l,r,L,R;
    S v,sum;
    F lazy;
    node():left(nullptr),right(nullptr),par(nullptr),l(),r(),L(),R(),v(M::e()),sum(M::e()),lazy(M::id()){}
    node(I k,S v_):left(nullptr),right(nullptr),par(nullptr),l(k),r(k+1),L(k),R(k+1),v(v_),sum(v_),lazy(M::id()){}
    node(I l_,I r_,S v_):left(nullptr),right(nullptr),par(nullptr),l(l_),r(r_),L(l_),R(r_),v(v_),sum(v_),lazy(M::id()){}
    inline void propagate(const F&f){
      v=M::mapping(f,v,1);
      sum=M::mapping(f,sum,R-L);
      lazy=M::composition(f,lazy);
    }
    inline void update(){
      sum=M::pow(v,r-l);
      if(left)sum=M::op(left->sum,sum),L=left->L;
      else L=l;
      if(right)sum=M::op(sum,right->sum),R=right->R;
      else R=r;
    }
    inline void push(){
      if(lazy!=M::id()){
        if(left)left->propagate(lazy);
        if(right)right->propagate(lazy);
        lazy=M::id();
      }
    }
  };
  node nil;
  node *root;
  node *split(node *nd,I k){
    node *lnd=&nil,*rnd=&nil;
    while(true){
      nd->push();
      if(k<nd->l){
        node *c=nd->left;
        c->push();
        if(k<c->l){
          nd->left=c->right;
          c->right=nd;
          rnd->left=c;
          c->par=rnd;
          rnd=c;
          nd->update();
          nd=c->left;
        }
        else if(c->r<=k){
          rnd->left=nd;
          nd->par=rnd;
          rnd=nd;
          lnd->right=c;
          c->par=lnd;
          lnd=c;
          nd=c->right;
        }
        else{
          rnd->left=nd;
          nd->par=rnd;
          rnd=nd;
          nd=c;
          break;
        }
      }
      else if(nd->r<=k){
        node *c=nd->right;
        c->push();
        if(k<c->l){
          lnd->right=nd;
          nd->par=lnd;
          lnd=nd;
          rnd->left=c;
          c->par=rnd;
          rnd=c;
          nd=c->left;
        }
        else if(c->r<=k){
          nd->right=c->left;
          c->left=nd;
          lnd->right=c;
          c->par=lnd;
          lnd=c;
          nd->update();
          nd=c->right;
        }
        else{
          lnd->right=nd;
          nd->par=lnd;
          lnd=nd;
          nd=c;
          break;
        }
      }
      else break;
    }
    lnd->right=nd->left;
    while(lnd!=&nil){
      lnd->update();
      lnd=lnd->par;
    }
    rnd->left=nd->right;
    while(rnd!=&nil){
      rnd->update();
      rnd=rnd->par;
    }
    nd->left=nil.right;
    nd->right=nil.left;
    nd->update();
    return nd;
  }
  node *between(node *nd,I l,I r){
    nd=split(nd,l);
    node *lnd;
    if(nd->l==l){
      lnd=nd->left;
      nd->left=nullptr;
      nd->update();
    }
    else{
      lnd=new node(nd->l,l,nd->v);
      lnd->left=nd->left;
      lnd->update();
      nd->l=l;
      nd->left=nullptr;
    }
    nd=split(nd,r);
    if(nd->l!=r){
      node *mnd=new node(nd->l,r,nd->v);
      mnd->left=nd->left;
      nd->left=mnd;
      nd->l=r;
      mnd->update();
      nd->update();
    }
    lnd=right_most<node,false>(lnd);
    lnd->right=nd;
    lnd->update();
    return lnd;
  }
public:
  DynamicLazySegmentTree():nil(),root(new node(std::numeric_limits<I>::min()/2,std::numeric_limits<I>::max()/2,M::e())){}
  void set(I k,const S&x){
    root=between(root,k,k+1);
    root->right->left->v=x;
    root->right->update(),root->update();
  }
  S get(I k){
    root=split(root,k);
    return root->v;
  }
  void apply(I l,I r,const F&f){
    if(l==r)return;
    root=between(root,l,r);
    root->right->left->propagate(f);
    root->right->update(),root->update();
  }
  S prod(I l,I r){
    if(l==r)return M::e();
    root=between(root,l,r);
    return root->right->left->sum;
  }
  inline S all_prod()const{return root->sum;}
};
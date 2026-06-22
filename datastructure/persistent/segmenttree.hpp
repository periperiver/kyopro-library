#pragma once
#include<vector>
#include "math/util.hpp"
template<typename I,typename M>
struct PersistentSegmentTree{
  using S=typename M::S;
  struct node{
    node *left,*right;
    S v;
    node():node(M::e()){}
    node(S v):v(v),left(nullptr),right(nullptr){}
    inline void update(){
      v=M::op(left?left->v:M::e(),right?right->v:M::e());
    }
  };
  node *root;
  I L,R;
  PersistentSegmentTree(I L,I R,node *root):L(L),R(R),root(root){}
  PersistentSegmentTree():PersistentSegmentTree(0,0){}
  PersistentSegmentTree(I L,I R):L(L),R(R),root(nullptr){}
  PersistentSegmentTree(const std::vector<S>&init):L(0),R(init.size()){
    auto dfs=[&](auto self,int l,int r)->node* {
      if(l+1==r)return new node(init[l]);
      I mid=(l+r)/2;
      node *res=new node();
      res->left=self(self,l,mid);
      res->right=self(self,mid,r);
      res->update();
      return res;
    };
    root=dfs(dfs,0,init.size());
  }
  S get(I id)const{
    node *nd=root;
    I l=L,r=R;
    while(r-l>1&&nd){
      I mid=(l+r)/2;
      if(id<mid)nd=nd->left,r=mid;
      else nd=nd->right,l=mid;
    }
    return nd?nd->v:M::e();
  }
  S prod(I l,I r)const{
    if(l==r)return M::e();
    node *nd=root;
    I le=L,ri=R;
    while(nd&&ri-le>1){
      I mid=(le+ri)/2;
      if(r<=mid)nd=nd->left,ri=mid;
      else if(mid<=l)nd=nd->right,le=mid;
      else break;
    }
    if(!nd)return M::e();
    if(le==l&&ri==r)return nd->v;
    S res=M::e();
    node *ndl=nd->left,*ndr=nd->right;
    I ndl_l=le,ndl_r=(le+ri)/2;
    I ndr_l=(le+ri)/2,ndr_r=ri;
    while(ndl&&ndl_l<l){
      I mid=(ndl_l+ndl_r)/2;
      if(l<mid){
        if(ndl->right)res=M::op(ndl->right->v,res);
        ndl=ndl->left;
        ndl_r=mid;
      }
      else{
        ndl=ndl->right;
        ndl_l=mid;
      }
    }
    if(ndl)res=M::op(ndl->v,res);
    while(ndr&&ndr_r>r){
      I mid=(ndr_l+ndr_r)/2;
      if(r<=mid){
        ndr=ndr->left;
        ndr_r=mid;
      }
      else{
        if(ndr->left)res=M::op(res,ndr->left->v);
        ndr=ndr->right;
        ndr_l=mid;
      }
    }
    if(ndr)res=M::op(res,ndr->v);
    return res;
  }
  [[nodiscard]]PersistentSegmentTree set(I id,S val)const{
    node *newroot=new node();
    node *now=newroot,*old=root;
    I l=L,r=R;
    node *his[msb(R-L)+1];
    int p=0;
    while(r-l>1){
      his[p++]=now;
      I mid=(l+r)/2;
      if(id<mid){
        now->right=old?old->right:nullptr;
        now->left=new node();
        now=now->left;
        if(old)old=old->left;
        r=mid;
      }
      else{
        now->left=old?old->left:nullptr;
        now->right=new node();
        now=now->right;
        if(old)old=old->right;
        l=mid;
      }
    }
    now->v=val;
    while(p)his[--p]->update();
    return PersistentSegmentTree(L,R,newroot);
  }
  S all_prod()const{
    return root?root->v:M::e();
  }
  //Lからのみ
  template<typename Func>
  I max_right(const Func&f)const{
    S now=M::e();
    auto dfs=[&](auto self,node *nd,I l,I r)->I {
      if(!nd)return r;
      if(l+1==r){
        S nxt=M::op(now,nd->v);
        if(f(nxt)){
          now=nxt;
          return r;
        }
        else return l;
      }
      S al=M::op(now,nd->v);
      if(f(al)){
        now=al;
        return r;
      }
      I mid=(l+r)/2;
      I lres=self(self,nd->left,l,mid);
      if(lres==mid)return self(self,nd->right,mid,r);
      else return lres;
    };
    return dfs(dfs,root,L,R);
  }
};
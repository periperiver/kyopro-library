#pragma once
#include<numeric>
#include<limits>
template<typename T>
struct LiChaoTree{
private:
  static constexpr T inf=std::numeric_limits<T>::max();
  struct node{
    node *left=nullptr,*right=nullptr;
    T a,b;
    node(T a_,T b_):a(a_),b(b_){}
    node():a(0),b(inf){}
    T f(T x)const{return a*x+b;}
  };
  static void swap(node *lhs,node *rhs){
    std::swap(lhs->a,rhs->a);
    std::swap(lhs->b,rhs->b);
  }
  void add(node *nd,node *nw,T l,T r){
    while(true){
      if(nw->f(l)<nd->f(l))swap(nd,nw);
      if(nd->f(r-1)<=nw->f(r-1))return;
      T mid=(l+r)/2;
      if(nd->f(mid)<=nw->f(mid)){
        if(!nd->right){
          nd->right=new node(*nw);
          return;
        }
        else{
          nd=nd->right;
          l=mid;
        }
      }
      else{
        swap(nd,nw);
        if(!nd->left){
          nd->left=new node(*nw);
          return;
        }
        else{
          nd=nd->left;
          r=mid;
        }
      }
    }
  }
  void add2(node *nd,node *nw,T l,T r,T lq,T rq){
    if(rq<=l||r<=lq)return;
    if(lq<=l&&r<=rq){
      node nw2(nw->a,nw->b);
      add(nd,&nw2,l,r);
    }
    else{
      T mid=(l+r)/2;
      if(!nd->left&&lq<mid)nd->left=new node();
      if(!nd->right&&rq>mid)nd->right=new node();
      add2(nd->left,nw,l,mid,lq,rq);
      add2(nd->right,nw,mid,r,lq,rq);
    }
  }
  node *root;
  const T L,R;
public:
  LiChaoTree(T L_,T R_):L(L_),R(R_),root(new node()){}
  inline void add_line(T a,T b){
    node nd(a,b);
    add(root,&nd,L,R);
  }
  inline void add_segment(T l,T r,T a,T b){
    node nd(a,b);
    add2(root,&nd,L,R,l,r);
  }
  inline T query(T x)const{
    T res=inf;
    node *nd=root;
    T l=L,r=R;
    while(nd){
      T now=nd->f(x);
      if(res>now)res=now;
      T mid=(l+r)/2;
      if(x<mid)nd=nd->left,r=mid;
      else nd=nd->right,l=mid;
    }
    return res;
  }
};
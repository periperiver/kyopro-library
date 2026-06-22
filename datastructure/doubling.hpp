#pragma once
#include "template.hpp"
template<typename S,auto op,auto e,typename F,auto mapping,auto composition,auto id>
struct DynamicDoubling{
private:
  struct node{
    int sz,idx;
    S v,sum;
    F lazy;
    node *left,*right,*par;
    node():sz(1),idx(-1),v(e()),sum(e()),lazy(id()),left(nullptr),right(nullptr),par(nullptr){}
    node(S v_):sz(1),idx(-1),v(v_),sum(v_),lazy(id()),left(nullptr),right(nullptr),par(nullptr){}
    inline void update(){
      sz=1;
      sum=v;
      if(left)sz+=left->sz,sum=op(sum,left->sum);
      if(right)sz+=right->sz,sum=op(right->sum,sum);
    }
    inline void push(){
      if(left)left->propagate(lazy);
      if(right)right->propagate(lazy);
      lazy=id();
    }
    inline void propagate(F lz){
      v=mapping(lz,v);
      sum=mapping(lz,sum);
      lazy=composition(lz,lazy);
    }
    inline bool is_root()const{
      if(!this->par)return true;
      return this->par->left!=this&&this->par->right!=this;
    }
    void splay(){
      while(!this->is_root()){
        node *p=this->par;
        p->push();
        this->push();
        if(p->is_root()){
          if(p->left==this){
            if(this->right)this->right->par=p;
            p->left=this->right;
            this->right=p;
            this->par=p->par;
            p->par=this;
          }
          else{
            if(this->left)this->left->par=p;
            p->right=this->left;
            this->left=p;
            this->par=p->par;
            p->par=this;
          }
          this->sum=p->sum;
          this->sz=p->sz;
          p->update();
        }
        else{
          node *pp=p->par;
          pp->push();
          p->push();
          this->push();
          if(p->left==this){
            if(pp->left==p){
              if(this->right)this->right->par=p;
              if(p->right)p->right->par=pp;
              p->left=this->right;
              this->right=p;
              pp->left=p->right;
              p->right=pp;
              if(pp->par){
                if(pp->par->left==pp)pp->par->left=this;
                else if(pp->par->right==pp)pp->par->right=this;
              }
              this->par=pp->par;
              p->par=this;
              pp->par=p;
            }
            else{
              if(this->left)this->left->par=pp;
              if(this->right)this->right->par=p;
              pp->right=this->left;
              p->left=this->right;
              if(pp->par){
                if(pp->par->left==pp)pp->par->left=this;
                else if(pp->par->right==pp)pp->par->right=this;
              }
              this->par=pp->par;
              pp->par=this;
              p->par=this;
              this->left=pp;
              this->right=p;
            }
          }
          else{
            if(pp->left==p){
              if(this->left)this->left->par=p;
              if(this->right)this->right->par=pp;
              p->right=this->left;
              pp->left=this->right;
              if(pp->par){
                if(pp->par->left==pp)pp->par->left=this;
                else if(pp->par->right==pp)pp->par->right=this;
              }
              this->par=pp->par;
              p->par=this;
              pp->par=this;
              this->left=p;
              this->right=pp;
            }
            else{
              if(this->left)this->left->par=p;
              if(p->left)p->left->par=pp;
              p->right=this->left;
              this->left=p;
              pp->right=p->left;
              p->left=pp;
              if(pp->par){
                if(pp->par->left==pp)pp->par->left=this;
                else if(pp->par->right==pp)pp->par->right=this;
              }
              this->par=pp->par;
              p->par=this;
              pp->par=p;
            }
          }
          this->sum=pp->sum;
          this->sz=pp->sz;
          pp->update();
          p->update();
        }
      }
      this->push();
    }
  };
  void access(node *nd){
    while(true){
      nd->splay();
      nd->right=nullptr;
      nd->update();
      if(!nd->par)break;
      nd->par->splay();
      nd->par->right=nd;
      nd->par->update();
    }
  }
  vector<node>dat;
  vector<int>p;
  S power_prod(S x,ll k){
    S ret=e();
    while(k){
      if(k&1)ret=op(ret,x);
      x=op(x,x);
      k>>=1;
    }
    return ret;
  }
  F power_lazy(F lazy,ll k){
    F ret=id();
    while(k){
      if(k&1)ret=composition(ret,lazy);
      lazy=composition(lazy,lazy);
      k>>=1;
    }
    return ret;
  }
  void get_kth(node*&nd,int k){
    while(true){
      int rsize=nd->right?nd->right->sz:0;
      if(rsize>k)nd=nd->right;
      else if(rsize==k)break;
      else nd=nd->left,k-=rsize+1;
    }
    nd->splay();
  }
public:
  DynamicDoubling(int n):dat(n),p(n){
    iota(all(p),0);
    rep(i,n)dat[i].idx=i;
  }
  void set_p(int i,int x){
    access(&dat[i]);
    node *nd=&dat[i];
    while(nd->left)nd=nd->left;
    nd->splay();
    access(&dat[p[nd->idx]]);
    dat[i].splay();
    if(dat[i].par){
      if(dat[i].left){
        dat[i].left->par=dat[i].par;
        dat[i].left=nullptr;
        dat[i].update();
      }
      dat[i].par=nullptr;
    }
    else if(dat[i].left){
      dat[i].left->par=nullptr;
      dat[i].left=nullptr;
      dat[i].update();
      nd->splay();
      nd->left=&dat[i];
      dat[i].par=nd;
      nd->update();
      dat[i].splay();
    }
    p[i]=x;
    node *ndx=&dat[x];
    access(ndx);
    while(ndx->left)ndx=ndx->left;
    ndx->splay();
    if(ndx->idx!=i)dat[i].par=&dat[x];
  }
  void set_v(int i,S x){
    dat[i].splay();
    dat[i].v=x;
    dat[i].update();
  }
  void apply(int i,ll k,F f){
    if(k==0)return;
    access(&dat[i]);
    if(dat[i].sz>k){
      node *nd=&dat[i];
      get_kth(nd,k);
      nd->right->propagate(f);
      return;
    }
    dat[i].propagate(f);
    k-=dat[i].sz;
    node *nd=&dat[i];
    while(nd->left)nd=nd->left;
    nd->splay();
    nd=&dat[p[nd->idx]];
    access(nd);
    nd->propagate(power_lazy(f,k/nd->sz));
    k%=nd->sz;
    if(k==0)return;
    get_kth(nd,k);
    nd->right->propagate(f);
  }
  S prod(int i,ll k){
    if(k==0)return e();
    access(&dat[i]);
    if(dat[i].sz>k){
      node *nd=&dat[i];
      get_kth(nd,k);
      return nd->right->sum;
    }
    S ret=dat[i].sum;
    k-=dat[i].sz;
    node *nd=&dat[i];
    while(nd->left)nd=nd->left;
    nd->splay();
    nd=&dat[p[nd->idx]];
    access(nd);
    ret=op(ret,power_prod(nd->sum,k/nd->sz));
    k%=nd->sz;
    if(k!=0){
      get_kth(nd,k);
      ret=op(ret,nd->right->sum);
    }
    return ret;
  }
  int step(int i,ll k){
    access(&dat[i]);
    if(dat[i].sz>k){
      node *nd=&dat[i];
      get_kth(nd,k);
      return nd->idx;
    }
    k-=dat[i].sz;
    node *nd=&dat[i];
    while(nd->left)nd=nd->left;
    nd->splay();
    nd=&dat[p[nd->idx]];
    access(nd);
    k%=nd->sz;
    get_kth(nd,k);
    return nd->idx;
  }
};
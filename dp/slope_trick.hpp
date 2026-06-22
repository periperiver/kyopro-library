#pragma once
#include<utility>
#include<vector>
#include<concepts>
#include<cassert>
#include "../datastructure/dynamic/splay.hpp"
template<std::integral T,T lim>
struct SlopeTrick{
private:
  struct node{
    node *left,*right,*par;
    T slope,len;
    T slope_sum,len_sum;
    T lazy;
    int sz;
    node():left(nullptr),right(nullptr),par(nullptr),slope(0),len(0),slope_sum(0),len_sum(0),lazy(0),sz(1){}
    void propagate(T f){
      lazy+=f;
      slope+=f;
      slope_sum+=f*len_sum;
    }
    void push(){
      if(left)left->propagate(lazy);
      if(right)right->propagate(lazy);
      lazy=0;
    }
    void update(){
      slope_sum=slope*len;
      len_sum=len;
      sz=1;
      if(left){
        slope_sum+=left->slope_sum;
        len_sum+=left->len_sum;
        sz+=left->sz;
      }
      if(right){
        slope_sum+=right->slope_sum;
        len_sum+=right->len_sum;
        sz+=right->sz;
      }
    }
    ~node(){
      if(left)delete left;
      if(right)delete right;
    }
  };
  void search_slope(T slope){
    while(true){
      root->push();
      if(root->slope==slope)break;
      else if(root->slope>slope){
        if(root->left)root=root->left;
        else break;
      }
      else{
        if(root->right)root=root->right;
        else break;
      }
    }
    splay<node,true>(root);
  }
  std::pair<node*,node*>split(node *nd,T pos){
    while(true){
      nd->push();
      T llen=nd->left?nd->left->len_sum:0;
      if(pos<llen)nd=nd->left;
      else if(pos<llen+nd->len){
        splay<node,true>(nd);
        if(pos==llen){
          node *lnd=nd->left;
          nd->left=nullptr;
          lnd->par=nullptr;
          nd->update();
          return std::make_pair(lnd,nd);
        }
        else{
          node *rnd=new node();
          rnd->slope=nd->slope;
          rnd->len=llen+nd->len-pos;
          rnd->right=nd->right;
          if(rnd->right)rnd->right->par=rnd;
          rnd->update();
          nd->right=nullptr;
          nd->len=pos-llen;
          nd->update();
          return std::make_pair(nd,rnd);
        }
      }
      else pos-=llen+nd->len,nd=nd->right;
    }
    __builtin_unreachable();
  }
  node *root;
  T lv,lp;
  public:
  SlopeTrick():root(nullptr),lv(0),lp(0){}
  SlopeTrick(T l,T r):root(new node()),lv(0),lp(l){
    assert(-lim<=l&&l<=r&&r<=lim);
    root->len=root->len_sum=r-l;
  }
  void clamp(T nl,T nr){
    if(!root)assert(!"invalid slope trick");
    if(nr<lp||lp+root->len_sum<nl)assert(!"no intersection");
    if(std::max(lp,nl)==std::min(lp+root->len_sum,nr)){
      T nx=std::max(lp,nl);
      T nv=get(nx);
      delete root;
      root=new node();
      lp=nx,lv=nv;
      return;
    }
    node*tmp;
    if(lp<nl){
      std::tie(tmp,root)=split(root,nl-lp);
      lp=nl;
      lv+=tmp->slope_sum;
      delete tmp;
    }
    if(nr<lp+root->len_sum){
      std::tie(root,tmp)=split(root,nr-lp);
      delete tmp;
    }
  }
  void add(T b){
    if(!root)assert(!"invalid slope trick");
    lv+=b;
  }
  void add_abs(T a,T c=1){
    if(!root)assert(!"invalid slope trick");
    lv+=c*(a-lp);
    root->propagate(-c);
    add_r(a,c*2);
  }
  void add_l(T a,T c){
    if(!root)assert(!"invalid slope trick");
    lv+=c*(a-lp);
    root->propagate(-c);
    add_r(a,c);
  }
  void add_r(T a,T c){
    if(!root)assert(!"invalid slope trick");
    if(a<=lp){
      root->propagate(c);
      lv+=c*(lp-a);
      return;
    }
    if(lp+root->len_sum<=a)return;
    auto [lnd,rnd]=split(root,a-lp);
    rnd->propagate(c);
    rnd->push();
    rnd->left=lnd;
    lnd->par=rnd;
    rnd->update();
    root=rnd;
  }
  void add(SlopeTrick rhs){
    if(!root||!rhs.root)assert(!"invalid slope trick");
    T nl=std::max(lp,rhs.lp),nr=std::min(lp+root->len_sum,rhs.root->len_sum);
    if(nl>nr)assert(!"no intersection");
    this->clamp(nl,nr);
    rhs.clamp(nl,nr);
    lv+=rhs.lv;
    if(root->sz<rhs.root->sz){
      std::swap(root,rhs.root);
    }
    node *top=nullptr;
    auto dfs=[&](auto self,node *nd)->void {
      if(!nd)return;
      nd->push();
      self(self,nd->left);
      if(nd->len==root->len_sum){
        root->propagate(nd->slope);
        if(top)top->right=root;
        root->par=top;
      }
      else{
        auto [lnd,rnd]=split(root,nd->len);
        lnd->propagate(nd->slope);
        lnd=right_most(lnd);
        if(top)top->right=lnd;
        lnd->par=top;
        top=lnd;
        root=rnd;
      }
      self(self,nd->right);
    };
    dfs(dfs,rhs.root);
    while(root->par){
      root=root->par;
      root->update();
    }
  }
  void convolution(SlopeTrick rhs){
    lp+=rhs.lp;
    lv+=rhs.lv;
    if(!root||!rhs.root){
      if(!root)root=rhs.root;
      return;
    }
    if(root->len==0){
      root=rhs.root;
      return;
    }
    if(rhs.root->len==0)return;
    if(root->sz<rhs.root->sz)std::swap(root,rhs.root);
    auto dfs=[&](auto self,node *nd)->void {
      if(!nd)return;
      nd->push();
      self(self,nd->left);
      node *rnd=nd->right;
      search_slope(nd->slope);
      if(root->slope==nd->slope){
        root->len+=nd->len;
        nd->left=nd->right=nullptr;
        delete nd;
      }
      else if(root->slope<nd->slope){
        nd->left=nullptr;
        nd->right=root->right;
        if(nd->right)nd->right->par=nd;
        root->right=nd;
        nd->par=root;
        nd->update();
      }
      else{
        nd->right=nullptr;
        nd->left=root->left;
        if(nd->left)nd->left->par=nd;
        root->left=nd;
        nd->par=root;
        nd->update();
      }
      root->update();
      self(self,rnd);
    };
    dfs(dfs,rhs.root);
    clamp(-lim,lim);
  }
  T min(){
    if(!root)return std::numeric_limits<T>::max();
    search_slope(0);
    T res;
    if(root->slope>=0){
      if(!root->left)res=lv;
      else res=lv+root->left->slope_sum;
    }
    else{
      res=lv+root->slope*root->len;
      if(root->left)res+=root->left->slope_sum;
    }
    return res;
  }
  std::pair<T,T>amin(){
    search_slope(0);
    if(root->slope==0){
      T l=lp+(root->left?root->left->len_sum:0);
      return std::make_pair(l,l+root->len);
    }
    else{
      T l=lp+(root->left?root->left->len_sum:0);
      if(root->slope<0)l+=root->len;
      return std::make_pair(l,l);
    }
  }
  T get(T x){
    if(!root||x<lp||lp+root->len_sum<x)return std::numeric_limits<T>::max();
    x-=lp;
    if(x==root->len_sum)return lv+root->slope_sum;
    while(true){
      root->push();
      T llen=root->left?root->left->len_sum:0;
      if(x<llen)root=root->left;
      else if(x<llen+root->len){
        x-=llen;
        break;
      }
      else x-=llen+root->len,root=root->right;
    }
    splay<node,true>(root);
    return lv+(root->left?root->left->slope_sum:0)+x*root->slope;
  }
};
#pragma once
#include<iostream>
#include<cassert>
#include "rolling_hash_base.hpp"
#include "datastructure/dynamic/splay.hpp"
template<typename T=char>
struct DynamicString{
private:
  struct dynamic_string_impl{
    struct node{
      node *par,*left,*right;
      T v;
      mint61 sum,mus;
      bool rev;
      int sz;
      node():par(nullptr),left(nullptr),right(nullptr),v(),sum(),mus(),rev(false),sz(1){}
      explicit node(T v_):par(nullptr),left(nullptr),right(nullptr),v(v_),sum(v_),mus(v_),rev(false),sz(1){}
      void reverse(){
        std::swap(left,right);
        std::swap(sum,mus);
        rev^=1;
      }
      void push(){
        if(rev){
          if(left)left->reverse();
          if(right)right->reverse();
          rev=false;
        }
      }
      void update(){
        sum=mus=mint61::raw(v);
        sz=1;
        if(left){
          sum=left->sum*RollingHashBase::power[1]+sum;
          mus=mus*RollingHashBase::power[left->sz]+left->mus;
          sz+=left->sz;
        }
        if(right){
          sum=sum*RollingHashBase::power[right->sz]+right->sum;
          mus=right->mus*RollingHashBase::power[sz]+mus;
          sz+=right->sz;
        }
      }
      ~node(){
        if(left)delete left;
        if(right)delete right;
      }
    };
    node *root;
    dynamic_string_impl():root(nullptr){}
    dynamic_string_impl(T x):root(new node(x)){RollingHashBase::resize(1);}
    template<typename U,std::enable_if_t<std::is_same_v<U,std::string>||std::is_same_v<U,std::vector<T>>,std::nullptr_t> =nullptr>
    dynamic_string_impl(const U&s){
      RollingHashBase::resize(s.size());
      if(s.empty())root=nullptr;
      else{
        root=new node(s[0]);
        for(int i=1;i<(int)s.size();i++){
          node *nxt=new node(s[i]);
          nxt->left=root;
          root->par=nxt;
          root=nxt;
          root->update();
        }
      }
    }
    static std::tuple<node*,node*,node*>split3(node *nd,int l,int r){
      if(l==0||r==nd->sz){
        if(l==0&&r==nd->sz)return std::make_tuple(nullptr,nd,nullptr);
        else if(l==0){
          nd=get_k(nd,r);
          node *lnd=nd->left;
          lnd->par=nd->left=nullptr;
          nd->update();
          return std::make_tuple(nullptr,lnd,nd);
        }
        else{
          nd=get_k(nd,l);
          node *lnd=nd->left;
          lnd->par=nd->left=nullptr;
          nd->update();
          return std::make_tuple(lnd,nd,nullptr);
        }
      }
      nd=get_k(nd,r);
      node *rnd=nd;
      nd=nd->left;
      rnd->left=nd->par=nullptr;
      rnd->update();
      nd=get_k(nd,l);
      node *lnd=nd->left;
      lnd->par=nd->left=nullptr;
      nd->update();
      return std::make_tuple(lnd,nd,rnd);
    }
    static int lcp(node*&lhs,node*&rhs){
      int ok=0,ng=std::min(lhs->sz,rhs->sz);
      if(lhs->sz==ng||rhs->sz==ng){
        if(lhs->sz==ng&&rhs->sz==ng){
          if(lhs->sum==rhs->sum)return ng;
        }
        else if(lhs->sz==ng){
          rhs=get_k(rhs,ng);
          if(lhs->sum==rhs->left->sum)return ng;
        }
        else{
          lhs=get_k(lhs,ng);
          if(rhs->sum==lhs->left->sum)return ng;
        }
        ng--;
      }
      while(ng-ok>1){
        int mid=(ok+ng)/2;
        lhs=get_k(lhs,mid);
        rhs=get_k(rhs,mid);
        if(lhs->left->sum==rhs->left->sum)ok=mid;
        else ng=mid;
      }
      return ok;
    }
    static int comp(node*&lhs,node*&rhs){
      if(!lhs||!rhs){
        if(!lhs&&!rhs)return 0;
        if(!lhs)return -1;
        return 1;
      }
      int l=lcp(lhs,rhs);
      if(l==lhs->sz||l==rhs->sz){
        if(l==lhs->sz&&l==rhs->sz)return 0;
        if(l==lhs->sz)return -1;
        return 1;
      }
      lhs=get_k(lhs,l);
      rhs=get_k(rhs,l);
      if(lhs->v<rhs->v)return -1;
      return 1;
    }
    void insert(int k,T v){
      assert(0<=k&&k<=size());
      if(!root)root=new node(v);
      else if(k==root->sz){
        node *nxt=new node(v);
        nxt->left=root;
        root->par=nxt;
        root=nxt;
        root->update();
      }
      else{
        root=get_k(root,k);
        node *nxt=new node(v);
        nxt->left=root->left;
        if(nxt->left)nxt->left->par=nxt;
        root->left=nxt;
        nxt->par=root;
        nxt->update();
        root->update();
      }
      RollingHashBase::resize(size());
    }
    void erase(int k){
      assert(0<=k&&k<size());
      root=get_k(root,k);
      node *lnd=root->left,*rnd=root->right;
      root->left=root->right=nullptr;
      delete root;
      if(lnd)lnd->par=nullptr;
      if(rnd)rnd->par=nullptr;
      root=merge(lnd,rnd);
    }
    mint61 get(int l,int r){
      assert(0<=l&&l<=r&&r<=size());
      if(l==r)return mint61();
      auto [lnd,mnd,rnd]=split3(root,l,r);
      mint61 res=mnd->sum;
      root=merge(lnd,merge(mnd,rnd));
      return res;
    }
    void reverse(int l,int r){
      assert(0<=l&&l<=r&&r<=size());
      if(r-l<=1)return;
      auto [lnd,mnd,rnd]=split3(root,l,r);
      mnd->reverse();
      root=merge(lnd,merge(mnd,rnd));
    }
    T get(int k){
      assert(0<=k&&k<size());
      root=get_k(root,k);
      return root->v;
    }
    void set(int k,T v){
      assert(0<=k&&k<size());
      root=get_k(root,k);
      root->v=v;
      root->update();
    }
    inline int size()const{
      return root?root->sz:0;
    }
  };
  dynamic_string_impl *dat;
public:
  DynamicString():dat(new dynamic_string_impl()){}
  DynamicString(T x):dat(new dynamic_string_impl(x)){}
  template<typename U,std::enable_if_t<std::is_same_v<U,std::string>||std::is_same_v<U,std::vector<T>>,std::nullptr_t> =nullptr>
  DynamicString(const U&s):dat(new dynamic_string_impl(s)){}
  inline void insert(int k,T v){dat->insert(k,v);}
  inline void erase(int k){dat->erase(k);}
  inline mint61 get(int l,int r)const{return dat->get(l,r);}
  inline void reverse(int l,int r){dat->reverse(l,r);}
  inline T operator[](int k)const{return dat->get(k);}
  inline void set(int k,T v){dat->set(k,v);}
  inline int size()const{return dat->size();}
  inline void destruct(){delete dat;}
  friend bool operator==(DynamicString lhs,DynamicString rhs){return dynamic_string_impl::comp(lhs.dat->root,rhs.dat->root)==0;}
  friend bool operator!=(DynamicString lhs,DynamicString rhs){return dynamic_string_impl::comp(lhs.dat->root,rhs.dat->root)!=0;}
};
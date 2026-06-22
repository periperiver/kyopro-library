#pragma once
#include<type_traits>
#include<cstdint>
#include<limits>
#include "math/util.hpp"
template<typename T,typename T2>
struct DynamicConvexHullTrick{
  static_assert(std::is_integral_v<T>);
private:
  static constexpr T inf=std::numeric_limits<T>::max();
  struct line{
    T a,b;
    int idx;
    line():a(0),b(inf),idx(-1){}
    line(T a_,T b_,int idx_):a(a_),b(b_),idx(idx_){}
    inline T operator()(T x)const{
      return a*x+b;
    }
    bool operator<(const line&rhs)const{
      if(a==rhs.a){
        if(b==rhs.b)return idx<rhs.idx;
        return b<rhs.b;
      }
      return a<rhs.a;
    }
    bool operator==(const line&rhs)const{
      return a==rhs.a&&b==rhs.b&&idx==rhs.idx;
    }
  };
  static inline bool convex(const line&a,const line&b,const line&c){
    T2 lhs=T2(b.a-a.a)*T2(c.b-a.b);
    T2 rhs=T2(c.a-a.a)*T2(b.b-a.b);
    if(lhs!=rhs)return lhs>rhs;
    if(std::min(a.idx,c.idx)>b.idx)return true;
    if(a.idx>b.idx&&a.a==b.a&&a.b==b.b)return true;
    if(c.idx>b.idx&&b.a==c.a&&b.b==c.b)return true;
    return false;
  }
  struct AVL{
    struct avl_node{
      avl_node *left,*right,*par;
      int8_t height;
      line lc,rc;
      line lm;
      bool lazy;
      avl_node():left(nullptr),right(nullptr),par(nullptr),height(0),lazy(false){}
      avl_node(T a,T b,int idx_):left(nullptr),right(nullptr),par(nullptr),height(0),lc(a,b,idx_),rc(a,b,idx_),lm(a,b,idx_),lazy(false){}
      inline int8_t diff()const{
        return (left?left->height:0)-(right?right->height:0);
      }
      inline void light_update(){
        lm=left->lm;
        height=std::max(left->height,right->height)+1;
      }
      inline void heavy_update(){
        if(!lazy)return;
        left->heavy_update();
        right->heavy_update();
        avl_node *lnd=left,*rnd=right;
        T midx=rnd->lm.a;
        while(lnd->left||rnd->left){
          if(lnd->left&&rnd->left){
            if(!convex(lnd->lc,lnd->rc,rnd->lc)){
              lnd=lnd->left;
            }
            else if(!convex(lnd->rc,rnd->lc,rnd->rc)){
              rnd=rnd->right;
            }
            else{
              if((T2(lnd->rc.b-lnd->lc.b)*T2(midx-lnd->lc.a)+T2(lnd->lc.b)*T2(lnd->rc.a-lnd->lc.a))*T2(rnd->rc.a-rnd->lc.a)<(T2(rnd->rc.b-rnd->lc.b)*T2(midx-rnd->lc.a)+T2(rnd->lc.b)*T2(rnd->rc.a-rnd->lc.a))*T2(lnd->rc.a-lnd->lc.a)){
                lnd=lnd->right;
              }
              else{
                rnd=rnd->left;
              }
            }
          }
          else if(lnd->left){
            if(convex(lnd->lc,lnd->rc,rnd->lc))lnd=lnd->right;
            else lnd=lnd->left;
          }
          else{
            if(convex(lnd->rc,rnd->lc,rnd->rc))rnd=rnd->left;
            else rnd=rnd->right;
          }
        }
        lc=lnd->lc,rc=rnd->rc;
        lazy=false;
      }
    };
    static avl_node* rotl(avl_node*nd){
      avl_node *ch=nd->right;
      if(nd->par){
        if(nd->par->left==nd)nd->par->left=ch;
        else nd->par->right=ch;
      }
      ch->par=nd->par;
      nd->right=ch->left;
      nd->right->par=nd;
      ch->left=nd;
      nd->par=ch;
      return ch;
    }
    static avl_node* rotr(avl_node*nd){
      avl_node *ch=nd->left;
      if(nd->par){
        if(nd->par->left==nd)nd->par->left=ch;
        else nd->par->right=ch;
      }
      ch->par=nd->par;
      nd->left=ch->right;
      nd->left->par=nd;
      ch->right=nd;
      nd->par=ch;
      return ch;
    }
    static avl_node* balance(avl_node*nd){
      if(nd->diff()==2){
        if(nd->left->diff()==-1){
          nd->left=rotl(nd->left);
          nd=rotr(nd);
          nd->left->light_update();
          nd->right->light_update();
          nd->light_update();
          nd->left->lazy=nd->right->lazy=nd->lazy=true;
        }
        else{
          nd=rotr(nd);
          nd->right->light_update();
          nd->light_update();
          nd->right->lazy=nd->lazy=true;
        }
      }
      else if(nd->diff()==-2){
        if(nd->right->diff()==1){
          nd->right=rotr(nd->right);
          nd=rotl(nd);
          nd->left->light_update();
          nd->right->light_update();
          nd->light_update();
          nd->left->lazy=nd->right->lazy=nd->lazy=true;
        }
        else{
          nd=rotl(nd);
          nd->left->light_update();
          nd->light_update();
          nd->left->lazy=nd->lazy=true;
        }
      }
      else{
        nd->light_update();
        nd->lazy=true;
      }
      return nd;
    }
    static void insert(avl_node*&root,T a,T b,int idx){
      if(!root){
        root=new avl_node(a,b,idx);
        return;
      }
      avl_node *nd=root;
      line ins(a,b,idx);
      while(nd->left){
        if(ins<nd->right->lm)nd=nd->left;
        else nd=nd->right;
      }
      avl_node *nch=new avl_node(a,b,idx);
      avl_node *np=new avl_node();
      if(nd->par){
        if(nd->par->left==nd)nd->par->left=np;
        else nd->par->right=np;
        np->par=nd->par;
      }
      nch->par=np;
      nd->par=np;
      if(ins<nd->lm){
        np->left=nch;
        np->right=nd;
      }
      else{
        np->left=nd;
        np->right=nch;
      }
      while(true){
        np=balance(np);
        if(np->par)np=np->par;
        else break;
      }
      root=np;
    }
    static bool erase(avl_node*&root,T a,T b,int idx=-1){
      if(!root)return false;
      avl_node *nd=root;
      line era(a,b,idx);
      while(nd->left){
        if(era<nd->right->lm)nd=nd->left;
        else nd=nd->right;
      }
      if(!(era==nd->lm))return false;
      avl_node *p=nd->par;
      if(!p){
        root=nullptr;
        delete nd;
      }
      else if(p->par){
        avl_node *nnd;
        if(p->left==nd)nnd=p->right;
        else nnd=p->left;
        if(p->par->left==p)p->par->left=nnd;
        else p->par->right=nnd;
        nnd->par=p->par;
        delete p;
        delete nd;
        nnd=nnd->par;
        while(true){
          nnd=balance(nnd);
          if(nnd->par)nnd=nnd->par;
          else break;
        }
        root=nnd;
      }
      else{
        if(p->left==nd)root=p->right;
        else root=p->left;
        root->par=nullptr;
        delete p;
        delete nd;
      }
      return true;
    }
  };
  typename AVL::avl_node *root;
public:
  DynamicConvexHullTrick():root(nullptr){}
  void add(T a,T b,int idx=-1){
    AVL::insert(root,a,b,idx);
  }
  bool erase(T a,T b,int idx=-1){
    return AVL::erase(root,a,b,idx);
  }
  line min(T x)const{
    if(!root)return line(0,inf,-1);
    root->heavy_update();
    typename AVL::avl_node *nd=root;
    while(nd->left){
      T leval=nd->lc(x),reval=nd->rc(x);
      if(leval<reval)nd=nd->left;
      else if(leval>reval)nd=nd->right;
      else if(nd->lc.idx<nd->rc.idx)nd=nd->left;
      else nd=nd->right;
    }
    return nd->lc;
  }
};
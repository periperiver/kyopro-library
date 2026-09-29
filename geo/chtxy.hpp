#pragma once
#include "point.hpp"
#include "../datastructure/dynamic/top_down_splay.hpp"
template<typename T>
struct CHTxy{
private:
  struct node{
    node *left,*right,*ptr;
    Point<T>p;
    node(){}
    node(Point<T>p):left(nullptr),right(nullptr),ptr(nullptr),p(p){}
    ~node(){
      if(left)delete left;
      if(right)delete right;
    }
  };
  struct lower_hull{
    node *root;
    lower_hull():root(nullptr){}
    bool empty()const{return !root;}
    static node* delete_step_l(node *nd,const Point<T>&p){
      while(nd->left){
        nd->left=right_most<node,false>(nd->left);
        node *lnd=nd->left;
        if(cross(lnd->p,nd->p,p)<0){
          nd->left=nullptr;
          delete nd;
          nd=lnd;
        }
        else break;
      }
      return nd;
    }
    static node* delete_step_r(node *nd,const Point<T>&p){
      while(nd->right){
        nd->right=left_most<node,false>(nd->right);
        node *rnd=nd->right;
        if(cross(p,nd->p,rnd->p)<0){
          nd->right=nullptr;
          delete nd;
          nd=rnd;
        }
        else break;
      }
      return nd;
    }
    void add(Point<T>p){
      if(!root)root=new node(p);
      else{
        root=top_down_splay(root,[&](node *nd)->int {
          if(p==nd->p)return 0;
          else if(p<nd->p)return nd->left?-1:0;
          else return nd->right?1:0;
        });
        if(p==root->p)return;
        node *lnd,*rnd;
        if(p<root->p){
          lnd=root->left;
          rnd=root;
          rnd->left=nullptr;
          if(!lnd){
            rnd=delete_step_r(rnd,p);
            root=new node(p);
            root->right=rnd;
            root->ptr=rnd;
            return;
          }
          else lnd=right_most<node,false>(lnd);
        }
        else{
          rnd=root->right;
          lnd=root;
          lnd->right=nullptr;
          if(!rnd){
            lnd=delete_step_l(lnd,p);
            root=new node(p);
            root->left=lnd;
            lnd->ptr=root;
            return;
          }
          else rnd=left_most<node,false>(rnd);
        }
        if(cross(lnd->p,p,rnd->p)<0){
          root=rnd;
          root->left=lnd;
          return;
        }
        lnd=delete_step_l(lnd,p);
        rnd=delete_step_r(rnd,p);
        root=new node(p);
        root->left=lnd;
        root->right=rnd;
        lnd->ptr=root;
        root->ptr=rnd;
      }
    }
    Point<T>query(Point<T>p){
      root=top_down_splay(root,[&](node *nd)->int {
        if(nd->ptr&&dot(nd->p,p)<dot(nd->ptr->p,p))return nd->right?1:0;
        else return nd->left?-1:0;
      });
      if(!root->ptr)return root->p;
      else return dot(root->p,p)<dot(root->ptr->p,p)?root->ptr->p:root->p;
    }
    ~lower_hull(){if(root)delete root;}
  };
  lower_hull lower,upper;
public:
  CHTxy(){}
  void add(Point<T>p){
    lower.add(p);
    upper.add(-p);
  }
  Point<T>max(Point<T>p){
    if(p.y<0||(p.x>=0&&p.y==0))return lower.query(p);
    else return -upper.query(-p);
  }
  Point<T>min(Point<T>p){return this->max(-p);}
  bool empty()const{return lower.empty();}
};
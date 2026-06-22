#pragma once
#include<vector>
template<typename T>
struct SubtreeSum{
private:
  struct node{
    node *left,*right,*par;
    T sum;
    bool rev;
    node():left(nullptr),right(nullptr),par(nullptr),sum(T()),rev(false){}
    node(node*nil):left(nil),right(nil),par(nil),sum(T()),rev(false){}
    inline void reverse(){
      std::swap(left,right);
      rev^=1;
    }
    inline void push(){
      if(rev){
        left->reverse();
        right->reverse();
        rev=false;
      }
    }
  };
  void splay(node *nd){
    while(nd->par!=&nil){
      node *p=nd->par;
      node *pp=p->par;
      if(pp!=&nil)pp->push();
      p->push();
      nd->push();
      if(p->left==nd){
        if(pp->left==p){
          pp->sum=pp->sum-p->sum+p->right->sum;
          p->sum=p->sum-nd->sum+nd->right->sum-p->right->sum+pp->sum;
          nd->sum=nd->sum-nd->right->sum+p->sum;
          nd->par=pp->par;
          if(pp->par->left==pp)nd->par->left=nd;
          else if(pp->par->right==pp)nd->par->right=nd;
          pp->left=p->right;
          pp->left->par=pp;
          pp->par=p;
          p->right=pp;
          p->left=nd->right;
          p->left->par=p;
          p->par=nd;
          nd->right=p;
        }
        else if(pp->right==p){
          T sub=pp->sum;
          pp->sum=pp->sum-p->sum+nd->left->sum;
          p->sum=p->sum-nd->sum+nd->right->sum;
          nd->sum=sub;
          nd->par=pp->par;
          if(pp->par->left==pp)nd->par->left=nd;
          else if(pp->par->right==pp)nd->par->right=nd;
          pp->right=nd->left;
          pp->right->par=pp;
          pp->par=nd;
          p->left=nd->right;
          p->left->par=p;
          p->par=nd;
          nd->left=pp;
          nd->right=p;
        }
        else{
          p->sum=p->sum-nd->sum+nd->right->sum;
          nd->sum=nd->sum-nd->right->sum+p->sum;
          nd->par=p->par;
          p->left=nd->right;
          p->left->par=p;
          p->par=nd;
          nd->right=p;
        }
      }
      else if(p->right==nd){
        if(pp->left==p){
          T sub=pp->sum;
          pp->sum=pp->sum-p->sum+nd->right->sum;
          p->sum=p->sum-nd->sum+nd->left->sum;
          nd->sum=sub;
          nd->par=pp->par;
          if(pp->par->left==pp)nd->par->left=nd;
          else if(pp->par->right==pp)nd->par->right=nd;
          pp->left=nd->right;
          pp->left->par=pp;
          pp->par=nd;
          p->right=nd->left;
          p->right->par=p;
          p->par=nd;
          nd->left=p;
          nd->right=pp;
        }
        else if(pp->right==p){
          pp->sum=pp->sum-p->sum+p->left->sum;
          p->sum=p->sum-nd->sum+nd->left->sum-p->left->sum+pp->sum;
          nd->sum=nd->sum-nd->left->sum+p->sum;
          nd->par=pp->par;
          if(pp->par->left==pp)nd->par->left=nd;
          else if(pp->par->right==pp)nd->par->right=nd;
          pp->right=p->left;
          pp->right->par=pp;
          pp->par=p;
          p->left=pp;
          p->right=nd->left;
          p->right->par=p;
          p->par=nd;
          nd->left=p;
        }
        else{
          p->sum=p->sum-nd->sum+nd->left->sum;
          nd->sum=nd->sum-nd->left->sum+p->sum;
          nd->par=p->par;
          p->right=nd->left;
          p->right->par=p;
          p->par=nd;
          nd->left=p;
        }
      }
      else break;
    }
    nd->push();
  }
  void expose(node *nd){
    splay(nd);
    nd->right=&nil;
    while(nd->par!=&nil){
      node *p=nd->par;
      splay(p);
      p->sum=p->sum-nd->sum+nd->left->sum;
      nd->sum=nd->sum-nd->left->sum+p->sum;
      nd->par=p->par;
      p->right=nd->left;
      p->right->par=p;
      p->par=nd;
      nd->left=p;
    }
  }
  node nil;
  std::vector<node>nds;
public:
  SubtreeSum(){}
  explicit SubtreeSum(int n):nil(),nds(n,node(&nil)){}
  void link(int u,int v){
    expose(&nds[u]);
    expose(&nds[v]);
    nds[v].reverse();
    nds[v].par=&nds[u];
    nds[u].sum+=nds[v].sum;
  }
  void cut(int u,int v){
    expose(&nds[u]);
    nds[u].reverse();
    expose(&nds[v]);
    nds[v].sum-=nds[v].left->sum;
    nds[v].left->par=&nil;
    nds[v].left=&nil;
  }
  void add(int u,T x){
    expose(&nds[u]);
    nds[u].sum+=x;
  }
  T sum(int u){
    expose(&nds[u]);
    return nds[u].sum;
  }
};
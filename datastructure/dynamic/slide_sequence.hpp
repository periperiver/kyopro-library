#pragma once
#include<cassert>
#include "../erasable_priority_queue.hpp"
#include "splay.hpp"
template<typename I,typename T>
struct SlideSequence{
private:
  struct node{
    node *left,*right,*par,*pre,*nxt;
    I len,sz;
    T val;
    int sign,sum;
    node():left(nullptr),right(nullptr),par(nullptr),pre(nullptr),nxt(nullptr),len(0),sz(0),val(),sign(0),sum(0){}
    node(I len,T val):left(nullptr),right(nullptr),par(nullptr),pre(nullptr),nxt(nullptr),len(len),sz(len),val(val),sign(0),sum(0){}
    void update(){
      sz=len;
      sum=sign;
      if(left)sz+=left->sz,sum+=left->sum;
      if(right)sz+=right->sz,sum+=right->sum;
    }
    ~node(){
      if(left)delete left;
      if(right)delete right;
    }
  };
  ErasablePriorityQueue<std::pair<I,node*>,std::vector<std::pair<I,node*>>,std::greater<std::pair<I,node*>>>a,b;
  node *root;
  I lazy;
  void eval(node *nd){
    if(nd->pre&&nd->nxt){
      if(nd->pre->val<=nd->val&&nd->val>nd->nxt->val){
        nd->sign++;
        a.emplace(nd->len-=lazy,nd);
        nd->update();
      }
      if(nd->pre->val>nd->val&&nd->val<=nd->nxt->val){
        nd->sign--;
        b.emplace(nd->len+=lazy,nd);
        nd->update();
      }
    }
  }
  void eval_erase(node *nd){
    if(nd->pre&&nd->nxt){
      if(nd->pre->val<=nd->val&&nd->val>nd->nxt->val){
        nd->sign--;
        a.emplace_erase(nd->len,nd);
        nd->len+=lazy;
        nd->update();
      }
      if(nd->pre->val>nd->val&&nd->val<=nd->nxt->val){
        nd->sign++;
        b.emplace_erase(nd->len,nd);
        nd->len-=lazy;
        nd->update();
      }
    }
  }
  void merge(node *lnd,node *rnd){
    if(!lnd)root=rnd;
    else if(!rnd)root=lnd;
    else{
      lnd=right_most(lnd),rnd=left_most(rnd);
      lnd->nxt=rnd;
      rnd->pre=lnd;
      eval(lnd),eval(rnd);
      lnd->right=rnd;
      rnd->par=lnd;
      lnd->update();
      root=lnd;
    }
  }
  void change_reset(node *nd){
    if(nd->left){
      nd->left->par=nullptr;
      nd->left=right_most(nd->left);
      nd->left->par=nd;
      eval_erase(nd->left);
    }
    if(nd->right){
      nd->right->par=nullptr;
      nd->right=left_most(nd->right);
      nd->right->par=nd;
      eval_erase(nd->right);
    }
    eval_erase(nd);
  }
public:
  SlideSequence():root(nullptr),lazy(0){}
  I size()const{return root?root->sz+lazy*root->sum:0;}
  void add_left(I len,T val){this->merge(new node(len,val),root);}
  void add_right(I len,T val){this->merge(root,new node(len,val));}
  void replace_range_min(I len){
    assert(0<=len&&len<size());
    while(!a.empty()){
      auto [l,nd]=a.top();
      if(l+lazy>len)break;
      a.pop();
      splay(nd);
      nd->left->par=nd->right->par=nullptr;
      nd->left=nd->right=nullptr;
      node *lnd=nd->pre,*rnd=nd->nxt;
      splay(lnd),splay(rnd);
      eval_erase(lnd),eval_erase(rnd);
      lnd->nxt=rnd,rnd->pre=lnd;
      if(lnd->val<=rnd->val)rnd->len+=l+lazy;
      else lnd->len+=l+lazy;
      rnd->update();
      lnd->update();
      eval(lnd),eval(rnd);
      lnd->right=rnd;
      rnd->par=lnd;
      lnd->update();
      root=lnd;
    }
    root=left_most(root);
    while(root->len<=len&&root->val>root->nxt->val){
      node *rnd=root->right;
      rnd->par=nullptr;
      rnd=left_most(rnd);
      eval_erase(rnd);
      rnd->len+=root->len;
      root->right=nullptr;
      delete root;
      rnd->pre=nullptr;
      root=rnd;
      root->update();
    }
    root=right_most(root);
    while(root->len<=len&&root->pre->val<=root->val){
      node *lnd=root->left;
      lnd->par=nullptr;
      lnd=right_most(lnd);
      eval_erase(lnd);
      lnd->len+=root->len;
      root->left=nullptr;
      delete root;
      lnd->nxt=nullptr;
      root=lnd;
      root->update();
    }
    if(!root->pre||root->pre->val<=root->val){
      root->len-=len;
      root->update();
    }
    if(root->pre){
      root=left_most(root);
      if(root->val>root->nxt->val){
        root->len-=len;
        root->update();
      }
    }
    lazy-=len;
  }
  void replace_range_max(I len){
    assert(0<=len&&len<size());
    while(!b.empty()){
      auto [l,nd]=b.top();
      if(l-lazy>len)break;
      b.pop();
      splay(nd);
      nd->left->par=nd->right->par=nullptr;
      nd->left=nd->right=nullptr;
      node *lnd=nd->pre,*rnd=nd->nxt;
      splay(lnd),splay(rnd);
      eval_erase(lnd),eval_erase(rnd);
      lnd->nxt=rnd,rnd->pre=lnd;
      if(lnd->val<=rnd->val)lnd->len+=l-lazy;
      else rnd->len+=l-lazy;
      rnd->update();
      lnd->update();
      eval(lnd),eval(rnd);
      lnd->right=rnd;
      rnd->par=lnd;
      lnd->update();
      root=lnd;
    }
    root=left_most(root);
    while(root->len<=len&&root->val<=root->nxt->val){
      node *rnd=root->right;
      rnd->par=nullptr;
      rnd=left_most(rnd);
      eval_erase(rnd);
      rnd->len+=root->len;
      root->right=nullptr;
      delete root;
      rnd->pre=nullptr;
      root=rnd;
      root->update();
    }
    root=right_most(root);
    while(root->len<=len&&root->pre->val>root->val){
      node *lnd=root->left;
      lnd->par=nullptr;
      lnd=right_most(lnd);
      eval_erase(lnd);
      lnd->len+=root->len;
      root->left=nullptr;
      delete root;
      lnd->nxt=nullptr;
      root=lnd;
      root->update();
    }
    if(!root->pre||root->pre->val>root->val){
      root->len-=len;
      root->update();
    }
    if(root->pre){
      root=left_most(root);
      if(root->val<=root->nxt->val){
        root->len-=len;
        root->update();
      }
    }
    lazy+=len;
  }
  T get(I k){
    assert(0<=k&&k<size());
    while(true){
      I lsz=root->left?root->left->sz+root->left->sum*lazy:0;
      if(k<lsz)root=root->left;
      else{
        k-=lsz;
        I msz=root->len+root->sign*lazy;
        if(k<msz)break;
        else k-=msz,root=root->right;
      }
    }
    splay(root);
    return root->val;
  }
  void set(I k,T val){
    assert(0<=k&&k<size());
    get(k);
    change_reset(root);
    if(root->len==1){
      root->val=val;
      root->update();
      if(root->left)eval(root->left);
      if(root->right)eval(root->right);
      eval(root);
      root->update();
    }
    else{
      k-=root->left?root->left->sz+root->left->sum*lazy:0;
      if(k==0){
        root->len--;
        node *ch=root->left;
        root->left=nullptr;
        node *nd=new node(1,val);
        nd->pre=ch;
        if(ch)ch->nxt=nd;
        root->pre=nd;
        nd->nxt=root;
        if(ch)eval(ch);
        if(root->right)eval(root->right);
        root->update();
        eval(root);
        nd->left=ch;
        if(ch)ch->par=nd;
        nd->right=root;
        root->par=nd;
        nd->update();
        eval(nd);
        root=nd;
      }
      else if(k+1==root->len){
        root->len--;
        node *ch=root->right;
        root->right=nullptr;
        node *nd=new node(1,val);
        nd->nxt=ch;
        if(ch)ch->pre=nd;
        root->nxt=nd;
        nd->pre=root;
        if(ch)eval(ch);
        if(root->left)eval(root->left);
        root->update();
        eval(root);
        nd->right=ch;
        if(ch)ch->par=nd;
        nd->left=root;
        root->par=nd;
        nd->update();
        eval(nd);
        root=nd;
      }
      else{
        node *ch=root->left;
        root->left=nullptr;
        root->len-=k+1;
        node *lnd=new node(k,root->val);
        node *nd=new node(1,val);
        lnd->pre=ch;
        if(ch)ch->nxt=lnd;
        nd->pre=lnd;
        lnd->nxt=nd;
        root->pre=nd;
        nd->nxt=root;
        if(ch)eval(ch);
        if(root->right)eval(root->right);
        root->update();
        eval(root);
        lnd->left=ch;
        if(ch)ch->par=lnd;
        lnd->update();
        eval(lnd);
        nd->left=lnd;
        lnd->par=nd;
        nd->right=root;
        root->par=nd;
        nd->update();
        eval(nd);
        root=nd;
      }
    }
  }
  ~SlideSequence(){if(root)delete root;}
};
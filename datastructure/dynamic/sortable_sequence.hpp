#pragma once
#include<type_traits>
#include "binary_trie.hpp"
template<typename I,typename M>
struct SortableSequence{
private:
  using BT=BinaryTrie<I,M>;
  using S=typename M::S;
  struct node{
    static node nil;
    node *left=nullptr,*right=nullptr,*par=this;
    BT bt;
    int sz=0;
    S v=M::e();
    bool rev=false,chrev=false;
    node():bt(),left(this),right(this),par(this){}
    node(const I&key,const S&x):bt(),left(&nil),right(&nil),sz(1){
      bt.set(key,x);
    }
    node(const BT&bt_):bt(bt_),left(&nil),right(&nil),sz(bt.size()){}
    inline void update(){
      v=bt.all_prod();
      if(chrev)M::revS(v);
      v=M::op(left->v,M::op(v,right->v));
      sz=left->sz+bt.size()+right->sz;
    }
    inline void reverse(){
      if(this!=&nil){
        std::swap(left,right);
        rev^=1;
        chrev^=1;
        M::revS(v);
      }
    }
    inline void push(){
      if(rev){
        left->reverse();
        right->reverse();
        rev=false;
      }
    }
  };
  std::pair<node*,node*>split(node *nd,int k){
    if(k==0)return std::make_pair(&node::nil,nd);
    if(k==nd->sz)return std::make_pair(nd,&node::nil);
    node *lnd=&node::nil,*rnd=&node::nil;
    while(true){
      nd->push();
      if(nd->left->sz>k){
        node *ch=nd->left;
        ch->push();
        if(ch->left->sz>k){
          nd->left=ch->right;
          ch->right=nd;
          rnd->left=ch;
          ch->par=rnd;
          rnd=rnd->left;
          nd->update();
          nd=ch->left;
        }
        else if(ch->left->sz+ch->bt.size()<=k){
          k-=ch->left->sz+ch->bt.size();
          lnd->right=ch;
          ch->par=lnd;
          lnd=lnd->right;
          rnd->left=nd;
          nd->par=rnd;
          rnd=rnd->left;
          nd=ch->right;
        }
        else{
          k-=ch->left->sz;
          rnd->left=nd;
          nd->par=rnd;
          rnd=rnd->left;
          nd=ch;
          break;
        }
      }
      else if(nd->left->sz+nd->bt.size()<=k){
        k-=nd->left->sz+nd->bt.size();
        node *ch=nd->right;
        ch->push();
        if(ch->left->sz>k){
          lnd->right=nd;
          nd->par=lnd;
          lnd=lnd->right;
          rnd->left=ch;
          ch->par=rnd;
          rnd=rnd->left;
          nd=ch->left;
        }
        else if(ch->left->sz+ch->bt.size()<=k){
          k-=ch->left->sz+ch->bt.size();
          nd->right=ch->left;
          ch->left=nd;
          lnd->right=ch;
          ch->par=lnd;
          lnd=lnd->right;
          nd->update();
          nd=ch->right;
        }
        else{
          k-=ch->left->sz;
          lnd->right=nd;
          nd->par=lnd;
          lnd=lnd->right;
          nd=ch;
          break;
        }
      }
      else{
        k-=nd->left->sz;
        break;
      }
    }
    lnd->right=nd->left;
    while(lnd!=&node::nil){
      lnd->update();
      lnd=lnd->par;
    }
    rnd->left=nd->right;
    while(rnd!=&node::nil){
      rnd->update();
      rnd=rnd->par;
    }
    if(k==0){
      node *res=node::nil.right;
      nd->left=&node::nil;
      nd->right=node::nil.left;
      nd->update();
      return std::make_pair(res,nd);
    }
    auto [lbt,rbt]=nd->bt.split(nd->chrev?nd->bt.size()-k:k);
    node *res=new node(lbt);
    nd->bt=rbt;
    if(nd->chrev)std::swap(nd->bt,res->bt);
    res->left=node::nil.right;
    nd->left=&node::nil;
    nd->right=node::nil.left;
    res->chrev=nd->chrev;
    res->update();
    nd->update();
    return std::make_pair(res,nd);
  }
  node* meld(node *nd){
    auto dfs=[](auto self,node *nd)->BT {
      if(nd->left!=&node::nil&&nd->right!=&node::nil){
        BT l=self(self,nd->left);
        BT r=self(self,nd->right);
        delete(nd->left);
        delete(nd->right);
        return BT::meld(l,BT::meld(nd->bt,r));
      }
      else if(nd->left!=&node::nil){
        BT res=BT::meld(self(self,nd->left),nd->bt);
        delete(nd->left);
        return res;
      }
      else if(nd->right!=&node::nil){
        BT res=BT::meld(nd->bt,self(self,nd->right));
        delete(nd->right);
        return res;
      }
      else{
        return nd->bt;
      }
    };
    nd->bt=dfs(dfs,nd);
    nd->left=&node::nil;
    nd->right=&node::nil;
    nd->v=nd->bt.all_prod();
    nd->sz=nd->bt.size();
    node::nil.left=&node::nil;
    node::nil.right=&node::nil;
    return nd;
  }
  node* merge(node *lhs,node *rhs){
    if(lhs==&node::nil)return rhs;
    if(rhs==&node::nil)return lhs;
    node *lnd=&node::nil;
    while(lhs->right!=&node::nil){
      lhs->push();
      node *ch=lhs->right;
      if(ch->right!=&node::nil){
        ch->push();
        lhs->right=ch->left;
        ch->left=lhs;
        lnd->right=ch;
        ch->par=lnd;
        lnd=lnd->right;
        lhs->update();
        lhs=ch->right;
      }
      else{
        lnd->right=lhs;
        lhs->par=lnd;
        lnd=lnd->right;
        lhs=ch;
      }
    }
    lhs->push();
    lnd->right=lhs->left;
    while(lnd!=&node::nil){
      lnd->update();
      lnd=lnd->par;
    }
    lhs->left=node::nil.right;
    lhs->right=rhs;
    lhs->update();
    return lhs;
  }
  node *root;
public:
  SortableSequence():root(&node::nil){}
  void insert(int i,const I&key,const S&val){
    auto [lnd,rnd]=split(root,i);
    root=new node(key,val);
    root->left=lnd;
    root->right=rnd;
    root->update();
  }
  void erase(int i){
    auto [lnd,R]=split(root,i);
    auto [mnd,rnd]=split(R,1);
    delete(mnd);
    root=merge(lnd,rnd);
  }
  void set(int i,const I&key,const S&val){
    auto [lnd,R]=split(root,i);
    auto [mnd,rnd]=split(R,1);
    mnd->bt.clear();
    mnd->bt.set(key,val);
    mnd->left=lnd;
    mnd->right=rnd;
    mnd->update();
    root=mnd;
  }
  S prod(int l,int r){
    auto [lnd,R]=split(root,l);
    auto [mnd,rnd]=split(R,r-l);
    S res=mnd->v;
    root=merge(lnd,merge(mnd,rnd));
    return res;
  }
  void sort(int l,int r){
    auto [lnd,R]=split(root,l);
    auto [mnd,rnd]=split(R,r-l);
    mnd=meld(mnd);
    mnd->chrev=false;
    root=merge(lnd,merge(mnd,rnd));
  }
  void reverse(int l,int r){
    auto [lnd,R]=split(root,l);
    auto [mnd,rnd]=split(R,r-l);
    mnd->reverse();
    root=merge(lnd,merge(mnd,rnd));
  }
  inline S get(int i){
    return prod(i,i+1);
  }
};
template<typename I,typename M>typename SortableSequence<I,M>::node SortableSequence<I,M>::node::nil;
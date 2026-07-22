#pragma once
#include<concepts>
#include<vector>
#include<ranges>
#include<cassert>
#include "../math/modint.hpp"
#include "../math/primitive_root.hpp"
#include "dynamic/splay.hpp"
struct DataDependentTree{
// private:
  static mint61 base;
  struct inc;
  struct dup;
  struct inc{
    inc *left,*right,*par;
    dup *head,*tail,*d_par;
    mint61 val;
    int sz;
    inc():left(nullptr),right(nullptr),par(nullptr),head(nullptr),tail(nullptr),d_par(nullptr),val(),sz(0){}
    template<std::integral T>
    inc(const T&x):left(nullptr),right(nullptr),par(nullptr),head(nullptr),tail(nullptr),d_par(nullptr),val(x),sz(1){}
    inc(dup *d):inc(d,d){}
    inc(dup *d1,dup *d2):left(nullptr),right(nullptr),par(nullptr),head(d1),tail(d2),d_par(nullptr),val(),sz(1){
      while(true){
        val=val*base+d1->val;
        d1->i_par=this;
        if(d1==d2)break;
        d1=d1->next;
      }
    }
    void update(){
      sz=1;
      if(left)sz+=left->sz;
      if(right)sz+=right->sz;
    }
    int size()const{
      if(!head)return 1;
      int res=0;
      for(dup*nd=head;nd!=tail;nd=nd->next)res+=nd->sz;
      res+=tail->sz;
      return res;
    }
    ~inc(){
      if(left)delete left;
      if(right)delete right;
      for(dup*nd=head;nd!=tail;nd=nd->next)delete nd;
      if(tail)delete tail;
    }
  };
  struct dup{
    inc *ch,*i_par;
    dup *prev,*next;
    mint61 val;
    int sz;
    dup():ch(nullptr),i_par(nullptr),prev(nullptr),next(nullptr),val(),sz(0){}
    dup(inc *i):ch(i),i_par(nullptr),prev(nullptr),next(nullptr),val(i->val*base+mint61::raw(i->sz*i->size())),sz(i->sz*i->size()){ch->d_par=this;}
    inc* lower_bound(int k){
      assert(0<=k&&k<=ch->sz);
      while(true){
        assert(ch);
        int lsz=ch->left?ch->left->sz:0;
        if(k<lsz)ch=ch->left;
        else if(k<lsz+1)break;
        else k-=lsz+1,ch=ch->right;
      }
      splay(ch);
      ch->d_par=this;
      return ch;
    }
    ~dup(){
      if(ch)delete ch;
    }
  };
  static constexpr unsigned long long mix(mint61 x,int seed){
    unsigned long long v=x.val()+seed;
    v+=0x9e3779b97f4a7c15ull;
    v=(v^(v>>30))*0xbf58476d1ce4e5b9ull;
    v=(v^(v>>27))*0x94d049bb133111ebull;
    return v;
  }
  static constexpr bool check(mint61 x,mint61 y,int seed){
    return mix(x,seed)<mix(y,seed);
  }
  static int get_level(inc*nd){
    int res=0;
    while(nd->head)nd=nd->head->lower_bound(0),res++;
    return res;
  }
  static inc* merge(inc *lnd,inc *rnd){
    while(lnd->head)lnd=lnd->tail->lower_bound(lnd->tail->sz-1);
    while(rnd->head)rnd=rnd->head->lower_bound(0);
    while(lnd->d_par||rnd->d_par){
      if(!lnd->d_par)lnd=new inc(new dup(lnd));
      if(!rnd->d_par)rnd=new inc(new dup(rnd));
      dup *d1=lnd->d_par,*d2=rnd->d_par;
    }
    return nullptr;
  }
  static std::pair<inc*,inc*>split(inc *nd,int k){
    return {nullptr,nullptr};
  }
  static int lcp(inc *lnd,inc *rnd){
    int res=0;
    int levell=get_level(lnd),levelr=get_level(rnd);
    while(levell<levelr)rnd=rnd->head->lower_bound(0),levelr--;
    while(levell>levelr)lnd=lnd->head->lower_bound(0),levell--;
    if(lnd->val==rnd->val)return lnd->sz;
    while(lnd->head){
      dup *d1=lnd->head,*d2=rnd->head;
      while(d1&&d2&&d1->val==d2->val){
        res+=d1->sz;
        d1=d1->next;
        d2=d2->next;
      }
      if(!d1||!d2)break;
      if(d1->ch->val==d2->ch->val){
        assert(d1->sz!=d2->sz);
        if(d1->sz>d2->sz)std::swap(d1,d2);
        res+=d1->sz;
        if(!d1->next)break;
        lnd=d1->next->lower_bound(0);
        rnd=d2->lower_bound(d1->ch->sz);
      }
      else{
        lnd=d1->lower_bound(0);
        rnd=d2->lower_bound(0);
      }
    }
    return res;
  }
  inc*root;
public:
  DataDependentTree():root(nullptr){}
  template<std::integral T>
  explicit DataDependentTree(const T&x):root(new inc(x)){}
  template<std::integral T>
  explicit DataDependentTree(const std::vector<T>&init){
    if(init.empty()){
      root=nullptr;
      return;
    }
    std::vector<inc*>nds(init.size());
    for(const auto&[i,v]:init|std::views::enumerate)nds[i]=new inc(v);
    int level=0;
    while((int)nds.size()>1){
      std::vector<dup*>nds2;
      for(int i=0;i<(int)nds.size();){
        int j=i+1;
        while(j<(int)nds.size()&&nds[i]->val==nds[j]->val){
          nds[j]->left=nds[j-1];
          nds[j-1]->par=nds[j];
          nds[j]->update();
          j++;
        }
        nds2.emplace_back(new dup(nds[j-1]));
        i=j;
      }
      for(int i=0;i<(int)nds2.size()-1;i++){
        nds2[i]->next=nds2[i+1];
        nds2[i+1]->prev=nds2[i];
      }
      std::vector<inc*>nds3;
      for(int i=0;i<(int)nds2.size();){
        int j=i+1;
        while(j<(int)nds2.size()&&check(nds2[j-1]->val,nds2[j]->val,level))j++;
        nds3.emplace_back(new inc(nds2[i],nds2[j-1]));
        i=j;
      }
      level++;
      nds=std::move(nds3);
    }
    root=nds[0];
  }
  explicit DataDependentTree(const std::string&s):DataDependentTree([](const std::string&s){
    std::vector<char>res(s.size());
    for(const auto&[i,c]:s|std::views::enumerate)res[i]=c;
    return res;
  }(s)){}
  void merge(DataDependentTree ddt){
    if(!root)root=ddt.root;
    else if(ddt.root)root=merge(root,ddt.root);
  }
  std::pair<DataDependentTree,DataDependentTree>split(int k){
    auto [lnd,rnd]=split(root,k);
    root=nullptr;
    DataDependentTree resl,resr;
    resl.root=lnd;
    resr.root=rnd;
    return std::make_pair(resl,resr);
  }
  int lcp(DataDependentTree ddt){
    if(!root||!ddt.root)return 0;
    return lcp(root,ddt.root);
  }
};
mint61 DataDependentTree::base=12345;//primitive_root(mint61::mod());

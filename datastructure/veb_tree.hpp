#pragma once
#include<vector>
#include<cassert>
#include<cstdint>
#include "math/util.hpp"
struct VanEmdeBoasTree{
private:
  int n;
  int l,r;
  int s,sl;
  int mask;
  using u64=uint64_t;
  struct veb3{
    u64 aux=0;
    inline void insert(int x){aux|=u64(1)<<x;}
    inline void erase(int x){aux&=~(u64(1)<<x);}
    inline bool empty()const{return aux==0;}
    inline void clear(){aux=0;}
    inline int predecessor(int x)const{
      if(x<0)return -1;
      u64 sh=aux<<(63-x);
      return sh==0?-1:x-__builtin_clzll(sh);
    }
    inline int successor(int x)const{
      u64 sh=aux>>x;
      return sh==0?64:x+__builtin_ctzll(sh);
    }
    inline bool contains(int x)const{return aux>>x&1;}
  };
  struct veb2{
    int l,r;
    int s,sl;
    int mask;
    int mn,mx;
    std::vector<veb3>child;
    veb3 aux;
    veb2(){}
    veb2(int log2n):l(log2n/2),r((log2n+1)/2),s(1<<log2n),sl(1<<(log2n/2)),mask((1<<((log2n+1)/2))-1),mn(1<<log2n),mx(-1){
      child.resize(sl);
    }
    void insert(int x){
      if(x<=mn){
        if(x==mn)return;
        std::swap(x,mn);
        if(x==s)mx=mn;
        if(x>=mx)return;
      }
      else if(x>=mx){
        if(x==mx)return;
        std::swap(x,mx);
        if(x<=mn)return;
      }
      int nx=x>>r;
      if(child[nx].empty())aux.insert(nx);
      child[nx].insert(x&mask);
    }
    void erase(int x){
      if(x<=mn){
        if(x<mn)return;
        x=mn=successor(mn+1);
        if(x>=mx){
          if(x>mx)mx=-1;
          return;
        }
      }
      else if(x>=mx){
        if(x>mx)return;
        x=mx=predecessor(mx-1);
        if(x<=mn)return;
      }
      int nx=x>>r;
      child[nx].erase(x&mask);
      if(child[nx].empty())aux.erase(nx);
    }
    bool empty()const{return mx<mn;}
    void clear(){
      mn=s,mx=-1;
      for(int i=0;i<sl;i++)child[i].clear();
      aux.clear();
    }
    int predecessor(int x)const{
      if(x>=mx)return mx;
      if(x<mn)return -1;
      int nx=x>>r;
      int res=child[nx].predecessor(x&mask);
      if(res>=0)return (nx<<r)+res;
      nx=aux.predecessor(nx-1);
      return nx<0?mn:((nx<<r)+child[nx].predecessor(mask));
    }
    int successor(int x)const{
      if(x<=mn)return mn;
      if(x>mx)return s;
      int nx=x>>r;
      int res=child[nx].successor(x&mask);
      if(res<=mask)return (nx<<r)+res;
      nx=aux.successor(nx+1);
      return nx>=sl?mx:((nx<<r)+child[nx].successor(0));
    }
    inline bool contains(int x)const{return successor(x)==x;}
  };
  std::vector<veb2>child;
  veb2 aux;
  int mn,mx;
  int N;
public:
  VanEmdeBoasTree():mn(0),mx(-1){}
  VanEmdeBoasTree(int n):N(n){
    if(n<16)n=16;
    n=ceil_pow2(n);
    int log2n=msb(n);
    assert(n<=(1<<24));
    mn=-1;
    mx=n;
    l=log2n/2,r=(log2n+1)/2;
    s=1<<log2n,sl=1<<l;
    mask=(1<<r)-1;
    aux=veb2(l);
    child.resize(sl,veb2(r));
  }
  void insert(int x){
    if(x<=mn){
      if(x==mn)return;
      std::swap(x,mn);
      if(x==s)mx=mn;
      if(x>=mx)return;
    }
    else if(x>=mx){
      if(x==mx)return;
      std::swap(x,mx);
      if(x<=mn)return;
    }
    int nx=x>>r;
    if(child[nx].empty())aux.insert(nx);
    child[nx].insert(x&mask);
  }
  void erase(int x){
    if(x<=mn){
      if(x<mn)return;
      x=mn=successor(mn+1);
      if(x>=mx){
        if(x>mx)mx=-1;
        return;
      }
    }
    else if(x>=mx){
      if(x>mx)return;
      x=mx=predecessor(mx-1);
      if(x<=mn)return;
    }
    int nx=x>>r;
    child[nx].erase(x&mask);
    if(child[nx].empty())aux.erase(nx);
  }
  void clear(){
    mn=s,mx=-1;
    for(int i=0;i<sl;i++)child[i].clear();
    aux.clear();
  }
  int predecessor(int x)const{
    if(x>=mx)return mx;
    if(x<mn)return -1;
    int nx=x>>r;
    int res=child[nx].predecessor(x&mask);
    if(res>=0)return (nx<<r)+res;
    nx=aux.predecessor(nx-1);
    return nx<0?mn:((nx<<r)+child[nx].predecessor(mask));
  }
  int successor(int x)const{
    if(x<=mn)return mn;
    if(x>mx)return N;
    int nx=x>>r;
    int res=child[nx].successor(x&mask);
    if(res<=mask)return std::min(N,(nx<<r)+res);
    nx=aux.successor(nx+1);
    return std::min(N,(nx>=sl?mx:((nx<<r)+child[nx].successor(0))));
  }
  inline bool contains(int x)const{return successor(x)==x;}
};
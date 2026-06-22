#pragma once
#include<cassert>
#include<type_traits>
using namespace std;
template<int b,typename en=void>
struct VanEmdeBoasTree{
private:
  using u64=uint64_t;
  static constexpr int l=b/2,r=(b+1)/2;
  static constexpr int s=1<<b,sl=1<<l;
  static constexpr int mask=(1<<r)-1;
  VanEmdeBoasTree<r>child[sl];
  VanEmdeBoasTree<l>aux;
  int mn=s,mx=-1;
public:
  void insert(int x){
    if(x<=mn){
      if(x==mn)return;
      swap(x,mn);
      if(x==s)mx=mn;
      if(x>=mx)return;
    }
    else if(x>=mx){
      if(x==mx)return;
      swap(x,mx);
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
    debug(x,res);
    if(res<=mask)return (nx<<r)+res;
    nx=aux.successor(nx+1);
    return nx>=sl?mx:((nx<<r)+child[nx].successor(0));
  }
  bool contains(int x)const{return successor(x)==x;}
};
template<int b>
struct VanEmdeBoasTree<b,enable_if_t<(0<=b&&b<=6)>>{
private:
  using u64=uint64_t;
  static constexpr int s=1<<b;
  u64 aux=0;
public:
  void insert(int x){aux|=u64(1)<<x;}
  void erase(int x){aux&=~(u64(1)<<x);}
  bool empty()const{return aux==0;}
  void clear(){aux=0;}
  int predecessor(int x)const{
    u64 sh=aux<<(63-x);
    return sh==0?-1:x-__builtin_clzll(sh);
  }
  int successor(int x)const{
    u64 sh=aux>>x;
    return sh==0?s:x+__builtin_ctzll(sh);
  }
  bool contains(int x)const{return aux>>x&1;}
};
#pragma once
#include<vector>
#include<numeric>
#include "math/util.hpp"
#warning このコード遅いよ
template<typename T>
struct RangeLinearAddRangeMin{
private:
  static constexpr std::pair<T,T>id=std::make_pair(0,0);
  static constexpr T inf=std::numeric_limits<T>::max();
  struct node{
    int li,ri;
    T lb,rb;
    std::pair<T,T>lazy;
    node(){}
    node(int i,T v):li(i),ri(i),lb(v),rb(v),lazy(0,0){}
    inline void propagate(const std::pair<T,T>&f){
      lb+=li*f.first+f.second;
      rb+=ri*f.first+f.second;
      lazy.first+=f.first;
      lazy.second+=f.second;
    }
  };
  static constexpr T cross(int ax,T ay,int bx,T by,int cx,T cy){
    return (bx-ax)*(cy-ay)-(cx-ax)*(by-ay);
  }
  std::vector<node>dat;
  int z;
  int log2n,n;
  inline void push(int i){
    dat[i*2].propagate(dat[i].lazy);
    dat[i*2+1].propagate(dat[i].lazy);
    dat[i].lazy=id;
  }
  void update(int i){
    int lid=i*2,rid=i*2+1;
    int m=msb(rid);
    int mid=(rid^(1<<m))<<(log2n-m);
    T l1,l2,r1,r2;
    T la=0,lb=0,ra=0,rb=0;
    #define pushl la+=dat[lid].lazy.first;lb+=dat[lid].lazy.second;
    #define pushr ra+=dat[rid].lazy.first;rb+=dat[rid].lazy.second;
    #define calcl l1=dat[lid].lb+dat[lid].li*la+lb;l2=dat[lid].rb+dat[lid].ri*la+lb;
    #define calcr r1=dat[rid].lb+dat[rid].li*ra+rb;r2=dat[rid].rb+dat[rid].ri*ra+rb;
    calcl
    calcr
    while(lid<z||rid<z){
      if(lid<z&&cross(dat[lid].li,l1,dat[lid].ri,l2,dat[rid].li,r1)<0){
        pushl
        lid=lid*2;
        calcl
      }
      else if(rid<z&&cross(dat[lid].ri,l2,dat[rid].li,r1,dat[rid].ri,r2)<0){
        pushr
        rid=rid*2+1;
        calcr
      }
      else if(lid>=z){
        pushr
        rid=rid*2;
        calcr
      }
      else if(rid>=z){
        pushl
        lid=lid*2+1;
        calcl
      }
      else{
        if(((l2-l1)*(mid-dat[lid].li)+l1*(dat[lid].ri-dat[lid].li))*(dat[rid].ri-dat[rid].li)<((r2-r1)*(mid-dat[rid].li)+r1*(dat[rid].ri-dat[rid].li))*(dat[lid].ri-dat[lid].li)){
          pushl
          lid=lid*2+1;
          calcl
        }
        else{
          pushr
          rid=rid*2;
          calcr
        }
      }
    }
    dat[i].li=dat[lid].li;
    dat[i].ri=dat[rid].ri;
    dat[i].lb=l1;
    dat[i].rb=r1;
    #undef pushl
    #undef pushr
    #undef calcl
    #undef calcr
  }
  inline void path_push(int i){
    int l=lsb(i);
    for(int j=log2n;j>l;j--)push(i>>j);
  }
  std::pair<int,T> get_min(int i){
    while(i<z){
      push(i);
      if(dat[i].lb<dat[i].rb)i=i*2;
      else i=i*2+1;
    }
    return std::make_pair(dat[i].li,dat[i].lb);
  }
public:
  RangeLinearAddRangeMin(){}
  explicit RangeLinearAddRangeMin(int n_):RangeLinearAddRangeMin(std::vector<T>(n_)){}
  explicit RangeLinearAddRangeMin(const std::vector<T>&init):n(init.size()),dat(ceil_pow2((int)init.size())*2){
    z=dat.size()>>1;
    log2n=msb(z);
    for(int i=0;i<init.size();i++)dat[z+i]=node(i,init[i]);
    for(int i=n;i<z;i++)dat[z+i]=node(i,0);
    for(int i=z-1;i>=1;i--)update(i);
  }
  void add(int l,int r,T a,T b){
    l+=z,r+=z;
    int l2=l,r2=r;
    std::pair<T,T>f(a,b);
    path_push(l),path_push(r);
    while(l<r){
      if(l&1)dat[l++].propagate(f);
      if(r&1)dat[--r].propagate(f);
      l>>=1,r>>=1;
    }
    l=l2,r=r2;
    int x=msb(l^r);
    if(x>log2n)x=log2n;
    for(int i=lsb(l)+1;i<=x;i++)update(l>>i);
    for(int i=lsb(r)+1;i<=x;i++)update(r>>i);
    for(int i=x+1;i<=log2n;i++)update(l>>i);
  }
  std::pair<int,T> min(int l,int r){
    l+=z,r+=z;
    path_push(l),path_push(r);
    std::pair<int,T>res=std::make_pair(-1,inf);
    while(l<r){
      if(l&1){
        std::pair<int,T>v=get_min(l++);
        if(res.second>v.second)res=v;
      }
      if(r&1){
        std::pair<int,T>v=get_min(--r);
        if(res.second>v.second)res=v;
      }
      l>>=1,r>>=1;
    }
    return res;
  }
};
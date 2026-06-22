#pragma once
#include<iostream>
#include<vector>
#include<limits>
#include "../math/util.hpp"
template<typename T>
struct SegmentTreeBeats{
private:
  static constexpr T inf=std::numeric_limits<T>::max();
  static constexpr T minf=std::numeric_limits<T>::min();
  struct node{
    T sum,add;
    T mx,mx2,mn,mn2;
    int mxcnt,mncnt;
    node():sum(0),add(0),mx(minf),mx2(minf),mn(inf),mn2(inf),mxcnt(0),mncnt(0){}
    node(T x):sum(x),add(0),mx(x),mx2(minf),mn(x),mn2(inf),mxcnt(1),mncnt(1){}
  };
  std::vector<node>dat;
  int z,n,log2n;
  inline void update(int i){
    node&nd=dat[i];
    nd.sum=dat[i*2].sum+dat[i*2+1].sum;
    if(dat[i*2].mx==dat[i*2+1].mx){
      nd.mx=dat[i*2].mx;
      nd.mx2=std::max(dat[i*2].mx2,dat[i*2+1].mx2);
      nd.mxcnt=dat[i*2].mxcnt+dat[i*2+1].mxcnt;
    }
    else{
      int large=i*2+(dat[i*2].mx<dat[i*2+1].mx);
      nd.mx=dat[large].mx;
      nd.mx2=std::max(dat[large].mx2,dat[large^1].mx);
      nd.mxcnt=dat[large].mxcnt;
    }
    if(dat[i*2].mn==dat[i*2+1].mn){
      nd.mn=dat[i*2].mn;
      nd.mn2=std::min(dat[i*2].mn2,dat[i*2+1].mn2);
      nd.mncnt=dat[i*2].mncnt+dat[i*2+1].mncnt;
    }
    else{
      int small=i*2+(dat[i*2].mn>dat[i*2+1].mn);
      nd.mn=dat[small].mn;
      nd.mn2=std::min(dat[small].mn2,dat[small^1].mn);
      nd.mncnt=dat[small].mncnt;
    }
  }
  inline void push(int i){
    node&nd=dat[i];
    if(nd.add){
      propagate_add(i*2,nd.add);
      propagate_add(i*2+1,nd.add);
      nd.add=0;
    }
    if(nd.mx<dat[i*2].mx)propagate_chmin(i*2,nd.mx);
    if(nd.mn>dat[i*2].mn)propagate_chmax(i*2,nd.mn);
    if(nd.mx<dat[i*2+1].mx)propagate_chmin(i*2+1,nd.mx);
    if(nd.mn>dat[i*2+1].mn)propagate_chmax(i*2+1,nd.mn);
  }
  inline void propagate_add(int i,T v){
    node&nd=dat[i];
    nd.add+=v;
    nd.sum+=v<<(log2n-msb(i));
    nd.mx+=v;
    nd.mn+=v;
    if(nd.mx2!=minf)nd.mx2+=v;
    if(nd.mn2!=inf)nd.mn2+=v;
  }
  inline void propagate_chmin(int i,T v){
    node&nd=dat[i];
    nd.sum+=(v-nd.mx)*nd.mxcnt;
    if(nd.mn==nd.mx)nd.mn=v;
    else if(nd.mn2==nd.mx)nd.mn2=v;
    nd.mx=v;
  }
  inline void propagate_chmax(int i,T v){
    node&nd=dat[i];
    nd.sum+=(v-nd.mn)*nd.mncnt;
    if(nd.mx==nd.mn)nd.mx=v;
    else if(nd.mx2==nd.mn)nd.mx2=v;
    nd.mn=v;
  }
  inline void rec_chmin(int i,T v){
    if(dat[i].mx<=v)return;
    if(dat[i].mx2<=v)return propagate_chmin(i,v);
    push(i);
    rec_chmin(i*2,v);
    rec_chmin(i*2+1,v);
    update(i);
  }
  inline void rec_chmax(int i,T v){
    if(dat[i].mn>=v)return;
    if(dat[i].mn2>=v)return propagate_chmax(i,v);
    push(i);
    rec_chmax(i*2,v);
    rec_chmax(i*2+1,v);
    update(i);
  }
  template<int com>
  inline void propagate(int i,T v){
    static_assert(com==0||com==1||com==2);
    if constexpr(com==0)propagate_add(i,v);
    else if constexpr(com==1)rec_chmin(i,v);
    else rec_chmax(i,v);
  }
  void path_push(int i){
    int l=lsb(i);
    for(int j=log2n;j>l;j--)push(i>>j);
  }
  void path_update(int i){
    i>>=lsb(i);
    while(i>>=1)update(i);
  }
  template<int com>
  inline void op(T&x,const node&y){
    static_assert(com==0||com==1||com==2);
    if constexpr(com==0)x+=y.sum;
    else if constexpr(com==1)x=std::min(x,y.mn);
    else x=std::max(x,y.mx);
  }
  template<int com>
  inline T e(){
    static_assert(com==0||com==1||com==2);
    if constexpr(com==0)return 0;
    else if constexpr(com==1)return inf;
    else return minf;
  }
  template<int com>
  T prod(int l,int r){
    l+=z,r+=z;
    path_push(l),path_push(r);
    T res=e<com>();
    while(l<r){
      if(l&1)op<com>(res,dat[l++]);
      if(r&1)op<com>(res,dat[--r]);
      l>>=1,r>>=1;
    }
    return res;
  }
  template<int com>
  void apply(int l,int r,T v){
    l+=z,r+=z;
    path_push(l),path_push(r);
    int l2=l,r2=r;
    while(l<r){
      if(l&1)propagate<com>(l++,v);
      if(r&1)propagate<com>(--r,v);
      l>>=1,r>>=1;
    }
    path_update(l2),path_update(r2);
  }
public:
  SegmentTreeBeats(){}
  explicit SegmentTreeBeats(int n):SegmentTreeBeats(std::vector<T>(n)){}
  SegmentTreeBeats(int n,T v):SegmentTreeBeats(std::vector<T>(n,v)){}
  explicit SegmentTreeBeats(const std::vector<T>&init):n(init.size()){
    z=ceil_pow2(n);
    log2n=msb(z);
    dat.resize(z*2);
    for(int i=0;i<n;i++){
      dat[i+z].sum=dat[i+z].mx=dat[i+z].mn=init[i];
      dat[i+z].mxcnt=dat[i+z].mncnt=1;
    }
    for(int i=z;--i;)update(i);
  }
  inline T sum(int l,int r){return prod<0>(l,r);}
  inline T min(int l,int r){return prod<1>(l,r);}
  inline T max(int l,int r){return prod<2>(l,r);}
  inline void add(int l,int r,T v){apply<0>(l,r,v);}
  inline void chmin(int l,int r,T v){apply<1>(l,r,v);}
  inline void chmax(int l,int r,T v){apply<2>(l,r,v);}
  T get(int p){
    p+=z;
    for(int i=log2n;i>=1;i--)push(p>>i);
    return dat[p].sum;
  }
  friend std::ostream&operator<<(std::ostream&os,SegmentTreeBeats rhs){
    if(rhs.n==0){
      os<<"{}";
      return os;
    }
    os<<'{';
    for(int i=0;i<rhs.n;i++)os<<rhs.get(i)<<" }"[i+1==rhs.n];
    return os;
  }
};
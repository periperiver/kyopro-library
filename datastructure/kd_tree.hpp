#pragma once
#include<vector>
#include<map>
#include<algorithm>
#include<numeric>
#include "math/util.hpp"
template<typename I,typename M>
struct kdTree{
  static_assert(std::is_arithmetic_v<I>);
private:
  static constexpr I inf=std::numeric_limits<I>::max();
  static constexpr I minf=std::numeric_limits<I>::min();
  using S=typename M::S;
  using F=typename M::F;
  int z,log2n;
  std::vector<S>dat;
  std::vector<F>lazy;
  std::vector<std::tuple<I,I,I,I>>rect;
  std::vector<int>pos;
  void build(std::vector<std::tuple<I,I,S>>a){
    int n=a.size();
    z=ceil_pow2(n);
    log2n=msb(z);
    dat.resize(z*2,M::e());
    lazy.resize(z*2,M::id());
    rect.resize(z*2);
    a.resize(z,std::make_tuple(I(),I(),M::e()));
    std::vector<int>index(z);
    std::iota(index.begin(),index.end(),0);
    for(int i=1;i<z;i++){
      int m=msb(i);
      int l=(i^(1<<m))<<(log2n-m);
      int r=l+(1<<(log2n-msb(i)));
      if(i==1){
        I mnx=inf,mxx=minf,mny=inf,mxy=minf;
        for(const auto&[x,y,v]:a){
          if(mnx>x)mnx=x;
          if(mxx<x)mxx=x;
          if(mny>y)mny=y;
          if(mxy<y)mxy=y;
        }
        rect[1]=std::make_tuple(mnx,mxx,mny,mxy);
      }
      rect[i*2]=rect[i*2+1]=rect[i];
      if(m&1){
        std::nth_element(index.begin()+l,index.begin()+(l+r)/2,index.begin()+r,[&](int x,int y){return std::get<1>(a[x])<std::get<1>(a[y]);});
        std::get<3>(rect[i*2])=std::get<2>(rect[i*2+1])=std::get<1>(a[index[(l+r)/2]]);
      }
      else{
        std::nth_element(index.begin()+l,index.begin()+(l+r)/2,index.begin()+r,[&](int x,int y){return std::get<0>(a[x])<std::get<0>(a[y]);});
        std::get<1>(rect[i*2])=std::get<0>(rect[i*2+1])=std::get<0>(a[index[(l+r)/2]]);
      }
    }
    pos.resize(z);
    for(int i=0;i<z;i++){
      const auto&[x,y,v]=a[index[i]];
      dat[i+z]=v;
      pos[index[i]]=i;
      rect[i+z]=std::make_tuple(x,x,y,y);
    }
    for(int i=z-1;i>=1;i--)update(i);
  }
  inline void update(int i){
    dat[i]=M::op(dat[i*2],dat[i*2+1]);
  }
  inline void propagate(int i,const F&f){
    dat[i]=M::mapping(f,dat[i],log2n-msb(i));
    lazy[i]=M::composition(f,lazy[i]);
  }
  inline void push(int i){
    propagate(i*2,lazy[i]);
    propagate(i*2+1,lazy[i]);
    lazy[i]=M::id();
  }
public:
  kdTree(const std::vector<std::tuple<I,I,S>>&init){
    build(init);
  }
  S prod(I lx,I rx,I ly,I ry){
    rx--,ry--;
    S res=M::e();
    auto dfs=[&](auto self,int id)->void {
      const auto&[nlx,nrx,nly,nry]=rect[id];
      if(rx<nlx||nrx<lx||ry<nly||nry<ly)return;
      if(lx<=nlx&&nrx<=rx&&ly<=nly&&nry<=ry){
        res=M::op(res,dat[id]);
        return;
      }
      if(id<z){
        push(id);
        self(self,id*2);
        self(self,id*2+1);
      }
    };
    dfs(dfs,1);
    return res;
  }
  void apply(I lx,I rx,I ly,I ry,const F&f){
    rx--,ry--;
    auto dfs=[&](auto self,int id)->void {
      const auto&[nlx,nrx,nly,nry]=rect[id];
      if(rx<nlx||nrx<lx||ry<nly||nry<ly)return;
      if(lx<=nlx&&nrx<=rx&&ly<=nly&&nry<=ry){
        propagate(id,f);
        return;
      }
      if(id<z){
        push(id);
        self(self,id*2);
        self(self,id*2+1);
        update(id);
      }
    };
    dfs(dfs,1);
  }
  void set(int id,const S&x){
    id=pos[id]+z;
    for(int i=log2n;i>=1;i--)push(id>>i);
    dat[id]=x;
    for(int i=1;i<=log2n;i++)update(id>>i);
  }
};
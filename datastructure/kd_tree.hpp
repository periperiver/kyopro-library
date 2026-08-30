#pragma once
#include<vector>
#include<tuple>
#include<cassert>
#include<algorithm>
#include "../math/util.hpp"
template<typename I,typename M>
struct kdTree{
private:
  using S=typename M::S;
  using F=typename M::F;
  struct Rectangle{
    I lx,rx,ly,ry;
    Rectangle(){}
    Rectangle(I x,I y):lx(x),rx(x+1),ly(y),ry(y+1){}
    Rectangle(I lx,I rx,I ly,I ry):lx(lx),rx(rx),ly(ly),ry(ry){}
  };
  int n,z,log2n;
  std::vector<std::pair<I,int>>xz,yz;
  std::vector<S>dat;
  std::vector<std::pair<F,bool>>lazy;
  std::vector<int>pos;
  std::vector<Rectangle>rect;
  std::vector<int>len;
  inline void propagate(int i,F f){
    dat[i]=M::mapping(f,dat[i],len[i]);
    if(i<z){
      lazy[i].first=M::composition(f,lazy[i].first);
      lazy[i].second=true;
    }
  }
  inline void push(int id){
    if(lazy[id].second){
      propagate(id*2,lazy[id].first);
      propagate(id*2+1,lazy[id].first);
      lazy[id]=std::make_pair(M::id(),false);
    }
  }
  inline void update(int i){dat[i]=M::op(dat[i*2],dat[i*2+1]);}
  S prod_rec(int id,int lx,int rx,int ly,int ry){
    const auto&[lx2,rx2,ly2,ry2]=rect[id];
    if(lx<=lx2&&rx2<=rx&&ly<=ly2&&ry2<=ry)return dat[id];
    if(rx2<=lx||rx<=lx2||ry2<=ly||ry<=ly2)return M::e();
    push(id);
    return M::op(prod_rec(id*2,lx,rx,ly,ry),prod_rec(id*2+1,lx,rx,ly,ry));
  }
  void apply_rec(int id,int lx,int rx,int ly,int ry,F f){
    const auto&[lx2,rx2,ly2,ry2]=rect[id];
    if(lx<=lx2&&rx2<=rx&&ly<=ly2&&ry2<=ry)return propagate(id,f);
    if(rx2<=lx||rx<=lx2||ry2<=ly||ry<=ly2)return;
    push(id);
    apply_rec(id*2,lx,rx,ly,ry,f);
    apply_rec(id*2+1,lx,rx,ly,ry,f);
    update(id);
  }
  inline int getx(I x)const{return std::lower_bound(xz.begin(),xz.end(),std::make_pair(x,0))-xz.begin();}
  inline int gety(I y)const{return std::lower_bound(yz.begin(),yz.end(),std::make_pair(y,0))-yz.begin();}
public:
  kdTree(){}
  kdTree(std::vector<std::tuple<I,I,S>>init):n(init.size()),z(ceil_pow2(n)),log2n(msb(z)),xz(n),yz(n),dat(z*2,M::e()),lazy(z,std::make_pair(M::id(),false)),pos(n),rect(z*2),len(z*2){
    for(int i=0;i<n;i++){
      const auto&[x,y,v]=init[i];
      xz[i]=std::make_pair(x,i);
      yz[i]=std::make_pair(y,i);
    }
    std::sort(xz.begin(),xz.end()),std::sort(yz.begin(),yz.end());
    std::vector<std::tuple<int,int,int>>idx(n);
    for(int i=0;i<n;i++){
      auto&[x,y,v]=init[i];
      x=std::lower_bound(xz.begin(),xz.end(),std::make_pair(x,i))-xz.begin();
      y=std::lower_bound(yz.begin(),yz.end(),std::make_pair(y,i))-yz.begin();
      idx[i]=std::make_tuple(i,x,y);
    }
    std::vector<std::pair<int,int>>lr(z*2);
    lr[1]=std::make_pair(0,n);
    for(int i=1;i<z;i++){
      auto [l,r]=lr[i];
      if(msb(i)%2==0){
        std::sort(idx.begin()+l,idx.begin()+r,[&](const std::tuple<int,int,int>&lhs,const std::tuple<int,int,int>&rhs){return std::get<1>(lhs)<std::get<1>(rhs);});
        int m=(l+r)/2;
        lr[i*2]=std::make_pair(l,m);
        lr[i*2+1]=std::make_pair(m,r);
      }
      else{
        std::sort(idx.begin()+l,idx.begin()+r,[&](const std::tuple<int,int,int>&lhs,const std::tuple<int,int,int>&rhs){return std::get<2>(lhs)<std::get<2>(rhs);});
        int m=(l+r)/2;
        lr[i*2]=std::make_pair(l,m);
        lr[i*2+1]=std::make_pair(m,r);
      }
      len[i]=r-l;
    }
    for(int i=z;i<z*2;i++){
      if(lr[i].first!=lr[i].second){
        auto [id,x,y]=idx[lr[i].first];
        dat[i]=std::get<2>(init[id]);
        pos[id]=i;
        rect[i]=Rectangle(x,y);
        len[i]=1;
      }
    }
    for(int i=z;--i;){
      update(i);
      if(lr[i].first+1==lr[i].second){
        rect[i]=rect[i*2+1];
        continue;
      }
      const auto&[lx1,rx1,ly1,ry1]=rect[i*2];
      const auto&[lx2,rx2,ly2,ry2]=rect[i*2+1];
      rect[i]=Rectangle(std::min(lx1,lx2),std::max(rx1,rx2),std::min(ly1,ly2),std::max(ry1,ry2));
    }
  }
  void set(int i,S v){
    i=pos[i];
    for(int j=log2n;j>=1;j--)push(i>>j);
    dat[i]=v;
    while(i>>=1)update(i);
  }
  S prod(I lx,I rx,I ly,I ry){
    assert(lx<=rx);
    assert(ly<=ry);
    lx=getx(lx),rx=getx(rx),ly=gety(ly),ry=gety(ry);
    if(lx==rx||ly==ry)return M::e();
    auto res=prod_rec(1,lx,rx,ly,ry);
    return res;
  }
  void apply(I lx,I rx,I ly,I ry,F f){
    assert(lx<=rx);
    assert(ly<=ry);
    lx=getx(lx),rx=getx(rx),ly=gety(ly),ry=gety(ry);
    if(lx<rx&&ly<ry)apply_rec(1,lx,rx,ly,ry,f);
  }
  S all_prod()const{return dat[1];}
};
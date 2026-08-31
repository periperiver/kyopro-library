#pragma once
#include<vector>
#include<tuple>
#include<cassert>
#include<algorithm>
#include "../math/util.hpp"
template<typename I,typename M>
struct kdTree{
private:
  using S=typename M::M1::S;
  using F=typename M::M2::S;
  struct Rectangle{
    I lx,rx,ly,ry;
    Rectangle(){}
    Rectangle(I x,I y):lx(x),rx(x+1),ly(y),ry(y+1){}
    Rectangle(I lx,I rx,I ly,I ry):lx(lx),rx(rx),ly(ly),ry(ry){}
    template<int mask>
    constexpr bool contains(const Rectangle&r)const{
      if constexpr(mask==0)return r.lx<=lx&&rx<=r.rx&&r.ly<=ly&&ry<=r.ry;
      else if constexpr(mask==1)return rx<=r.rx&&r.ly<=ly&&ry<=r.ry;
      else if constexpr(mask==2)return r.lx<=lx&&r.ly<=ly&&ry<=r.ry;
      else if constexpr(mask==3)return r.ly<=ly&&ry<=r.ry;
      else if constexpr(mask==4)return r.lx<=lx&&rx<=r.rx&&ry<=r.ry;
      else if constexpr(mask==5)return rx<=r.rx&&ry<=r.ry;
      else if constexpr(mask==6)return r.lx<=lx&&ry<=r.ry;
      else if constexpr(mask==7)return ry<=r.ry;
      else if constexpr(mask==8)return r.lx<=lx&&rx<=r.rx&&r.ly<=ly;
      else if constexpr(mask==9)return rx<=r.rx&&r.ly<=ly;
      else if constexpr(mask==10)return r.lx<=lx&&r.ly<=ly;
      else if constexpr(mask==11)return r.ly<=ly;
      else if constexpr(mask==12)return r.lx<=lx&&rx<=r.rx;
      else if constexpr(mask==13)return rx<=r.rx;
      else if constexpr(mask==14)return r.lx<=lx;
    }
    template<int mask>
    constexpr bool disjoint(const Rectangle&r)const{;
      if constexpr(mask==0)return rx<=r.lx||r.rx<=lx||ry<=r.ly||r.ry<=ly;
      else if constexpr(mask==1)return r.rx<=lx||ry<=r.ly||r.ry<=ly;
      else if constexpr(mask==2)return rx<=r.lx||ry<=r.ly||r.ry<=ly;
      else if constexpr(mask==3)return ry<=r.ly||r.ry<=ly;
      else if constexpr(mask==4)return rx<=r.lx||r.rx<=lx||r.ry<=ly;
      else if constexpr(mask==5)return r.rx<=lx||r.ry<=ly;
      else if constexpr(mask==6)return rx<=r.lx||r.ry<=ly;
      else if constexpr(mask==7)return r.ry<=ly;
      else if constexpr(mask==8)return rx<=r.lx||r.rx<=lx||ry<=r.ly;
      else if constexpr(mask==9)return r.rx<=lx||ry<=r.ly;
      else if constexpr(mask==10)return rx<=r.lx||ry<=r.ly;
      else if constexpr(mask==11)return ry<=r.ly;
      else if constexpr(mask==12)return rx<=r.lx||r.rx<=lx;
      else if constexpr(mask==13)return r.rx<=lx;
      else if constexpr(mask==14)return rx<=r.lx;
    }
  };
  struct node{
    Rectangle rect;
    I split;
    S val;
    F lazy;
    bool flag;
    node():rect(),split(),val(M::M1::e()),lazy(M::M2::e()),flag(false){}
  };
  int n,z,log2n;
  std::vector<std::pair<I,int>>xz,yz;
  std::vector<node>dat;
  std::vector<int>pos;
  inline void propagate(int i,F f){
    dat[i].val=M::act(dat[i].val,f);
    if(i<z){
      dat[i].lazy=M::M2::op(dat[i].lazy,f);
      dat[i].flag=true;
    }
  }
  inline void push(int i){
    if(dat[i].flag){
      propagate(i*2,dat[i].lazy);
      propagate(i*2+1,dat[i].lazy);
      dat[i].lazy=M::M2::e();
      dat[i].flag=false;
    }
  }
  inline void update(int i){dat[i].val=M::M1::op(dat[i*2].val,dat[i*2+1].val);}
  template<bool splitx,int mask>
  S prod_rec(int id,const Rectangle&r){
    if constexpr(mask==15)return dat[id].val;
    else{
      if(dat[id].rect.template contains<mask>(r))return dat[id].val;
      if(dat[id].rect.template disjoint<mask>(r))return M::M1::e();
      push(id);
      if constexpr(splitx){
        if(r.rx<=dat[id].split)return prod_rec<0,mask>(id*2,r);
        else if(dat[id].split<=r.lx)return prod_rec<0,mask>(id*2+1,r);
        else return M::M1::op(prod_rec<0,mask|2>(id*2,r),prod_rec<0,mask|1>(id*2+1,r));
      }
      else{
        if(r.ry<=dat[id].split)return prod_rec<1,mask>(id*2,r);
        else if(dat[id].split<=r.ly)return prod_rec<1,mask>(id*2+1,r);
        else return M::M1::op(prod_rec<1,mask|8>(id*2,r),prod_rec<1,mask|4>(id*2+1,r));
      }
    }
  }
  template<bool splitx,int mask>
  void apply_rec(int id,const Rectangle&r,F f){
    if constexpr(mask==15)return propagate(id,f);
    else{
      if(dat[id].rect.template contains<mask>(r))return propagate(id,f);
      if(dat[id].rect.template disjoint<mask>(r))return;
      push(id);
      if constexpr(splitx){
        if(r.rx<=dat[id].split)apply_rec<0,mask>(id*2,r,f);
        else if(dat[id].split<=r.lx)apply_rec<0,mask>(id*2+1,r,f);
        else apply_rec<0,mask|2>(id*2,r,f),apply_rec<0,mask|1>(id*2+1,r,f);
      }
      else{
        if(r.ry<=dat[id].split)apply_rec<1,mask>(id*2,r,f);
        else if(dat[id].split<=r.ly)apply_rec<1,mask>(id*2+1,r,f);
        else apply_rec<1,mask|8>(id*2,r,f),apply_rec<1,mask|4>(id*2+1,r,f);
      }
      update(id);
    }
  }
  inline int getx(I x)const{return std::lower_bound(xz.begin(),xz.end(),std::make_pair(x,0))-xz.begin();}
  inline int gety(I y)const{return std::lower_bound(yz.begin(),yz.end(),std::make_pair(y,0))-yz.begin();}
public:
  kdTree(){}
  kdTree(std::vector<std::tuple<I,I,S>>init):n(init.size()),z(ceil_pow2(n)),log2n(msb(z)),xz(n),yz(n),dat(z*2),pos(n){
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
        int m=(l+r)/2;
        std::nth_element(idx.begin()+l,idx.begin()+m,idx.begin()+r,[&](const std::tuple<int,int,int>&lhs,const std::tuple<int,int,int>&rhs){return std::get<1>(lhs)<std::get<1>(rhs);});
        lr[i*2]=std::make_pair(l,m);
        lr[i*2+1]=std::make_pair(m,r);
        dat[i].split=std::get<1>(idx[m]);
      }
      else{
        int m=(l+r)/2;
        std::nth_element(idx.begin()+l,idx.begin()+m,idx.begin()+r,[&](const std::tuple<int,int,int>&lhs,const std::tuple<int,int,int>&rhs){return std::get<2>(lhs)<std::get<2>(rhs);});
        lr[i*2]=std::make_pair(l,m);
        lr[i*2+1]=std::make_pair(m,r);
        dat[i].split=std::get<2>(idx[m]);
      }
    }
    for(int i=z;i<z*2;i++){
      if(lr[i].first!=lr[i].second){
        auto [id,x,y]=idx[lr[i].first];
        dat[i].val=std::get<2>(init[id]);
        dat[i].rect=Rectangle(x,y);
        pos[id]=i;
      }
    }
    for(int i=z;--i;){
      update(i);
      if(lr[i].first+1==lr[i].second){
        dat[i].rect=dat[i*2+1].rect;
        continue;
      }
      const auto&[lx1,rx1,ly1,ry1]=dat[i*2].rect;
      const auto&[lx2,rx2,ly2,ry2]=dat[i*2+1].rect;
      dat[i].rect=Rectangle(std::min(lx1,lx2),std::max(rx1,rx2),std::min(ly1,ly2),std::max(ry1,ry2));
    }
  }
  void set(int i,S v){
    i=pos[i];
    for(int j=log2n;j>=1;j--)push(i>>j);
    dat[i].val=v;
    while(i>>=1)update(i);
  }
  S prod(I lx,I rx,I ly,I ry){
    assert(lx<=rx);
    assert(ly<=ry);
    lx=getx(lx),rx=getx(rx),ly=gety(ly),ry=gety(ry);
    if(lx==rx||ly==ry)return M::M1::e();
    return prod_rec<1,0>(1,Rectangle(lx,rx,ly,ry));
  }
  void apply(I lx,I rx,I ly,I ry,F f){
    assert(lx<=rx);
    assert(ly<=ry);
    lx=getx(lx),rx=getx(rx),ly=gety(ly),ry=gety(ry);
    if(lx<rx&&ly<ry)apply_rec<1,0>(1,Rectangle(lx,rx,ly,ry),f);
  }
  S all_prod()const{return dat[1];}
};
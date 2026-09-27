#pragma once
#include<vector>
#include<cassert>
#include<algorithm>
#include<limits>
#include "bit.hpp"
#include "../monoid/pow.hpp"
template<typename I,typename M>
std::vector<typename M::S>static_rectangle_add_rectangle_sum(const std::vector<std::tuple<I,I,I,I,typename M::S>>&a,const std::vector<std::tuple<I,I,I,I>>&query){
  using S=typename M::S;
  static_assert(requires(S x){M::inverse(x);});
  int n=a.size(),q=query.size();
  I dx=std::numeric_limits<I>::max(),dy=std::numeric_limits<I>::max();
  std::vector<I>zy;
  zy.reserve(n*2);
  for(const auto&[lx,rx,ly,ry,val]:a){
    assert(lx<=rx&&ly<=ry);
    zy.push_back(ly);
    zy.push_back(ry);
    if(dx>lx)dx=lx;
    if(dy>ly)dy=ly;
  }
  std::sort(zy.begin(),zy.end());
  zy.erase(std::unique(zy.begin(),zy.end()),zy.end());
  std::vector<std::tuple<I,int,int,S>>event;
  event.reserve(n*2);
  for(const auto&[lx,rx,ly,ry,val]:a){
    assert(lx<=rx&&ly<=ry);
    int l=std::lower_bound(zy.begin(),zy.end(),ly)-zy.begin();
    int r=std::lower_bound(zy.begin(),zy.end(),ry)-zy.begin();
    event.emplace_back(lx,l,r,val);
    event.emplace_back(rx,l,r,M::inverse(val));
    if(dx>lx)dx=lx;
    if(dy>ly)dy=ly;
  }
  std::sort(event.begin(),event.end());
  std::vector<std::tuple<I,int,int,int>>get_event;
  get_event.reserve(q*2);
  {
    int id=0;
    for(const auto&[lx,rx,ly,ry]:query){
      int l=std::lower_bound(zy.begin(),zy.end(),ly)-zy.begin();
      int r=std::lower_bound(zy.begin(),zy.end(),ry)-zy.begin();
      get_event.emplace_back(lx,l,r,id);
      get_event.emplace_back(rx,l,r,id);
      id++;
    }
  }
  std::sort(get_event.begin(),get_event.end());
  BinaryIndexedTree<M>bit1(zy.size()),bit2(zy.size()),bit3(zy.size()),bit4(zy.size());
  int ptr=0;
  std::vector<S>res(q,M::e());
  for(auto [lx,l,r,id]:get_event){
    while(ptr<(int)event.size()){
      const auto&[lx2,l2,r2,val]=event[ptr];
      if(lx2<lx){
        S inv=M::inverse(val);
        bit1.add(l2,val);
        bit1.add(r2,inv);
        bit2.add(l2,monoid_pow<M>(inv,lx2-dx));
        bit2.add(r2,monoid_pow<M>(val,lx2-dx));
        bit3.add(l2,monoid_pow<M>(inv,zy[l2]-dy));
        bit3.add(r2,monoid_pow<M>(val,zy[r2]-dy));
        bit4.add(l2,monoid_pow<M>(monoid_pow<M>(val,lx2-dx),zy[l2]-dy));
        bit4.add(r2,monoid_pow<M>(monoid_pow<M>(inv,lx2-dx),zy[r2]-dy));
        ptr++;
      }
      else break;
    }
    I ly=std::get<2>(query[id]);
    I ry=std::get<3>(query[id]);
    S val=M::e();
    val=M::op(val,monoid_pow<M>(monoid_pow<M>(bit1.sum(0,l),lx-dx),ly-dy));
    val=M::op(val,monoid_pow<M>(bit2.sum(0,l),ly-dy));
    val=M::op(val,monoid_pow<M>(bit3.sum(0,l),lx-dx));
    val=M::op(val,bit4.sum(0,l));
    val=M::inverse(val);
    val=M::op(val,monoid_pow<M>(monoid_pow<M>(bit1.sum(0,r),lx-dx),ry-dy));
    val=M::op(val,monoid_pow<M>(bit2.sum(0,r),ry-dy));
    val=M::op(val,monoid_pow<M>(bit3.sum(0,r),lx-dx));
    val=M::op(val,bit4.sum(0,r));
    res[id]=M::op(M::inverse(res[id]),val);
  }
  return res;
}
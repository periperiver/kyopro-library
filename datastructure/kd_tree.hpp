#pragma once
#include<vector>
#include<array>
#include<algorithm>
#include<numeric>
#include<cassert>
#include<iostream>
#include "../math/util.hpp"
template<typename I,typename M>
struct kdTree{
private:
  using S=typename M::S;
  using F=typename M::F;
  static constexpr int B=4;
  static constexpr int indc=(1<<(B-1))-1;
  static_assert(2<=B&&B%2==0);
  using Iterator=typename std::vector<std::tuple<I,I,S,int>>::iterator;
  struct node{
    std::array<S,(1<<B)-1>dat;
    std::array<std::pair<F,bool>,(1<<B)-1>lazy;
    std::array<node*,(1<<B)>ch;
    std::array<std::tuple<int,int,int,int>,(1<<B)-1>rect;
    std::array<std::pair<I,int>,(1<<B)-1>split;
    node(){}
    node(Iterator l,Iterator r){if(l<r)build<0>(l,r);}
    template<int id>
    void build(Iterator l,Iterator r){
      lazy[id]=std::make_pair(M::id(),false);
      if(r-l==1){
        I x,y;
        int temp;
        std::tie(x,y,dat[id],temp)=*l;
        rect[id]=std::make_tuple(x,x+1,y,y+1);
        split[id]=std::make_pair(0,-1);
        return;
      }
      Iterator m=l+(r-l)/2;
      std::nth_element(l,m,r,[](const std::tuple<I,I,S,int>&lhs,const std::tuple<I,I,S,int>&rhs){
        if constexpr(msb(id+1)%2==0)return std::get<0>(lhs)!=std::get<0>(rhs)?std::get<0>(lhs)<std::get<0>(rhs):std::get<3>(lhs)<std::get<3>(rhs);
        else return std::get<1>(lhs)!=std::get<1>(rhs)?std::get<1>(lhs)<std::get<1>(rhs):std::get<3>(lhs)<std::get<3>(rhs);
      });
      if constexpr(msb(id+1)%2==0)split[id]=std::make_pair(std::get<0>(*m),std::get<3>(*m));
      else split[id]=std::make_pair(std::get<1>(*m),std::get<3>(*m));
      if constexpr(id<indc){
        build<id*2+1>(l,m);
        build<id*2+2>(m,r);
        const auto&[lx1,rx1,ly1,ry1]=rect[id*2+1];
        const auto&[lx2,rx2,ly2,ry2]=rect[id*2+2];
        rect[id]=std::make_tuple(std::min(lx1,lx2),std::max(rx1,rx2),std::min(ly1,ly2),std::max(ry1,ry2));
      }
      else{
        ch[getid<id>()]=new node(l,m);
        ch[getid<id>()+1]=new node(m,r);
        const auto&[lx1,rx1,ly1,ry1]=ch[getid<id>()]->rect[0];
        const auto&[lx2,rx2,ly2,ry2]=ch[getid<id>()+1]->rect[0];
        rect[id]=std::make_tuple(std::min(lx1,lx2),std::max(rx1,rx2),std::min(ly1,ly2),std::max(ry1,ry2));
      }
      update<id>();
    }
    template<int id>
    inline void propagate(F f){
      dat[id]=M::mapping(f,dat[id]);
      if(split[id].second!=-1){
        lazy[id].first=M::composition(f,lazy[id].first);
        lazy[id].second=true;
      }
    }
    template<int id>
    static constexpr int getid(){return (id-indc)*2;}
    template<int id>
    inline void push(){
      if(lazy[id].second){
        if constexpr(id<indc){
          propagate<id*2+1>(lazy[id].first);
          propagate<id*2+2>(lazy[id].first);
        }
        else{
          if(ch[getid<id>()])ch[getid<id>()]->template propagate<0>(lazy[id].first);
          if(ch[getid<id>()+1])ch[getid<id>()+1]->template propagate<0>(lazy[id].first);
        }
        lazy[id]=std::make_pair(M::id(),false);
      }
    }
    template<int id>
    inline void update(){
      if constexpr(id<indc)dat[id]=M::op(dat[id*2+1],dat[id*2+2]);
      else dat[id]=M::op(ch[getid<id>()]->dat[0],ch[getid<id>()+1]->dat[0]);
    }
    template<int id>
    S prod_rec(I lx,I rx,I ly,I ry){
      const auto&[lx2,rx2,ly2,ry2]=rect[id];
      if(lx<=lx2&&rx2<=rx&&ly<=ly2&&ry2<=ry)return dat[id];
      if(rx2<=lx||rx<=lx2||ry2<=ly||ry<=ly2)return M::e();
      push<id>();
      if constexpr(id<indc)return M::op(prod_rec<id*2+1>(lx,rx,ly,ry),prod_rec<id*2+2>(lx,rx,ly,ry));
      else return M::op(ch[getid<id>()]->template prod_rec<0>(lx,rx,ly,ry),ch[getid<id>()+1]->template prod_rec<0>(lx,rx,ly,ry));
    }
    template<int id>
    void apply_rec(I lx,I rx,I ly,I ry,F f){
      const auto&[lx2,rx2,ly2,ry2]=rect[id];
      if(lx<=lx2&&rx2<=rx&&ly<=ly2&&ry2<=ry)return propagate<id>(f);
      if(rx2<=lx||rx<=lx2||ry2<=ly||ry<=ly2)return;
      push<id>();
      if constexpr(id<indc)apply_rec<id*2+1>(lx,rx,ly,ry,f),apply_rec<id*2+2>(lx,rx,ly,ry,f);
      else ch[getid<id>()]->template apply_rec<0>(lx,rx,ly,ry,f),ch[getid<id>()+1]->template apply_rec<0>(lx,rx,ly,ry,f);
      update<id>();
    }
    template<int id>
    void set_rec(I x,I y,int i,S v){
      if(split[id].second==-1){
        dat[id]=v;
        return;
      }
      push<id>();
      I key=msb(id+1)%2==0?x:y;
      if(std::make_pair(key,i)<split[id]){
        if constexpr(id<indc)set_rec<id*2+1>(x,y,i,v);
        else ch[getid<id>()]->template set_rec<0>(x,y,i,v);
      }
      else{
        if constexpr(id<indc)set_rec<id*2+2>(x,y,i,v);
        else ch[getid<id>()+1]->template set_rec<0>(x,y,i,v);
      }
      update<id>();
    }
    ~node(){
      for(node*nd:ch)if(nd)delete nd;
    }
  };
  node*root;
  std::vector<std::pair<I,I>>pos;
public:
  kdTree():root(nullptr){}
  explicit kdTree(const std::vector<std::tuple<I,I,S>>&init):root(nullptr),pos(init.size()){
    if(!init.empty()){
      std::vector<std::tuple<I,I,S,int>>a(init.size());
      for(int i=0;i<(int)a.size();i++){
        const auto&[x,y,v]=init[i];
        pos[i]=std::make_pair(x,y);
        a[i]=std::make_tuple(x,y,v,i);
      }
      root=new node(a.begin(),a.end());
    }
  }
  S prod(I lx,I rx,I ly,I ry){
    if(root)return root->template prod_rec<0>(lx,rx,ly,ry);
    else return M::e();
  }
  void apply(I lx,I rx,I ly,I ry,F f){
    if(root)root->template apply_rec<0>(lx,rx,ly,ry,f);
  }
  void set(int i,S v){
    assert(0<=i&&i<(int)pos.size());
    root->template set_rec<0>(pos[i].first,pos[i].second,i,v);
  }
  S all_prod()const{return root?root->dat[0]:M::e();}
  ~kdTree(){
    if(root)delete root;
  }
};
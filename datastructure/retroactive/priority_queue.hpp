#pragma once
#include<vector>
#include "datastructure/segmenttree.hpp"
template<typename T,typename insert_t,typename erase_t>
struct RetroactivePriorityQueue{
private:
  static constexpr T inf=std::numeric_limits<T>::max();
  static constexpr T minf=std::numeric_limits<T>::min();
  static_assert(std::is_convertible_v<insert_t,std::function<void(int,T)>>);
  static_assert(std::is_convertible_v<erase_t,std::function<void(int,T)>>);
  struct Monoid{
    struct S{
      int sum,pre;
    };
    static inline S op(const S&x,const S&y){
      return S{x.sum+y.sum,std::min(x.pre,x.sum+y.pre)};
    }
    static inline S e(){return S{0,0};}
  };
  int n,z;
  SegmentTree<Monoid>seg;
  std::vector<std::pair<T,T>>mnmx;
  insert_t insert;
  erase_t erase;
  void update_min(int i){
    for(i+=z;i>>=1;)mnmx[i].first=std::min(mnmx[i*2].first,mnmx[i*2+1].first);
  }
  void update_max(int i){
    for(i+=z;i>>=1;)mnmx[i].second=std::max(mnmx[i*2].second,mnmx[i*2+1].second);
  }
  void update_minmax(int i){
    for(i+=z;i>>=1;){
      mnmx[i].first=std::min(mnmx[i*2].first,mnmx[i*2+1].first);
      mnmx[i].second=std::max(mnmx[i*2].second,mnmx[i*2+1].second);
    }
  }
  int min_pos(int r)const{
    int id=0;
    if(r==z)id=1;
    else{
      for(r+=mnmx.size()/2;(r>>=lsb(r))>1;){
        r--;
        if(mnmx[id].first>mnmx[r].first)id=r;
      }
    }
    if(id==0)return 0;
    while(id<z){
      if(mnmx[id].first==mnmx[id*2].first)id=id*2;
      else id=id*2+1;
    }
    return id-z;
  }
  int max_pos(int l)const{
    int prel=l;
    int id=0;
    if(l==0)id=1;
    else{
      for(l+=mnmx.size()/2;(l>>=lsb(l))>1;){
        if(mnmx[id].second<mnmx[l].second)id=l;
        l++;
      }
    }
    if(id==0)return prel;
    while(id<z){
      if(mnmx[id].second==mnmx[id*2].second)id=id*2;
      else id=id*2+1;
    }
    return id-z;
  }
  int bridge_prev(int i)const{
    int s=seg.prod(0,i).sum-seg.all_prod().sum;
    if(s==0)return i;
    int l=seg.min_left(i,[&](const typename Monoid::S&v)->bool {
      return s-v.sum+v.pre!=0;
    });
    return l-1;
  }
  int bridge_next(int i)const{
    int s=seg.prod(0,i).sum-seg.all_prod().sum;
    if(s==0)return i;
    int r=seg.max_right(i,[&](const typename Monoid::S&v)->bool {
      return s+v.pre!=0;
    });
    return r+1;
  }
public:
  RetroactivePriorityQueue(int n_,insert_t insert_,erase_t erase_):n(n_),z(ceil_pow2(n)),seg(n),mnmx(z*2,std::make_pair(inf,minf)),insert(std::move(insert_)),erase(std::move(erase_)){}
  int fail_count()const{return -seg.all_prod().sum;}
  void set_push(int i,T v){
    set_none(i);
    int b=bridge_prev(i);
    if(b!=-1){
      int pos=max_pos(b);
      T val=mnmx[pos+z].second;
      if(val<v){
        insert(i,v);
        mnmx[i+z].first=v;
        update_min(i);
        return;
      }
      insert(pos,val);
      mnmx[pos+z]=std::make_pair(val,minf);
      update_minmax(pos);
      seg.set(pos,{0,0});
    }
    mnmx[i+z].second=v;
    update_max(i);
    seg.set(i,{1,0});
  }
  void set_pop(int i){
    set_none(i);
    int b=bridge_next(i+1);
    int pos=min_pos(b);
    T val=mnmx[pos+z].first;
    if(val!=inf){
      erase(pos,val);
      mnmx[pos+z]=std::make_pair(inf,val);
      update_minmax(pos);
      seg.set(pos,{1,0});
    }
    seg.set(i,{-1,-1});
  }
  void set_none(int i){
    if(seg.get(i).sum==-1){
      int b=bridge_prev(i);
      if(b!=-1){
        int pos=max_pos(b);
        T val=mnmx[pos+z].second;
        if(val!=minf){
          insert(pos,val);
          mnmx[pos+z]=std::make_pair(val,minf);
          update_minmax(pos);
          seg.set(pos,{0,0});
        }
      }
      seg.set(i,{0,0});
    }
    else if(mnmx[i+z].first!=inf){
      erase(i,mnmx[i+z].first);
      mnmx[i+z].first=inf;
      update_min(i);
    }
    else if(mnmx[i+z].second!=minf){
      int b=bridge_next(i+1);
      int pos=min_pos(b);
      T val=mnmx[pos+z].first;
      if(val!=inf){
        erase(pos,val);
        mnmx[pos+z]=std::make_pair(inf,val);
        update_minmax(pos);
        seg.set(pos,{1,0});
      }
      mnmx[i+z].second=minf;
      update_max(i);
      seg.set(i,{0,i});
    }
  }
};
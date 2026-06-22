#pragma once
#include<iostream>
#include<vector>
#include<cassert>
#include<functional>
#include "../math/util.hpp"
template<typename T>
struct RangeAddRangeMin{
private:
  int n,z;
  std::vector<T>seg,mn;
  static constexpr T inf=std::numeric_limits<T>::max()/2;
public:
  RangeAddRangeMin(){}
  explicit RangeAddRangeMin(int n):n(n),z(ceil_pow2(n)),seg(z*2),mn(z*2){}
  explicit RangeAddRangeMin(const std::vector<T>&init):n(init.size()),z(ceil_pow2(n)),seg(z*2),mn(z*2,inf){
    std::copy(init.begin(),init.end(),mn.begin()+z);
    for(int i=z;--i;)mn[i]=std::min(mn[i*2],mn[i*2+1]);
  }
  void add(int l,int r,T x){
    assert(0<=l&&l<=r&&r<=n);
    if(l<n){
      seg[l+=z]+=x;
      mn[l]+=x;
      while(l>>=1){
        seg[l]+=x;
        mn[l]=std::min(mn[l*2],mn[l*2+1]+seg[l*2]);
      }
    }
    if(r<n){
      seg[r+=z]-=x;
      mn[r]-=x;
      while(r>>=1){
        seg[r]-=x;
        mn[r]=std::min(mn[r*2],mn[r*2+1]+seg[r*2]);
      }
    }
  }
  T min(int l,int r)const{
    assert(0<=l&&l<=r&&r<=n);
    if(l==r)return inf;
    if(r==n)r=z;
    T lv=inf,rv=inf,s=0;
    l+=z,r+=z;
    int l2=l;
    while(l<r){
      if(l&1){
        if(lv>mn[l]+s)lv=mn[l]+s;
        s+=seg[l];
        l++;
      }
      if(r&1){
        --r;
        rv+=seg[r];
        if(rv>mn[r])rv=mn[r];
      }
      l>>=1,r>>=1;
    }
    if(lv>rv+s)lv=rv+s;
    l=l2;
    while((l>>=lsb(l))>1)lv+=seg[--l];
    return lv;
  }
  T get(int i)const{
    assert(0<=i&&i<n);
    T res=mn[i+=z];
    while((i>>=lsb(i))>1)res+=seg[--i];
    return res;
  }
  inline void set(int i,T x){
    T s=0;
    for(int j=i+z;(j>>=lsb(j))>1;)s+=seg[--j];
    mn[i+=z]=x-s;
    while(i>>=1)mn[i]=std::min(mn[i*2],mn[i*2+1]+seg[i*2]);
  }
  template<typename Func>
  int max_right(int l,const Func&f)const{
    static_assert(std::is_convertible_v<Func,std::function<bool(T)>>);
    if(l==n)return n;
    l+=z;
    T s=0;
    for(int i=l;(i>>=lsb(i))>1;)s+=seg[--i];
    do{
      l>>=lsb(l);
      T v=s+mn[l];
      if(f(v))s+=seg[l++];
      else{
        while(l<z){
          l<<=1;
          v=s+mn[l];
          if(f(v))s+=seg[l++];
        }
        return l-z;
      }
    }while(l!=(l&-l));
    return n;
  }
  template<typename Func>
  int min_left(int r,const Func&f)const{
    static_assert(std::is_convertible_v<Func,std::function<bool(T)>>);
    if(r==0)return 0;
    r+=z;
    T s=0;
    if(r==z*2)s=seg[1];
    else for(int i=r;(i>>=lsb(i))>1;)s+=seg[--i];
    do{
      r--;
      r>>=lsb(~r);
      if(r==0)r=1;
      T v=s-seg[r]+mn[r];
      if(f(v))s-=seg[r];
      else{
        while(r<z){
          r=(r<<1)+1;
          v=s-seg[r]+mn[r];
          if(f(v))s-=seg[r--];
        }
        return r-z+1;
      }
    }while(r!=(r&-r));
    return 0;
  }
  std::vector<T>get_all()const{
    std::vector<T>res(mn.begin()+z,mn.begin()+z+n);
    T s=0;
    for(int i=0;i<n;i++){
      res[i]+=s;
      s+=seg[i+z];
    }
    return res;
  }
  friend std::ostream &operator<<(std::ostream&os,const RangeAddRangeMin&seg){
    std::vector<T>a=seg.get_all();
    int n=a.size();
    os<<"{";
    for(int i=0;i<n;i++)os<<a[i]<<",}"[i+1==n];
    if(n==0)os<<"}";
    return os;
  }
  int size()const{return n;}
};
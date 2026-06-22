#pragma once
#include<vector>
#include<algorithm>
#include<cassert>
#include<cmath>
#include<numeric>
template<typename T>
struct sqrtSegmentTree{
  int n,s,lb,rb;
  std::vector<T>w,seg,sum;
  std::vector<int>dat,lazy;
  T val;
  explicit sqrtSegmentTree(std::vector<T>w_,int l,int r):w(std::move(w_)),lb(l),rb(r),val(0){
    n=w.size(),s=std::max<int>(1,std::sqrt(n/7));
    w.resize((n+s-1)/s*s);
    seg.resize((n+s-1)/s*(s+1));
    sum.resize((n+s-1)/s);
    dat.resize((n+s-1)/s*s);
    lazy.resize((n+s-1)/s);
    for(int i=0;i<(n+s-1)/s;i++){
      T sum=std::reduce(w.begin()+i*s,w.begin()+(i+1)*s);
      std::fill(seg.begin()+i*(s+1)+1,seg.begin()+(i+1)*(s+1),sum);
    }
  }
  void update(int idx){
    int l=idx*s,r=(idx+1)*s;
    int d=*std::min_element(dat.begin()+l,dat.begin()+r);
    lazy[idx]+=d;
    int p=idx*(s+1);
    std::fill(seg.begin()+p,seg.begin()+p+s+1,0);
    for(int i=l;i<r;i++)seg[p+1+(dat[i]-=d)]+=w[i];
    for(int i=0;i<s;i++)seg[p+i+1]+=seg[p+i];
    l=std::clamp(lb-lazy[idx],0,s);
    r=std::clamp(rb-lazy[idx],0,s);
    val+=seg[p+r]-seg[p+l]-sum[idx];
    sum[idx]=seg[p+r]-seg[p+l];
  }
  void add(int l,int r,int x){
    int nl=(l+s-1)/s,nr=r/s;
    if(nl>nr){
      for(int i=l;i<r;i++)dat[i]+=x;
      update(l/s);
      return;
    }
    if(l<nl*s){
      for(int i=l;i<nl*s;i++)dat[i]+=x;
      update(l/s);
    }
    if(nr*s<r){
      for(int i=nr*s;i<r;i++)dat[i]+=x;
      update((r-1)/s);
    }
    for(int i=nl;i<nr;i++){
      lazy[i]+=x;
      int l2=std::clamp(lb-lazy[i],0,s);
      int r2=std::clamp(rb-lazy[i],0,s);
      T v=seg[i*(s+1)+r2]-seg[i*(s+1)+l2];
      val+=v-sum[i];
      sum[i]=v;
    }
  }
};
template<typename T>
T union_rectangles_range(const std::vector<std::tuple<T,T,T,T>>recs,int l,int r){
  struct E{
    T x;
    int l,r;
    int add;
    bool operator<(const E&rhs)const{return x<rhs.x;}
  };
  assert(1<=l&&l<=r);
  int n=recs.size();
  if(n==0)return 0;
  std::vector<E>query(n*2);
  std::vector<std::pair<T,int>>zy(n*2);
  for(int p=0;const auto&[lx,rx,ly,ry]:recs){
    zy[p*2]=std::make_pair(ly,p);
    zy[p*2+1]=std::make_pair(ry,p);
    p++;
  }
  std::sort(zy.begin(),zy.end());
  for(int p=0;const auto&[lx,rx,ly,ry]:recs){
    int l2=std::lower_bound(zy.begin(),zy.end(),std::make_pair(ly,p))-zy.begin();
    int r2=std::lower_bound(zy.begin(),zy.end(),std::make_pair(ry,p))-zy.begin();
    query[p*2]=E{lx,l2,r2,1};
    query[p*2+1]=E{rx,l2,r2,-1};
    p++;
  }
  std::sort(query.begin(),query.end());
  std::vector<T>w(n*2-1);
  for(int i=0;i<n*2-1;i++)w[i]=zy[i+1].first-zy[i].first;
  sqrtSegmentTree<T>seg(std::move(w),l,r);
  T res=0,prex=0;
  for(auto [x,l2,r2,add]:query){
    res+=seg.val*(x-prex);
    prex=x;
    seg.add(l2,r2,add);
  }
  return res;
}
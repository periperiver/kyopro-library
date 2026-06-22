#pragma once
#include<vector>
#include "math/util.hpp"
template<typename Line,typename Func>
struct LiChaoLinereeExtended{
private:
  int n,log2n;
  std::vector<Line>dat;
  Func f;
  Line inf;
  inline bool is_less(const Line&lhs,const Line&rhs,int x){
    if(x>=n)x=n-1;
    return f(lhs,x)<f(rhs,x);
  }
  void add_to_node(int i,Line x){
    int sz=1<<(log2n-msb(i));
    int l=(i-floor_pow2(i))*sz;
    int r=l+sz;
    while(true){
      int mid=(l+r)/2;
      if(is_less(x,dat[i],mid))std::swap(x,dat[i]);
      if(l+1==r)break;
      bool lf=is_less(x,dat[i],l);
      bool rf=is_less(x,dat[i],r-1);
      if(!lf&&!rf)break;
      if(lf)r=mid,i=i*2;
      else l=mid,i=i*2+1;
    }
  }
public:
  LiChaoLinereeExtended(int n_,Func f_,Line inf_):n(n_),log2n(msb(ceil_pow2(n_))),dat(ceil_pow2(n_)*2,inf_),f(f_),inf(inf_){}
  inline void add_line(const Line&x){
    add_to_node(1,x);
  }
  void add_segment(int l,int r,const Line&x){
    l+=dat.size()>>1,r+=dat.size()>>1;
    while(l<r){
      if(l&1)add_to_node(l++,x);
      if(r&1)add_to_node(--r,x);
      l>>=1,r>>=1;
    }
  }
  Line query(int x){
    Line res=inf;
    int x2=x;
    x+=dat.size()>>1;
    while(x){
      if(is_less(dat[x],res,x2))res=dat[x];
      x>>=1;
    }
    return res;
  }
};
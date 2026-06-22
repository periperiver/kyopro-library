#pragma once
#include "../math/util.hpp"
#include "bit_vector.hpp"
#include<algorithm>
#include<limits>
#include<type_traits>
#include<functional>
template<typename I>
struct RectangleQuery{
private:
  static constexpr I minf=std::numeric_limits<I>::min();
  int n,log2m;
  std::vector<I>yz;
  std::vector<std::pair<I,I>>xz;
  std::vector<bit_vector>mat;
public:
  RectangleQuery(){}
  RectangleQuery(std::vector<std::pair<I,I>>init):n(init.size()),xz(std::move(init)),yz(n){
    std::sort(xz.begin(),xz.end());
    for(int i=0;i<n;i++)yz[i]=xz[i].second;
    std::sort(yz.begin(),yz.end()),yz.erase(std::unique(yz.begin(),yz.end()),yz.end());
    log2m=msb(ceil_pow2(yz.size()+1));
    mat.resize(log2m,bit_vector(n));
    std::vector<int>bit(n),bit2(n);
    for(int i=0;i<n;i++)bit[i]=std::lower_bound(yz.begin(),yz.end(),xz[i].second)-yz.begin();
    for(int i=log2m-1;i>=0;i--){
      for(int j=0;j<n;j++)if(bit[j]>>i&1)mat[i].set(j);
      mat[i].build();
      int pos[2]={0,mat[i].zero};
      for(int j=0;j<n;j++){
        bit2[pos[bit[j]>>i&1]++]=bit[j];
      }
      std::swap(bit,bit2);
    }
  }
  int size()const{return n*(log2m+1);}
  template<typename Func>
  void get_vs(I x,I y,const Func&f)const{
    static_assert(std::is_convertible_v<Func,std::function<void(int)>>);
    int cx=std::lower_bound(xz.begin(),xz.end(),std::make_pair(x,y))-xz.begin();
    int cy=std::lower_bound(yz.begin(),yz.end(),xz[cx].second)-yz.begin();
    f(n*log2m+cx);
    for(int i=log2m-1;i>=0;i--){
      if(cy>>i&1)cx=mat[i].zero+mat[i].get(cx);
      else cx-=mat[i].get(cx);
      f(n*i+cx);
    }
  }
  template<typename Func>
  void get_range(I lx,I rx,I ly,I ry,const Func&f)const{
    static_assert(std::is_convertible_v<Func,std::function<void(int,int)>>);
    int l=std::lower_bound(xz.begin(),xz.end(),std::make_pair(lx,minf))-xz.begin();
    int r=std::lower_bound(xz.begin(),xz.end(),std::make_pair(rx,minf))-xz.begin();
    int d=std::lower_bound(yz.begin(),yz.end(),ly)-yz.begin();
    int u=std::lower_bound(yz.begin(),yz.end(),ry)-yz.begin();
    if(l>=r||d>=u)return;
    int m=msb(d^u);
    for(int i=log2m-1;i>m;i--){
      int lv=mat[i].get(l),rv=mat[i].get(r);
      if(d>>i&1)l=mat[i].zero+lv,r=mat[i].zero+rv;
      else l-=lv,r-=rv;
    }
    u=u-(1<<m);
    int l2=l,r2=r;
    l=mat[m].zero+mat[m].get(l),r=mat[m].zero+mat[m].get(r);
    if(u>>m&1){
      f(n*m+l,n*m+r);
    }
    else{
      for(int i=m-1;i>=0;i--){
        int lv=mat[i].get(l),rv=mat[i].get(r);
        if(u>>i&1){
          f(n*i+l-lv,n*i+r-rv);
          l=mat[i].zero+lv,r=mat[i].zero+rv;
        }
        else l-=lv,r-=rv;
      }
    }
    l=l2,r=r2;
    d=(1<<m)-d;
    l-=mat[m].get(l),r-=mat[m].get(r);
    if(d>>m&1){
      f(n*m+l,n*m+r);
    }
    else{
      for(int i=m-1;i>=0;i--){
        int lv=mat[i].get(l),rv=mat[i].get(r);
        if(d>>i&1){
          f(n*i+mat[i].zero+lv,n*i+mat[i].zero+rv);
          l-=lv,r-=rv;
        }
        else l=mat[i].zero+lv,r=mat[i].zero+rv;
      }
    }
  }
};
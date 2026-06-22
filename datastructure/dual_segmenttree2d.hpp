#pragma once
#include<vector>
#include<algorithm>
#include<limits>
#include "../math/util.hpp"
template<typename I,typename M>
struct DualSegmentTree2d{
private:
  static constexpr I minf=std::numeric_limits<I>::min();
  using F=typename M::F;
  int n,lg;
  std::vector<std::pair<I,I>>xz;
  std::vector<std::vector<I>>yz;
  std::vector<std::vector<F>>dat;
  void iapply(int h,int id,I ly,I ry,F f){
    id-=n>>h;
    int l=std::lower_bound(yz[h].begin()+(id<<h),yz[h].begin()+((id+1)<<h),ly)-(yz[h].begin()+(id<<h));
    int r=std::lower_bound(yz[h].begin()+(id<<h),yz[h].begin()+((id+1)<<h),ry)-(yz[h].begin()+(id<<h));
    l+=1<<h,r+=1<<h;
    auto itr=dat[h].begin()+((2*id)<<h);
    while(l<r){
      if(l&1){
        itr[l]=M::composition(itr[l],f);
        l++;
      }
      if(r&1){
        --r;
        itr[r]=M::composition(itr[r],f);
      }
      l>>=1,r>>=1;
    }
  }
  F iget(int h,int id,I y)const{
    id-=n>>h;
    F res=M::id();
    int yp=std::lower_bound(yz[h].begin()+(id<<h),yz[h].begin()+((id+1)<<h),y)-(yz[h].begin()+(id<<h));
    yp+=1<<h;
    auto itr=dat[h].begin()+((2*id)<<h);
    while(yp){
      res=M::composition(res,itr[yp]);
      yp>>=1;
    }
    return res;
  }
public:
  DualSegmentTree2d():n(0){}
  DualSegmentTree2d(std::vector<std::pair<I,I>>pos):n(ceil_pow2(pos.size())),lg(msb(n)),xz(n),yz(lg+1,std::vector<I>(n)),dat(lg+1,std::vector<F>(n*2,M::id())){
    std::copy(pos.begin(),pos.end(),xz.begin());
    std::sort(xz.begin(),xz.end());
    for(int i=0;i<n;i++)yz[0][i]=xz[i].second;
    for(int i=1;i<=lg;i++){
      int len=1<<i;
      for(int j=0;j<n;j+=len){
        std::merge(yz[i-1].begin()+j,yz[i-1].begin()+j+len/2,yz[i-1].begin()+j+len/2,yz[i-1].begin()+j+len,yz[i].begin()+j);
      }
    }
  }
  void apply(I lx,I rx,I ly,I ry,F f){
    int l=std::lower_bound(xz.begin(),xz.end(),std::make_pair(lx,minf))-xz.begin();
    int r=std::lower_bound(xz.begin(),xz.end(),std::make_pair(rx,minf))-xz.begin();
    l+=n,r+=n;
    int h=0;
    while(l<r){
      if(l&1)iapply(h,l++,ly,ry,f);
      if(r&1)iapply(h,--r,ly,ry,f);
      l>>=1,r>>=1,h++;
    }
  }
  F get(I x,I y)const{
    F res=M::id();
    int xp=std::lower_bound(xz.begin(),xz.end(),std::make_pair(x,y))-xz.begin();
    xp+=n;
    int h=0;
    while(xp){
      res=M::composition(res,iget(h,xp,y));
      xp>>=1;
      h++;
    }
    return res;
  }
};
#pragma once
#include<vector>
#include<algorithm>
#include "bit_vector.hpp"
#include "../math/util.hpp"
template<typename I,typename M>
struct BinaryIndexedTree3d{
private:
  using S=typename M::S;
  static constexpr I minf=std::numeric_limits<I>::min();
  int n;
  std::vector<bit_vector>wm;
  std::vector<std::tuple<I,I,I>>xyz;
  std::vector<std::pair<I,I>>yz;
  std::vector<I>zz;
  std::vector<int>ptr;
  std::vector<std::vector<S>>dat;
  void add2(int x,I y,I z,S v){
    int cy=std::lower_bound(yz.begin()+ptr[x],yz.begin()+ptr[x+1],std::make_pair(y,z))-(yz.begin()+ptr[x]);
    int cz=std::lower_bound(zz.begin()+ptr[x],zz.begin()+ptr[x+1],z)-(zz.begin()+ptr[x]);
    int lg=lsb(x+1);
    int len=1<<lg;
    for(int i=lg;i>=0;i--){
      if(cz>>i&1)cy=len-(wm[i].get(ptr[x+1])-wm[i].get(ptr[x]+cy));
      else cy-=wm[i].get(ptr[x]+cy)-wm[i].get(ptr[x]);
      auto bit=dat[i].begin()+ptr[x];
      for(int y2=cy;y2<len;y2+=(y2+1)&-(y2+1))bit[y2]+=v;
    }
  }
  S sum2(int x,I ly,I ry,I rz)const{
    S res=M::e();
    int l=std::lower_bound(yz.begin()+ptr[x],yz.begin()+ptr[x+1],std::make_pair(ly,minf))-(yz.begin()+ptr[x]);
    int r=std::lower_bound(yz.begin()+ptr[x],yz.begin()+ptr[x+1],std::make_pair(ry,minf))-(yz.begin()+ptr[x]);
    int u=std::lower_bound(zz.begin()+ptr[x],zz.begin()+ptr[x+1],rz)-(zz.begin()+ptr[x]);
    int lg=lsb(x+1);
    int len=1<<lg;
    if(u==0)return res;
    for(int i=lg;i>=0;i--){
      int p=wm[i].get(ptr[x]),lv=wm[i].get(ptr[x]+l),rv=wm[i].get(ptr[x]+r),one=wm[i].get(ptr[x+1]);
      if(u>>i&1){
        auto bit=dat[i].begin()+ptr[x];
        int l2=l-(lv-p),r2=r-(rv-p);
        while(l2<r2){
          res=M::op(res,bit[r2-1]);
          r2-=r2&-r2;
        }
        while(r2<l2){
          res=M::op(res,M::inverse(bit[l2-1]));
          l2-=l2&-l2;
        }
        l=len-(one-lv);
        r=len-(one-rv);
      }
      else{
        l-=lv-p;
        r-=rv-p;
      }
    }
    return res;
  }
public:
  BinaryIndexedTree3d(){}
  explicit BinaryIndexedTree3d(std::vector<std::tuple<I,I,I>>init){
    std::sort(init.begin(),init.end());
    init.erase(std::unique(init.begin(),init.end()),init.end());
    n=init.size();
    xyz=std::move(init);
    ptr.resize(n+1);
    for(int i=0;i<n;i++)ptr[i]=(i+1)&-(i+1);
    for(int i=1;i<=n;i++)ptr[i]+=ptr[i-1];
    yz=std::vector<std::pair<I,I>>(ptr[n]);
    zz=std::vector<I>(ptr[n]);
    dat=std::vector<std::vector<S>>(msb(n)+1,std::vector<S>(ptr[n]));
    for(int i=0;i<n;i++){
      const auto&[x,y,z]=xyz[i];
      yz[--ptr[i]]=std::make_pair(y,z);
      int len=(i+1)&-(i+1);
      int j=i+len;
      if(j<n)std::copy(yz.begin()+ptr[i],yz.begin()+ptr[i]+len,yz.begin()+ptr[j]-len),ptr[j]-=len;
    }
    for(int i=0;i<ptr[n];i++)zz[i]=yz[i].second;
    for(int i=0;i<n;i++){
      std::sort(yz.begin()+ptr[i],yz.begin()+ptr[i+1]);
      std::sort(zz.begin()+ptr[i],zz.begin()+ptr[i+1]);
    }
    wm=std::vector<bit_vector>(msb(n)+1,bit_vector(ptr[n]));
    std::vector<int>a(ptr[n]),b(ptr[n]);
    for(int i=0;i<n;i++){
      for(int j=ptr[i];j<ptr[i+1];j++)a[j]=std::lower_bound(zz.begin()+ptr[i],zz.begin()+ptr[i+1],yz[j].second)-(zz.begin()+ptr[i]);
    }
    for(int i=msb(n);i>=0;i--){
      for(int j=0;j<n;j++){
        if(lsb(j+1)>=i){
          int zero=0;
          for(int k=ptr[j];k<ptr[j+1];k++){
            if(a[k]>>i&1)wm[i].set(k);
            else zero++;
          }
          int pos[2]={ptr[j],ptr[j]+zero};
          for(int k=ptr[j];k<ptr[j+1];k++){
            b[pos[a[k]>>i&1]++]=a[k];
          }
        }
        else std::copy(a.begin()+ptr[j],a.begin()+ptr[j+1],b.begin()+ptr[j]);
      }
      wm[i].build();
      std::swap(a,b);
    }
  }
  void add(I x,I y,I z,S v){
    int cx=std::lower_bound(xyz.begin(),xyz.end(),std::make_tuple(x,y,z))-xyz.begin();
    while(cx<n){
      add2(cx,y,z,v);
      cx+=(cx+1)&-(cx+1);
    }
  }
  S sum(I lx,I rx,I ly,I ry,I lz,I rz)const{
    int l=std::lower_bound(xyz.begin(),xyz.end(),std::make_tuple(lx,minf,minf))-xyz.begin();
    int r=std::lower_bound(xyz.begin(),xyz.end(),std::make_tuple(rx,minf,minf))-xyz.begin();
    S res=M::e();
    while(l<r){
      res=M::op(res,sum2(r-1,ly,ry,rz));
      res=M::op(res,M::inverse(sum2(r-1,ly,ry,lz)));
      r-=r&-r;
    }
    while(r<l){
      res=M::op(res,M::inverse(sum2(l-1,ly,ry,rz)));
      res=M::op(res,sum2(l-1,ly,ry,lz));
      l-=l&-l;
    }
    return res;
  }
};
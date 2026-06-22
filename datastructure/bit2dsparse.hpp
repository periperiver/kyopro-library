#pragma once
#include<vector>
#include<algorithm>
#include<map>
template<typename I,typename M>
struct BinaryIndexedTree2D{
  using S=typename M::S;
private:
  int n;
  std::vector<I>xz,yz;
  std::vector<int>offset;
  std::vector<S>dat;
  void build(std::vector<std::tuple<I,I,S>>a){
    xz.resize(a.size());
    for(int i=0;i<a.size();i++)xz[i]=std::get<0>(a[i]);
    std::sort(xz.begin(),xz.end());
    std::sort(xz.begin(),xz.end()),xz.erase(std::unique(xz.begin(),xz.end()),xz.end());
    n=xz.size();
    std::sort(a.begin(),a.end(),[](std::tuple<I,I,S>&x,std::tuple<I,I,S>&y){
      return std::get<1>(x)<std::get<1>(y);
    });
    offset.resize(n+1,0);
    std::vector<I>ls(n,-1);
    for(auto&&[x,y,v]:a){
      x=std::lower_bound(xz.begin(),xz.end(),x)-xz.begin();
      int x2=x;
      I y2=y;
      while(x2<n){
        if(ls[x2]==y2)break;
        ls[x2]=y2;
        offset[x2+1]++;
        x2+=(x2+1)&-(x2+1);
      }
    }
    for(int i=0;i<n;i++)offset[i+1]+=offset[i];
    yz.resize(offset[n]);
    dat.resize(offset[n],0);
    ls.assign(n,-1);
    std::vector<int>lt(offset);
    for(auto [x,y,v]:a){
      while(x<n){
        if(ls[x]==y)dat[lt[x]-1]=M::op(dat[lt[x]-1],v);
        else{
          ls[x]=y;
          yz[lt[x]]=y;
          dat[lt[x]]=v;
          lt[x]++;
        }
        x+=(x+1)&-(x+1);
      }
    }
    for(int i=0;i<n;i++){
      int m=offset[i+1]-offset[i];
      for(int j=0;j<m-1;j++){
        int k=j+((j+1)&-(j+1));
        if(k<m)dat[offset[i]+k]=M::op(dat[offset[i]+k],dat[offset[i]+j]);
      }
    }
  }
public:
  BinaryIndexedTree2D(const std::vector<std::tuple<I,I,S>>&a){
    build(a);
  }
  BinaryIndexedTree2D(const std::map<std::pair<I,I>,S>&a){
    std::vector<std::tuple<I,I,S>>b;
    b.reserve(a.size());
    for(auto [k,v]:a)b.emplace_back(k.first,k.second,v);
    build(b);
  }
  void add(I x,I y,S v){
    x=std::lower_bound(xz.begin(),xz.end(),x)-xz.begin();
    while(x<n){
      int y2=std::lower_bound(yz.begin()+offset[x],yz.begin()+offset[x+1],y)-(yz.begin()+offset[x]);
      int m=offset[x+1]-offset[x];
      while(y2<m){
        dat[offset[x]+y2]=M::op(dat[offset[x]+y2],v);
        y2+=(y2+1)&-(y2+1);
      }
      x+=(x+1)&-(x+1);
    }
  }
  S sum(I lx,I rx,I ly,I ry){
    int l=std::lower_bound(xz.begin(),xz.end(),lx)-xz.begin()-1,r=std::lower_bound(xz.begin(),xz.end(),rx)-xz.begin()-1;
    S ret=M::e();
    while(l<r){
      int l2=std::lower_bound(yz.begin()+offset[r],yz.begin()+offset[r+1],ly)-(yz.begin()+offset[r])-1;
      int r2=std::lower_bound(yz.begin()+offset[r],yz.begin()+offset[r+1],ry)-(yz.begin()+offset[r])-1;
      while(l2<r2){
        ret=M::op(ret,dat[offset[r]+r2]);
        r2-=(r2+1)&-(r2+1);
      }
      while(l2!=r2){
        ret=M::op(ret,M::inverse(dat[offset[r]+l2]));
        l2-=(l2+1)&-(l2+1);
      }
      r-=(r+1)&-(r+1);
    }
    while(l!=r){
      int l2=std::lower_bound(yz.begin()+offset[l],yz.begin()+offset[l+1],ly)-(yz.begin()+offset[l])-1;
      int r2=std::lower_bound(yz.begin()+offset[l],yz.begin()+offset[l+1],ry)-(yz.begin()+offset[l])-1;
      while(l2<r2){
        ret=M::op(ret,M::inverse(dat[offset[l]+r2]));
        r2-=(r2+1)&-(r2+1);
      }
      while(l2!=r2){
        ret=M::op(ret,dat[offset[l]+l2]);
        l2-=(l2+1)&-(l2+1);
      }
      l-=(l+1)&-(l+1);
    }
    return ret;
  }
};
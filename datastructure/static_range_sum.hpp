#pragma once
#include<vector>
#include<utility>
#include<algorithm>
template<typename I,typename S>
struct StaticRangeSum{
private:
  std::vector<I>z;
  std::vector<S>dat;
  std::vector<S>predat;
  std::vector<tuple<I,I,S>>info;
  S isum(I r)const{
    int ir=std::lower_bound(all(z),r)-z.begin();
    return dat[ir]+(ir==z.size()?0:predat[ir]*(r-z[ir]));
  }
public:
  StaticRangeSum(){}
  void add(I l,I r,S v){
    z.push_back(l);
    z.push_back(r);
    info.push_back({l,r,v});
  }
  inline void add(I i,S v){add(i,i+1,v);}
  void build(){
    if(z.empty())return;
    std::sort(z.begin(),z.end());
    z.erase(std::unique(z.begin(),z.end()),z.end());
    dat.resize(z.size()+1,0);
    for(auto [l,r,v]:info){
      l=lower_bound(z.begin(),z.end(),l)-z.begin();
      r=lower_bound(z.begin(),z.end(),r)-z.begin();
      dat[l+1]+=v;
      dat[r+1]-=v;
    }
    for(int i=1;i<dat.size();i++)dat[i]+=dat[i-1];
    predat=dat;
    for(int i=1;i<dat.size()-1;i++){
      dat[i]=dat[i]*(z[i]-z[i-1])+dat[i-1];
    }
    dat[dat.size()-1]=dat[dat.size()-2];
  }
  S sum(I l,I r)const{
    return isum(r)-isum(l);
  }
};
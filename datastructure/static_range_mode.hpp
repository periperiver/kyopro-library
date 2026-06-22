#pragma once
#include<cmath>
#include<vector>
#include<algorithm>
template<typename T>
struct StaticRangeMode{
private:
  std::vector<int>ptr,idx;
  int n,b,c;
  std::vector<int>pos;
  std::vector<int>a;
  std::vector<T>z;
  std::vector<std::vector<std::pair<int,int>>>range;
  inline int next_k(int i,int k)const{return pos[i]+k<ptr[a[i]+1]-ptr[a[i]]?idx[ptr[a[i]]+pos[i]+k]:(int)a.size();}
  inline int prev_k(int i,int k)const{return pos[i]-k>=0?idx[ptr[a[i]]+pos[i]-k]:-1;}
public:
  StaticRangeMode(const std::vector<T>&init):a(init.size()),z(init){
    n=init.size();
    b=std::sqrt(n);
    c=(n+b-1)/b;
    std::sort(z.begin(),z.end()),z.erase(std::unique(z.begin(),z.end()),z.end());
    for(int i=0;i<n;i++)a[i]=std::lower_bound(z.begin(),z.end(),init[i])-z.begin();
    pos.resize(n),range.resize(c,std::vector<std::pair<int,int>>(c));
    for(int l=0;l<c;l++){
      std::vector<int>cnt(z.size());
      int v,mx=0;
      for(int r=l+1;r<=c;r++){
        for(int i=(r-1)*b;i<std::min(r*b,n);i++){
          int &ncnt=++cnt[a[i]];
          if(mx<ncnt){
            mx=ncnt;
            v=a[i];
          }
        }
        range[l][r-1]=std::make_pair(v,mx);
      }
    }
    idx.resize(n),ptr.resize(z.size()+1);
    for(int i=0;i<n;i++)pos[i]=ptr[a[i]]++;
    for(int i=1;i<=(int)z.size();i++)ptr[i]+=ptr[i-1];
    for(int i=n;i--;)idx[--ptr[a[i]]]=i;
  }
  std::pair<T,int>mode(int l,int r)const{
    int lb=(l+b-1)/b,rb=r/b;
    int v,cnt=0;
    if(lb>=rb){
      for(int i=l;i<r;){
        int p=next_k(i,cnt);
        if(p<r)v=a[i],cnt++;
        else i++;
      }
      return std::make_pair(z[v],cnt);
    }
    std::tie(v,cnt)=range[lb][rb-1];
    lb*=b,rb*=b;
    for(int i=l;i<lb;){
      int p=next_k(i,cnt);
      if(p<r)v=a[i],cnt++;
      else i++;
    }
    for(int i=rb;i<r;){
      int p=prev_k(i,cnt);
      if(p>=l)v=a[i],cnt++;
      else i++;
    }
    return std::make_pair(z[v],cnt);
  }
};
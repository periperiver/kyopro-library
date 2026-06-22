#pragma once
#include<vector>
#include<limits>
template<typename T>
std::vector<T>min_plus_convolution2(const std::vector<T>&a,const std::vector<T>&b){
  int n=a.size(),m=b.size();
  std::vector<T>res(n+m-1,std::numeric_limits<T>::max());
  auto dfs=[&](auto self,int lx,int rx,int ly,int ry)->void {
    if(lx==rx)return;
    int mx=(lx+rx)/2;
    int amin=ly;
    for(int i=ly;i<ry;i++){
      if(a[mx-amin]+b[amin]>a[mx-i]+b[i])amin=i;
    }
    if(res[mx]>a[mx-amin]+b[amin])res[mx]=a[mx-amin]+b[amin];
    self(self,lx,mx,amin,ry);
    self(self,mx+1,rx,ly,amin+1);
  };
  auto ur=[&](auto self,int l,int r)->void {
    if(l==r)return;
    int mid=(l+r)/2;
    self(self,l,mid);
    self(self,mid+1,r);
    dfs(dfs,mid,r,l,mid+1);
  };
  auto dl=[&](auto self,int l,int r)->void {
    if(l==r)return;
    int mid=(l+r)/2;
    self(self,l,mid);
    self(self,mid+1,r);
    dfs(dfs,l,mid+1,mid-n+1,r-n+1);
  };
  for(int i=0;;i+=n+1){
    if(i<=m-n-1){
      ur(ur,i,i+n);
      dl(dl,i+n,i+n*2);
    }
    else{
      if(i<=m-1){
        ur(ur,i,m-1);
        dfs(dfs,m-1,i+n,i,m);
        dl(dl,i+n,n+m-1);
      }
      break;
    }
  }
  return res;
}
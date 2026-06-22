#pragma once
#include<vector>
#include<cassert>
#include<algorithm>
template<typename T>
long long inversion(const std::vector<T>&a){
  if(a.empty())return 0;
  std::vector<T>g[2];
  g[0].resize(a.size());
  g[1].resize(a.size());
  long long res=0;
  auto dfs=[&](auto self,int l,int r,int k)->void {
    if(l+1==r)g[k][l]=a[l];
    else{
      int mid=(l+r)/2;
      self(self,l,mid,k^1);
      self(self,mid,r,k^1);
      int lp=l,rp=mid,p=l;
      while(lp<mid||rp<r){
        if(rp==r||(lp<mid&&g[k^1][lp]<=g[k^1][rp]))g[k][p++]=g[k^1][lp++],res+=rp-mid;
        else g[k][p++]=g[k^1][rp++];
      }
    }
  };
  dfs(dfs,0,a.size(),0);
  return res;
}
template<typename T>
long long inversion(const std::vector<T>&a,const std::vector<T>&b){
  assert(a.size()==b.size());
  int n=a.size();
  std::vector<T>z(b);
  std::sort(z.begin(),z.end()),z.erase(std::unique(z.begin(),z.end()),z.end());
  std::vector<int>zb(n);
  std::vector<int>cnt(z.size());
  for(int i=0;i<n;i++)cnt[zb[i]=std::lower_bound(z.begin(),z.end(),b[i])-z.begin()]++;
  for(int i=1;i<(int)cnt.size();i++)cnt[i]+=cnt[i-1];
  std::vector<int>cnt2(cnt);
  std::vector<int>ord(n);
  for(int i=n-1;i>=0;i--)ord[--cnt[zb[i]]]=i;
  std::vector<int>za(n);
  for(int i=0;i<n;i++){
    int k=std::lower_bound(z.begin(),z.end(),a[i])-z.begin();
    if(k==(int)z.size()||z[k]!=a[i])return -1;
    za[i]=ord[cnt[k]++];
  }
  if(cnt!=cnt2)return -1;
  return inversion(za);
}
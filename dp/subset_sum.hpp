#pragma once
#include<vector>
#include<cassert>
#include<algorithm>
std::vector<bool>subset_sum(const std::vector<int>&a,int s){
  assert(0<=s&&!a.empty()&&std::all_of(a.begin(),a.end(),[](int x){return x>=0;}));
  if(s==0)return std::vector<bool>(a.size(),false);
  int n=a.size();
  int mx=*std::max_element(a.begin(),a.end());
  int b=0,sb=0;
  while(b<n&&a[b]+sb<=s)sb+=a[b++];
  if(b==n&&sb<s)return {};
  int offset=s-mx+1;
  std::vector<int>dp(mx*2,-1);
  std::vector<std::vector<int>>pre(n,std::vector<int>(mx*2,-1));
  dp[sb-offset]=b;
  for(int i=b;i<n;i++){
    std::vector<int>ndp(dp);
    std::vector<int>&prei=pre[i];
    for(int j=mx-1;j>=0;j--){
      if(ndp[j+a[i]]<ndp[j]){
        prei[j+a[i]]=-2;
        ndp[j+a[i]]=ndp[j];
      }
    }
    for(int j=mx*2-1;j>=mx;j--){
      for(int k=ndp[j]-1;k>=std::max(dp[j],0);k--){
        if(ndp[j-a[k]]<k){
          prei[j-a[k]]=k;
          ndp[j-a[k]]=k;
        }
      }
    }
    std::swap(dp,ndp);
  }
  if(dp[mx-1]==-1)return {};
  std::vector<bool>res(n,false);
  int i=n-1,j=mx-1;
  while(i>=b){
    int p=pre[i][j];
    if(p==-2){
      res[i]=!res[i];
      j-=a[i];
      i--;
    }
    else if(p==-1)i--;
    else{
      res[p]=!res[p];
      j+=a[p];
    }
  }
  while(i>=0){
    res[i]=!res[i];
    i--;
  }
  return res;
}
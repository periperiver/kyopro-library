#pragma once
#include<vector>
using namespace std;
vector<bool>subset_sum(const vector<int>&a,int s){
  if(s==0)return vector<bool>(a.size(),false);
  int n=a.size();
  int mx=*max_element(a.begin(),a.end());
  int b=0,sb=0;
  while(b<n&&a[b]+sb<=s)sb+=a[b++];
  if(b==n&&sb<s)return {};
  int offset=s-mx+1;
  vector<int>dp(mx*2,-1);
  vector<vector<int>>pre(n,vector<int>(mx*2,-1));
  dp[sb-offset]=b;
  for(int i=b;i<n;i++){
    vector<int>ndp(dp);
    vector<int>&prei=pre[i];
    for(int j=mx-1;j>=0;j--){
      if(ndp[j+a[i]]<ndp[j]){
        prei[j+a[i]]=-2;
        ndp[j+a[i]]=ndp[j];
      }
    }
    for(int j=mx*2-1;j>=mx;j--){
      for(int k=ndp[j]-1;k>=max(dp[j],0);k--){
        if(ndp[j-a[k]]<k){
          prei[j-a[k]]=k;
          ndp[j-a[k]]=k;
        }
      }
    }
    swap(dp,ndp);
  }
  if(dp[mx-1]==-1)return {};
  vector<bool>res(n,false);
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
#pragma once
#include<vector>
#include<algorithm>
#include<cassert>
#include<numeric>
#include "../math/barrett_reduction.hpp"
namespace egz_impl{
std::vector<bool>egz_prime(int p,std::vector<int>a){
  BarrettReduction br(p);
  for(int&x:a)x=br.rem(x);
  std::vector<int>ord(p*2-1);
  std::iota(ord.begin(),ord.end(),0);
  std::sort(ord.begin(),ord.end(),[&](int lhs,int rhs){return a[lhs]<a[rhs];});
  for(int i=0;i<p;i++)if(a[ord[i]]==a[ord[i+p-1]]){
    std::vector<bool>res(p*2-1,false);
    for(int j=i;j<i+p;j++)res[ord[j]]=true;
    return res;
  }
  int sum=0;
  for(int i=0;i<p;i++){
    sum+=a[ord[i]];
    if(sum>=p)sum-=p;
  }
  sum=p-sum;
  if(sum==p)sum=0;
  std::vector<int>dp(p,-1);
  dp[0]=p;
  for(int i=0;i<p-1;i++){
    if(dp[sum]!=-1)break;
    int diff=a[ord[i+p]]-a[ord[i]];
    int l=0,r=br.rem((long long)sum*br.pow(diff,p-2));
    while(r-l>1){
      int mid=(l+r)/2;
      if(dp[br.rem((long long)mid*diff)]!=-1)l=mid;
      else r=mid;
    }
    r=br.rem((long long)r*diff);
    dp[r]=i;
  }
  std::vector<bool>res(p*2-1,false);
  for(int i=0;i<p;i++)res[ord[i]]=true;
  while(sum!=0){
    int i=dp[sum];
    res[ord[i]]=false;
    res[ord[i+p]]=true;
    sum=br.rem(sum-(a[ord[i+p]]-a[ord[i]])+p);
  }
  return res;
}
std::vector<bool>egz(int n,std::vector<int>a){
  assert((int)a.size()==n*2-1);
  if(n==1)return {true};
  for(int p=2;p*p<=n;p++)if(n%p==0){
    int q=n/p;
    std::vector<int>rem_idx,rem;
    std::vector<int>zero_idx;
    rem_idx.reserve(p*2-1),rem.reserve(p*2-1);
    for(int i=0;i<p-1;i++){
      rem_idx.push_back(p*(q*2-1)+i);
      rem.push_back(a[rem_idx[i]]);
    }
    zero_idx.reserve(p*(q*2-1));
    for(int i=0;i<q*2-1;i++){
      for(int j=0;j<p;j++){
        rem_idx.push_back(i*p+j);
        rem.push_back(a[i*p+j]);
      }
      std::vector<bool>res=egz_prime(p,std::move(rem));
      std::vector<int>nrem_idx,nrem;
      nrem_idx.reserve(p*2-1),nrem.reserve(p*2-1);
      for(int j=0;j<p*2-1;j++){
        if(res[j])zero_idx.push_back(rem_idx[j]);
        else{
          nrem_idx.push_back(rem_idx[j]);
          nrem.push_back(a[rem_idx[j]]);
        }
      }
      rem_idx=std::move(nrem_idx);
      rem=std::move(nrem);
    }
    std::vector<int>f(q*2-1);
    for(int i=0;i<q*2-1;i++){
      int sum=0;
      for(int j=0;j<p;j++)sum+=a[zero_idx[i*p+j]];
      f[i]=sum/p;
    }
    std::vector<bool>idx=egz(q,f);
    std::vector<bool>res(n*2-1,false);
    for(int i=0;i<q*2-1;i++)if(idx[i]){
      for(int j=0;j<p;j++)res[zero_idx[i*p+j]]=true;
    }
    return res;
  }
  return egz_prime(n,std::move(a));
}
}
using egz_impl::egz;
#pragma once
#include<vector>
#include<limits>
#include<utility>
#include "datastructure/fast_stack.hpp"
#include "math/util.hpp"
template<typename Func>
std::vector<decltype(std::declval<Func>()(0,0))>monge_shortest_path(int n,const Func&f){
  using T=decltype(f(0,0));
  static constexpr T inf=std::numeric_limits<T>::max()/2;
  std::vector<T>dp(n,inf);
  std::vector<int>amin(n);
  dp[0]=0;
  dp[n-1]=f(0,n-1);
  fast_stack<std::pair<int,int>>st(msb(n)*3+1);
  st.emplace(0,n-1);
  while(!st.empty()){
    auto [l,r]=st.pop();
    if(l<0){
      l=~l,r=~r;
      int mid=(l+r)/2;
      for(int i=l+1;i<=mid;i++){
        T now=dp[i]+f(i,r);
        if(dp[r]>now){
          dp[r]=now;
          amin[r]=i;
        }
      }
    }
    else if(l+1<r){
      int mid=(l+r)/2;
      st.emplace(mid,r);
      st.emplace(~l,~r);
      st.emplace(l,mid);
      for(int i=amin[l];i<=amin[r];i++){
        T now=dp[i]+f(i,mid);
        if(dp[mid]>now){
          dp[mid]=now;
          amin[mid]=i;
        }
      }
    }
  }
  return dp;
}
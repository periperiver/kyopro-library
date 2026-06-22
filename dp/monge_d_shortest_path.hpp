#pragma once
#include<vector>
#include<limits>
#include<utility>
#include<cassert>
#include "monge_shortest_path.hpp"
#include "math/golden_search.hpp"
#include "other/overflow_check.hpp"
template<typename Func>
decltype(std::declval<Func>()(0,0))monge_d_shortest_path(int n,int d,decltype(std::declval<Func>()(0,0)) u,const Func&f){
  using T=decltype(std::declval<Func>()(0,0));
  assert(!is_overflow_mul<T>(n*3,u));
  return golden_search<T,false>(-u*3,u*3,[&](T c)->T {
    return monge_shortest_path(n,[&](int i,int j)->T {
      return f(i,j)+c;
    })[n-1]-c*d;
  }).second;
}
template<typename Func>
decltype(std::declval<Func>()(0,0))monge_less_d_shortest_path(int n,int d,decltype(std::declval<Func>()(0,0)) u,const Func&f){
  using T=decltype(std::declval<Func>()(0,0));
  assert(!is_overflow_mul<T>(n*3,u));
  static constexpr T inf=std::numeric_limits<T>::max();
  std::vector<T>dp(n,inf);
  std::vector<int>len(n);
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
          len[r]=len[i]+1;
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
          len[mid]=len[i]+1;
          amin[mid]=i;
        }
      }
    }
  }
  if(len[n-1]<=d)return dp[n-1];
  return monge_d_shortest_path(n,d,u,f);
}
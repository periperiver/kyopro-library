#pragma once
#include<vector>
#include<limits>
#include<numeric>
#include "math/util.hpp"
template<typename Func>
std::vector<decltype(std::declval<Func>()(0,0))>larsch(int n,const Func&f){
  using T=decltype(std::declval<Func>()(0,0));
  std::vector<T>dp(n+1,std::numeric_limits<T>::max());
  dp[0]=T();
  struct node{
    int sz,row,pos;
    std::vector<int>cols;
    std::vector<int>amin;
    std::vector<T>val;
    node(){}
    node(int n_):sz(n_),row(1),pos(0),cols(){
      amin.reserve(n_);
      val.reserve(n_);
    }
  };
  std::vector<node>dat(msb(n)+1);
  for(int i=0;i<(int)dat.size();i++)dat[i]=node(n>>i);
  dat[0].cols.resize(n);
  std::iota(dat[0].cols.begin(),dat[0].cols.end(),0);
  auto original=[&](int dep,int x)->int {
    for(int i=dep;i>0;i--)x=dat[i].cols[x];
    return x;
  };
  auto dfs=[&](auto self,int dep)->void {
    if(dep!=0){
      const auto cs=[&]()->std::vector<int> {
        if(dat[dep].row==1)return {0};
        else return {dat[dep].row*2-3,dat[dep].row*2-2};
      }();
      for(int j:cs){
        while(true){
          if(dat[dep].cols.size()<dat[dep].row)break;
          T lhs=[&](){
            int p=original(dep-1,dat[dep].cols.back());
            return dp[p]+f(p,dat[dep].cols.size()<<dep);
          }();
          T rhs=[&](){
            int p=original(dep-1,j);
            return dp[p]+f(p,dat[dep].cols.size()<<dep);
          }();
          if(lhs>rhs)dat[dep].cols.pop_back();
          else break;
        }
        if(dat[dep].cols.size()!=dat[dep].sz)dat[dep].cols.push_back(j);
      }
    }
    if(dat[dep].row&1){
      int left=dat[dep].pos;
      int right;
      if(dat[dep].row==dat[dep].sz)right=dat[dep].sz-1;
      else{
        self(self,dep+1);
        right=dat[dep+1].cols[dat[dep+1].amin.back()];
      }
      dat[dep].amin.push_back(left);
      int p=original(dep,left);
      dat[dep].val.push_back(dp[p]+f(p,dat[dep].row<<dep));
      dat[dep].pos=right;
      for(int i=left+1;i<=right;i++){
        p=original(dep,i);
        T now=dp[p]+f(p,dat[dep].row<<dep);
        if(dat[dep].val.back()>now){
          dat[dep].val.back()=now;
          dat[dep].amin.back()=i;
        }
      }
    }
    else{
      dat[dep].amin.push_back(dat[dep].pos);
      int p=original(dep,dat[dep].pos);
      dat[dep].val.push_back(dp[p]+f(p,dat[dep].row<<dep));
      p=original(dep,dat[dep].row-1);
      T now=dp[p]+f(p,dat[dep].row<<dep);
      if(dat[dep].val.back()>now){
        dat[dep].val.back()=now;
        dat[dep].amin.back()=dat[dep].row-1;
      }
    }
    dat[dep].row++;
  };
  for(int i=1;i<=n;i++){
    dfs(dfs,0);
    dp[i]=dat[0].val.back();
  }
  return dp;
}
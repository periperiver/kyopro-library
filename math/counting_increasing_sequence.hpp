#pragma once
#include<vector>
#include "math/util.hpp"
#include "convolution/ntt.hpp"
#include "factorial.hpp"
//lower[i]<=A[i]<=upper[i]
template<typename T>
T counting_increasing_sequence(std::vector<int>lower,std::vector<int>upper){
  assert(lower.size()==upper.size());
  int n=lower.size();
  for(int i=1;i<n;i++)if(lower[i-1]>lower[i])lower[i]=lower[i-1];
  for(int i=n-2;i>=0;i--)if(upper[i]>upper[i+1])upper[i]=upper[i+1];
  for(int i=0;i<n;i++)if(lower[i]>upper[i])return 0;
  int f=lower[0];
  for(int i=0;i<n;i++){
    lower[i]-=f;
    upper[i]-=f;
  }
  lower.insert(lower.begin(),0);
  upper.push_back(upper.back());
  int m=upper[n];
  std::vector<T>red(n+1),blue(m+1);
  //下限同じ、上限単調増加(愚直)
  auto naive1=[&](int lx,int rx,int ly,int ry)->void {
    std::vector<std::vector<T>>dp(rx-lx+1,std::vector<T>(ry-ly+1,0));
    for(int i=lx;i<=rx;i++)dp[i-lx][0]=red[i];
    for(int j=ly+1;j<=upper[lx];j++)dp[0][j-ly]=dp[0][j-ly-1];
    for(int i=lx+1;i<=rx;i++){
      for(int j=ly+1;j<=upper[i];j++)dp[i-lx][j-ly]=dp[i-lx-1][j-ly]+dp[i-lx][j-ly-1];
    }
    for(int j=ly;j<=ry;j++)blue[j]=dp.back()[j-ly];
  };
  //下限単調増加、上限同じ(愚直)
  auto naive2=[&](int lx,int rx,int ly,int ry)->void {
    std::vector<std::vector<T>>dp(rx-lx+1,std::vector<T>(ry-ly+1,0));
    for(int j=ly;j<=ry;j++)dp[0][j-ly]=blue[j];
    for(int i=lx+1;i<=rx;i++){
      dp[i-lx][lower[i]-ly]=dp[i-lx-1][lower[i]-ly];
      for(int j=lower[i]+1;j<=ry;j++){
        dp[i-lx][j-ly]=dp[i-lx-1][j-ly]+dp[i-lx][j-ly-1];
      }
    }
    for(int i=lx;i<=rx;i++)red[i]=dp[i-lx].back();
  };
  auto calc_naive=[&](int lx,int rx,int ly,int ry)->void {
    std::vector<std::vector<T>>dp(rx-lx+1,std::vector<T>(ry-ly+1,0));
    for(int i=lx;i<=rx;i++)dp[i-lx][0]=red[i];
    for(int i=ly;i<=ry;i++)dp[0][i-ly]=blue[i];
    for(int i=1;i<=rx-lx;i++)for(int j=1;j<=ry-ly;j++){
      dp[i][j]=dp[i-1][j]+dp[i][j-1];
    }
    for(int i=lx;i<=rx;i++)red[i]=dp[i-lx][ry-ly];
    for(int i=ly;i<=ry;i++)blue[i]=dp[rx-lx][i-ly];
  };
  //長方形の寄与を畳み込みで計算
  auto calc=[&](int lx,int rx,int ly,int ry){
    std::vector<T>l(blue.begin()+ly,blue.begin()+ry+1);
    std::vector<T>d(red.begin()+lx,red.begin()+rx+1);
    for(int i=lx;i<=rx;i++)red[i]=0;
    for(int i=ly;i<=ry;i++)blue[i]=0;
    int x=rx-lx,y=ry-ly;
    for(int i=x;i>=1;i--)d[i]-=d[i-1];
    for(int i=y;i>=1;i--)l[i]-=l[i-1];
    int s=ceil_pow2(x+y+2);
    T invs=T(s).inv();
    d[0]=0;
    {//下から上
      std::vector<T>coef(x+1);
      for(int i=0;i<=x;i++)coef[i]=F<T>::C(y+i,i);
      coef=ntt_convolution(coef,d);
      for(int i=0;i<=x;i++)red[i+lx]+=coef[i];
    }
    {//左から右
      std::vector<T>coef(y+1);
      for(int i=0;i<=y;i++)coef[i]=F<T>::C(x+i,i);
      coef=ntt_convolution(coef,l);
      for(int i=0;i<=y;i++)blue[i+ly]+=coef[i];
    }
    std::vector<T>coef(s,0);
    for(int i=0;i<=x+y;i++)coef[i]=F<T>::factorial(i);
    dft(coef);
    {//下から右
      for(int i=0;i<=x;i++)d[i]*=F<T>::factorial_inv(x-i);
      d.resize(s,0);
      dft(d);
      for(int i=0;i<s;i++)d[i]*=coef[i];
      idft(d);
      for(int i=0;i<=y;i++)blue[i+ly]+=d[i+x]*F<T>::factorial_inv(i)*invs;
    }
    {//左から上
      for(int i=0;i<=y;i++)l[i]*=F<T>::factorial_inv(y-i);
      l.resize(s,0);
      dft(l);
      for(int i=0;i<s;i++)l[i]*=coef[i];
      idft(l);
      for(int i=0;i<=x;i++)red[i+lx]+=l[i+y]*F<T>::factorial_inv(i)*invs;
    }
  };
  //下限同じ、上限単調増加(分割統治)
  auto dfs1=[&](auto self,int lx,int rx,int ly,int ry)->void {
    if((long long)(rx-lx)*(ry-ly)<=150){
      naive1(lx,rx,ly,ry);
      return;
    }
    int mid=-1;
    long long s=0;
    for(int i=lx;i<=rx;i++){
      long long now=(long long)(rx-i)*(upper[i]-ly);
      if(s<now)mid=i,s=now;
    }
    if(mid==-1){
      for(int i=ly;i<=ry;i++)blue[i]=red[rx];
      return;
    }
    int y=upper[mid];
    self(self,lx,mid,ly,y);
    red[mid]=blue[ly];
    calc(mid,rx,ly,y);
    self(self,mid,rx,y,ry);
  };
  //下限単調増加、上限同じ(分割統治)
  auto dfs2=[&](auto self,int lx,int rx,int ly,int ry)->void {
    if((long long)(rx-lx)*(ry-ly)<=150){
      naive2(lx,rx,ly,ry);
      return;
    }
    int mid=-1;
    long long s=0;
    for(int i=lx;i<=rx;i++){
      long long now=(long long)(i-lx)*(ry-lower[i]);
      if(s<now)mid=i,s=now;
    }
    if(mid==-1){
      for(int i=lx;i<=rx;i++)red[i]=blue[ry];
      return;
    }
    int y=lower[mid];
    self(self,lx,mid,ly,y);
    blue[y]=red[lx];
    calc(lx,mid,y,ry);
    self(self,mid,rx,y,ry);
  };
  int x=0,y=0;
  red[0]=1;
  while(x<n&&lower[x+1]==y)red[x+1]=red[x],x++;
  int lx=0,ly=0;
  while(true){
    ly=y;
    y=upper[x];
    dfs1(dfs1,lx,x,ly,y);
    if(x==n)break;
    lx=x;
    while(x<n&&lower[x+1]<=y)x++;
    dfs2(dfs2,lx,x,ly,y);
  }
  return blue.back();
}
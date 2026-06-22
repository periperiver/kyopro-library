#pragma once
#include<vector>
#include "square_free_number_sieve.hpp"
#include "kth_root.hpp"
std::vector<std::vector<std::pair<int,int>>>powerful_number_sieve(unsigned long long n){
  std::vector<std::vector<std::pair<int,int>>>res;
  int n2=kth_root(n,2);
  res.reserve(n2*2);
  std::vector<int>lpf(n2+1,-1);
  for(int i=2;i<=n2;i++)if(lpf[i]==-1){
    for(int j=1;i*j<=n2;j++)lpf[i*j]=i;
  }
  std::vector<int>pos(n2+1,-1);
  for(int y:square_free_number_sieve(kth_root(n,3))){
    unsigned long long y3=(unsigned long long)y*y*y;
    std::vector<std::pair<int,int>>ps;
    {
      int ny=y;
      while(ny>1){
        int p=lpf[ny];
        int e=1;
        ny/=lpf[ny];
        while(ny>1){
          if(lpf[ny]==p)e++,ny/=lpf[ny];
          else break;
        }
        pos[p]=ps.size();
        ps.emplace_back(p,e*3);
      }
    }
    for(unsigned long long x=1;;x++){
      unsigned long long now=x*x*y3;
      if(now<=n){
        int nx=x;
        std::vector<std::pair<int,int>>ps2(ps);
        while(nx>1){
          int p=lpf[nx];
          int e=1;
          nx/=lpf[nx];
          while(nx>1){
            if(lpf[nx]==p)e++,nx/=lpf[nx];
            else break;
          }
          if(pos[p]==-1){
            pos[p]=ps2.size();
            ps2.emplace_back(p,e*2);
          }
          else{
            ps2[pos[p]].second+=e*2;
          }
        }
        res.push_back(ps2);
        for(int i=ps.size();i<ps2.size();i++)pos[ps2[i].first]=-1;
      }
      else break;
    }
    for(int i=0;i<ps.size();i++)pos[ps[i].first]=-1;
  }
  return res;
}
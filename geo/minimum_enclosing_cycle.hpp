#pragma once
#include<vector>
#include<numeric>
#include "cycle_inout.hpp"
#include "../random/generator.hpp"
template<typename T,typename T4>
std::vector<bool>minimum_enclosing_cycle(std::vector<Point<T>>p){
  int n=p.size();
  int a=-1,b=-1,c=-1;
  std::vector<int>ord(n);
  std::iota(ord.begin(),ord.end(),0);
  std::shuffle(ord.begin(),ord.end(),Random::mt);
  for(int i:ord){
    if(c!=-1&&cycle_inout<T,T4>(p[a],p[b],p[c],p[i])>=0)continue;
    if(c==-1&&b!=-1&&cycle_inout<T>(p[a],p[b],p[i])>=0)continue;
    if(c==-1&&b==-1&&a!=-1&&p[a]==p[i])continue;
    a=i,b=c=-1;
    for(int j:ord){
      if(i==j)break;
      if(c!=-1&&cycle_inout<T,T4>(p[a],p[b],p[c],p[j])>=0)continue;
      if(c==-1&&b!=-1&&cycle_inout<T>(p[a],p[b],p[j])>=0)continue;
      if(c==-1&&b==-1&&a!=-1&&p[a]==p[j])continue;
      b=j,c=-1;
      for(int k:ord){
        if(j==k)break;
        if(c!=-1&&cycle_inout<T,T4>(p[a],p[b],p[c],p[k])>=0)continue;
        if(c==-1&&b!=-1&&cycle_inout<T>(p[a],p[b],p[k])>=0)continue;
        c=k;
      }
    }
  }
  std::vector<bool>res(n);
  if(c!=-1){
    for(int i=0;i<n;i++)res[i]=cycle_inout<T,T4>(p[a],p[b],p[c],p[i])==0;
  }
  else if(b!=-1){
    for(int i=0;i<n;i++)res[i]=cycle_inout<T>(p[a],p[b],p[i])==0;
  }
  else std::fill(res.begin(),res.end(),1);
  return res;
}
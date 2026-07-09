#pragma once
#include<vector>
#include<algorithm>
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
template<typename T,typename T4>
T minimum_enclosing_cycle_diameter(std::vector<Point<T>>p){
  static_assert(std::is_integral_v<T>);
  std::vector<bool>cycle=minimum_enclosing_cycle<T,T4>(p);
  std::vector<int>idx;
  for(int i=0;i<(int)p.size();i++)if(cycle[i])idx.push_back(i);
  if((int)idx.size()==1)return 0;
  else if((int)idx.size()==2){
    T v=(p[idx[0]]-p[idx[1]]).norm();
    T v2=std::sqrt(v);
    return v2+(v2*v2!=v);
  }
  else{
    T4 s=std::abs(cross(p[idx[1]]-p[idx[0]],p[idx[2]]-p[idx[0]]));
    T4 v=T4((p[idx[0]]-p[idx[1]]).norm())*T4((p[idx[1]]-p[idx[2]]).norm());
    T4 v2=(p[idx[2]]-p[idx[0]]).norm();
    T4 sum=0;
    sum+=(v/(s*s))*v2;
    v=v%(s*s);
    T4 a=v/s,b=v%s;
    T4 c=v2/s,d=v2%s;
    sum+=a*c;
    T4 u=a*d+b*c;
    sum+=u/s;
    u%=s;
    sum+=(u*s+b*d+s*s-1)/(s*s);
    T4 sq=std::sqrt(sum);
    return sq+(sq*sq!=sum);
  }
}
#pragma once
#include<vector>
#include "math/util.hpp"
#include "datastructure/fast_stack.hpp"
template<typename Func>
std::vector<int>monotone_minima(int n,int m,const Func&f){
  std::vector<int>res(n);
  fast_stack<std::tuple<int,int,int,int>>st(msb(n)*2+5);
  st.emplace(0,n,0,m);
  while(!st.empty()){
    const auto[lx,rx,ly,ry]=st.pop();
    if(lx==rx)continue;
    if(lx+1==rx){
      res[lx]=ly;
      for(int i=ly+1;i<ry;i++)if(f(lx,res[lx],i))res[lx]=i;
    }
    else{
      int mid=(lx+rx)>>1;
      res[mid]=ly;
      for(int i=ly+1;i<ry;i++)if(f(mid,res[mid],i))res[mid]=i;
      st.emplace(lx,mid,ly,res[mid]+1);
      st.emplace(mid+1,rx,res[mid],ry);
    }
  }
  return res;
}
#pragma once
#include<vector>
#include "datastructure/fast_stack.hpp"
template<typename T,bool MIN=true>
std::vector<int>cartesian_tree(const std::vector<T>&a){
  int n=a.size();
  std::vector<int>res(n,-1);
  fast_stack<int>st(n);
  for(int i=0;i<n;i++){
    int ls=-1;
    while(!st.empty()){
      int x=st.top();
      if constexpr(MIN){
        if(a[x]>=a[i]){
          ls=x;
          st.pop();
        }
        else break;
      }
      else{
        if(a[x]<=a[i]){
          ls=x;
          st.pop();
        }
        else break;
      }
    }
    if(ls!=-1)res[ls]=i;
    if(!st.empty())res[i]=st.top();
    st.push(i);
  }
  return res;
}
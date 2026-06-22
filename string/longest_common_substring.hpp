#pragma once
#include<tuple>
#include<algorithm>
#include "suffix_array.hpp"
#include "lcp_array.hpp"
//l1,l2,len
template<typename T>
std::tuple<int,int,int>longest_common_substring(T a,const T&b){
  auto mx=std::max(*std::max_element(a.begin(),a.end()),*std::max_element(b.begin(),b.end()))+1;
  int n=a.size();
  a.insert(a.end(),mx);
  a.insert(a.end(),b.begin(),b.end());
  a.insert(a.end(),mx+1);
  std::vector<int>sa=suffix_array(a);
  std::vector<int>lcp=lcp_array(a,sa);
  int l=0,r=0,ans=0;
  for(int i=0;i<(int)lcp.size();i++){
    if((sa[i]<n)!=(sa[i+1]<n)){
      int nl,nr;
      if(sa[i]<n)nl=sa[i],nr=sa[i+1]-n-1;
      else nl=sa[i+1],nr=sa[i]-n-1;
      if(ans<lcp[i]){
        ans=lcp[i];
        l=nl;
        r=nr;
      }
    }
  }
  return std::make_tuple(l,r,ans);
}
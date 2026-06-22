#pragma once
#include<vector>
#include<cassert>
template<typename T>
std::vector<int>lcp_array(const T&s,const std::vector<int>&sa){
  assert(s.size()==sa.size());
  int n=s.size();
  std::vector<int>rank(n);
  for(int i=0;i<n;i++)rank[sa[i]]=i;
  std::vector<int>lcp(n-1);
  for(int i=0,h=0;i<n;i++){
    if(h>0)h--;
    if(rank[i]==0)continue;
    int j=sa[rank[i]-1];
    for(;i+h<n&&j+h<n;h++){
      if(s[i+h]!=s[j+h])break;
    }
    lcp[rank[i]-1]=h;
  }
  return lcp;
}
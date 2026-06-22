#pragma once
#include<vector>
template<typename T>
std::vector<int>manacher(const T&s){
  std::vector<int>res(s.size());
  for(int i=0,j=0,k;i<s.size();i+=k,j-=k){
    while(i-j>=0&&i+j<s.size()&&s[i-j]==s[i+j])j++;
    res[i]=j;
    k=1;
    while(i-k>=0&&res[i-k]+k<j)res[i+k]=res[i-k],k++;
  }
  return res;
}
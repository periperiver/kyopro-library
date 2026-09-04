#define PROBLEM "https://judge.yosupo.jp/problem/suffixarray"
#include<iostream>
#include<vector>
#include<algorithm>
#include<numeric>
#include "string/rolling_hash.hpp"
int main(){
  std::string s;
  std::cin>>s;
  RollingHash rh(s);
  std::vector<int>ans(s.size());
  std::iota(ans.begin(),ans.end(),0);
  std::sort(ans.begin(),ans.end(),[&](int lhs,int rhs){
    int l=RollingHashBase::lcp(rh,lhs,s.size(),rh,rhs,s.size());
    if(rhs+l==s.size())return false;
    if(lhs+l==s.size())return true;
    return s[lhs+l]<s[rhs+l];
  });
  for(int i=0;i<(int)ans.size();i++)std::cout<<ans[i]<<" \n"[i+1==ans.size()];
}
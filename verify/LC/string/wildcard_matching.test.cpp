#define PROBLEM "https://judge.yosupo.jp/problem/wildcard_pattern_matching"
#include<iostream>
#include "string/wildcard_matching.hpp"
int main(){
  std::string s,t;
  std::cin>>s>>t;
  auto ans=wildcard_matching(s,t,'*');
  for(int i=0;i<(int)ans.size();i++)std::cout<<ans[i];
  std::cout<<'\n';
}
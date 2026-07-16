#define PROBLEM "https://judge.yosupo.jp/problem/zalgorithm"
#include<iostream>
#include "string/z_algorithm.hpp"
int main(){
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::string s;
  std::cin>>s;
  std::vector<int>ans=z_algorithm(s);
  for(int i=0;i<(int)ans.size();i++)std::cout<<ans[i]<<" \n"[i+1==(int)ans.size()];
}
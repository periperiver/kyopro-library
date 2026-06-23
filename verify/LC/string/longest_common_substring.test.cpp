#define PROBLEM "https://judge.yosupo.jp/problem/longest_common_substring"
#include<iostream>
#include "string/longest_common_substring.hpp"
int main(){
  std::string s,t;
  std::cin>>s>>t;
  auto [l1,l2,len]=longest_common_substring(s,t);
  std::cout<<l1<<' '<<l1+len<<' '<<l2<<' '<<l2+len<<std::endl;
}
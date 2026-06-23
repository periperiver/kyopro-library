#define PROBLEM "https://judge.yosupo.jp/problem/suffixarray"
#include<iostream>
#include "string/suffix_array.hpp"
int main(){
  std::string s;
  std::cin>>s;
  std::vector<int>sa=suffix_array(s);
  for(int i=0;i<(int)sa.size();i++)std::cout<<sa[i]<<" \n"[i+1==(int)sa.size()];
}
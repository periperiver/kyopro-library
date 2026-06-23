#define PROBLEM "https://judge.yosupo.jp/problem/eertree"
#include<iostream>
#include "string/eertree.hpp"
int main(){
  std::string s;
  std::cin>>s;
  for(char&c:s)c-='a';
  eertree<26>e(s);
  std::cout<<e.distinct_palindrome()<<std::endl;
  for(const auto&nd:e.nds)if(nd.idx>=2)std::cout<<nd.par->idx-1<<' '<<nd.link->idx-1<<'\n';
  for(int i=0;i<(int)s.size();i++)std::cout<<e[i+1]->idx-1<<" \n"[i+1==(int)s.size()];
}
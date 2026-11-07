#define PROBLEM "https://judge.yosupo.jp/problem/eertree"
#include<iostream>
#include "string/eertree.hpp"
int main(){
  std::string s;
  std::cin>>s;
  eertree<26>e;
  std::vector<int>idx;
  idx.reserve(s.size());
  for(char c:s)idx.push_back(e.add(c-'a'));
  std::cout<<e.size()-2<<std::endl;
  for(int i=2;i<e.size();i++)std::cout<<e[i].par-1<<' '<<e[i].suffix-1<<'\n';
  for(int i=0;i<(int)idx.size();i++)std::cout<<idx[i]-1<<" \n"[i+1==(int)idx.size()];
}
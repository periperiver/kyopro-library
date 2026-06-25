#define PROBLEM "https://judge.yosupo.jp/problem/aho_corasick"
#include<iostream>
#include "string/aho.hpp"
int main(){
  int n;
  std::cin>>n;
  AhoCorasick<26>aho;
  std::vector<int>v(n);
  for(int i=0;i<n;i++){
    std::string s;
    std::cin>>s;
    for(char&c:s)c-='a';
    v[i]=aho.add(s);
  }
  aho.build_aho<false>();
  std::vector<int>par(aho.size());
  for(int i=0;i<aho.size();i++){
    for(int j=0;j<26;j++)if(aho[i][j]!=-1)par[aho[i][j]]=i;
  }
  for(int i=1;i<aho.size();i++)std::cout<<par[i]<<' '<<aho[i].failure<<'\n';
  for(int i=0;i<n;i++)std::cout<<v[i]<<" \n"[i+1==n];
}
#define PROBLEM "https://judge.yosupo.jp/problem/cartesian_tree"
#include<iostream>
#include "graph/cartesiantree.hpp"
int main(){
  int n;
  std::cin>>n;
  std::vector<int>p(n);
  for(int&x:p)std::cin>>x;
  std::vector<int>c=cartesian_tree(p);
  for(int i=0;i<n;i++){
    if(c[i]==-1)c[i]=i;
    std::cout<<c[i]<<" \n"[i+1==n];
  }
}
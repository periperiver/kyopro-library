#define PROBLEM "https://judge.yosupo.jp/problem/longest_increasing_subsequence"
#include "dp/lis.hpp"
int main(){
  int n;
  std::cin>>n;
  std::vector<int>a(n);
  for(int&x:a)std::cin>>x;
  a=longest_increasing_subsequence(a);
  std::cout<<a.size()<<'\n';
  for(int i=0;i<(int)a.size();i++)std::cout<<a[i]<<" \n"[i+1==(int)a.size()];
}
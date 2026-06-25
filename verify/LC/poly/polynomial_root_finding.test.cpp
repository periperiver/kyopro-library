#define PROBLEM "https://judge.yosupo.jp/problem/polynomial_root_finding"
#include "poly/polynomial_root_finding.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  std::cin>>n;
  std::vector<mint>f(n+1);
  for(mint&x:f)std::cin>>x;
  f=polynomial_root_finding(f);
  std::cout<<f.size()<<std::endl;
  for(int i=0;i<(int)f.size();i++)std::cout<<f[i]<<" \n"[i+1==(int)f.size()];
}
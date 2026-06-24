#define PROBLEM "https://judge.yosupo.jp/problem/multipoint_evaluation"
#include "poly/multipoint_evaluation.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,m;
  std::cin>>n>>m;
  std::vector<mint>f(n),p(m);
  for(mint&x:f)std::cin>>x;
  for(mint&x:p)std::cin>>x;
  p=multipoint_evaluation(f,p);
  for(int i=0;i<m;i++)std::cout<<p[i]<<" \n"[i+1==m];
}
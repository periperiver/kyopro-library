#define PROBLEM "https://judge.yosupo.jp/problem/division_of_polynomials"
#include "poly/div_mod.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,m;
  std::cin>>n>>m;
  std::vector<mint>f(n),g(m);
  for(mint&x:f)std::cin>>x;
  for(mint&x:g)std::cin>>x;
  auto [q,r]=poly_div_mod(f,g);
  std::cout<<q.size()<<' '<<r.size()<<std::endl;
  for(int i=0;i<(int)q.size();i++)std::cout<<q[i]<<" \n"[i+1==(int)q.size()];
  for(int i=0;i<(int)r.size();i++)std::cout<<r[i]<<" \n"[i+1==(int)r.size()];
}
#define PROBLEM "https://judge.yosupo.jp/problem/inv_of_polynomials"
#include "poly/half_gcd.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,m;
  std::cin>>n>>m;
  std::vector<mint>f(n),g(m);
  for(mint&x:f)std::cin>>x;
  for(mint&x:g)std::cin>>x;
  auto [a,b,c]=half_gcd<mint,true>(f,g);
  if((int)c.size()==1){
    mint inv=c[0].inv();
    std::cout<<a.size()<<std::endl;
    for(int i=0;i<(int)a.size();i++)std::cout<<a[i]*inv<<" \n"[i+1==a.size()];
  }
  else std::cout<<"-1\n";
}
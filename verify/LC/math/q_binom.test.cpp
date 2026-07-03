#define PROBLEM "https://judge.yosupo.jp/problem/q_binomial_coefficient_prime_mod"
#include "math/q_binom.hpp"
#include "math/arbitrary_modint.hpp"
using mint=arbitrary_modint<int,1>;
int main(){
  int t,m,q;
  std::cin>>t>>m>>q;
  mint::set_mod(m);
  std::vector<std::pair<int,int>>query(t);
  int lim=0;
  for(auto&[n,k]:query)std::cin>>n>>k,lim=std::max(lim,n);
  qBinom<mint>a(q,lim);
  for(const auto&[n,k]:query)std::cout<<a.C(n,k)<<'\n';
}
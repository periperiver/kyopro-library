#define PROBLEM "https://judge.yosupo.jp/problem/binomial_coefficient_prime_mod"
#include "math/factorial.hpp"
#include "math/arbitrary_modint.hpp"
using mint=arbitrary_modint<1>;
int main(){
  int t,m;
  std::cin>>t>>m;
  mint::set_mod(m);
  while(t--){
    int n,k;
    std::cin>>n>>k;
    std::cout<<F<mint>::C(n,k)<<'\n';
  }
}
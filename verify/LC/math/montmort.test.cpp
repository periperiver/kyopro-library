#define PROBLEM "https://judge.yosupo.jp/problem/montmort_number_mod"
#include "math/montmort.hpp"
#include "math/arbitrary_modint.hpp"
using mint=arbitrary_modint<2>;
int main(){
  int n,m;
  std::cin>>n>>m;
  mint::set_mod(m);
  std::vector<mint>ans=montmort<mint>(n);
  for(int i=1;i<=n;i++)std::cout<<ans[i]<<" \n"[i==n];
}
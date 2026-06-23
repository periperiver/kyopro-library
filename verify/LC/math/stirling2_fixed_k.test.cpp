#define PROBLEM "https://judge.yosupo.jp/problem/stirling_number_of_the_second_kind_fixed_k"
#include "math/stirling2_fixed_k.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,k;
  std::cin>>n>>k;
  std::vector<mint>ans=stirling2_fixed_k<mint>(n,k);
  for(int i=0;i<=n-k;i++)std::cout<<ans[i]<<" \n"[i==n-k];
}
#define PROBLEM "https://judge.yosupo.jp/problem/stirling_number_of_the_second_kind"
#include "math/stirling2_fixed_n.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  std::cin>>n;
  std::vector<mint>ans=stirling2_fixed_n<mint>(n);
  for(int i=0;i<=n;i++)std::cout<<ans[i]<<" \n"[i==n];
}
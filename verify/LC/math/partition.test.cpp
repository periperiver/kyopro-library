#define PROBLEM "https://judge.yosupo.jp/problem/partition_function"
#include "math/partition.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  std::cin>>n;
  std::vector<mint>ans=partition_number<mint>(n);
  for(int i=0;i<=n;i++)std::cout<<ans[i]<<" \n"[i==n];
}
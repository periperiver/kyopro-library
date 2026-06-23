#define PROBLEM "https://judge.yosupo.jp/problem/sharp_p_subset_sum"
#include "dp/count_subset_sum.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,t;
  std::cin>>n>>t;
  std::vector<mint>a(t+1);
  for(int i=0;i<n;i++){
    int s;
    std::cin>>s;
    a[s]++;
  }
  a=count_subset_sum(a);
  for(int i=1;i<=t;i++)std::cout<<a[i]<<" \n"[i==t];
}
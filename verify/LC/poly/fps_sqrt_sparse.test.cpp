#define PROBLEM "https://judge.yosupo.jp/problem/sqrt_of_formal_power_series_sparse"
#include "poly/fps_sparse.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,k;
  std::cin>>n>>k;
  std::vector<mint>a(n);
  for(int i=0;i<k;i++){
    int j;
    mint v;
    std::cin>>j>>v;
    a[j]+=v;
  }
  a=fps_sqrt_sparse(a);
  if(a.empty())std::cout<<"-1\n";
  else for(int i=0;i<n;i++)std::cout<<a[i]<<" \n"[i+1==n];
}
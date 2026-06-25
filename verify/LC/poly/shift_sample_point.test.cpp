#define PROBLEM "https://judge.yosupo.jp/problem/shift_of_sampling_points_of_polynomial"
#include "poly/shift_sample_point.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,m;
  mint c;
  std::cin>>n>>m>>c;
  std::vector<mint>f(n);
  for(mint&x:f)std::cin>>x;
  f=shift_sample_point(f,c,m);
  for(int i=0;i<m;i++)std::cout<<f[i]<<" \n"[i+1==m];
}
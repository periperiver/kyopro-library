#define PROBLEM "https://judge.yosupo.jp/problem/polynomial_interpolation"
#include "poly/multipoint_interpolate.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  std::cin>>n;
  std::vector<mint>x(n),y(n);
  for(mint&v:x)std::cin>>v;
  for(mint&v:y)std::cin>>v;
  x=multipoint_interpolate(x,y);
  for(int i=0;i<n;i++)std::cout<<x[i]<<" \n"[i+1==n];
}
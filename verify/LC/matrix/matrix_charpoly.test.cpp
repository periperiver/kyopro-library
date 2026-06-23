#define PROBLEM "https://judge.yosupo.jp/problem/characteristic_polynomial"
#include "matrix/poly_matrix.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  std::cin>>n;
  std::vector<std::vector<mint>>a(n,std::vector<mint>(n));
  for(int i=0;i<n;i++)for(int j=0;j<n;j++)std::cin>>a[i][j];
  std::vector<mint>b=matrix_charpoly(a);
  for(int i=0;i<=n;i++)std::cout<<b[i]<<" \n"[i==n];
}
#define PROBLEM "https://judge.yosupo.jp/problem/pow_of_matrix"
#include "matrix/matrix.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  long long k;
  std::cin>>n>>k;
  std::vector<std::vector<mint>>a(n,std::vector<mint>(n));
  for(int i=0;i<n;i++)for(int j=0;j<n;j++)std::cin>>a[i][j];
  a=matrix_pow(a,k);
  for(int i=0;i<n;i++)for(int j=0;j<n;j++)std::cout<<a[i][j]<<" \n"[j+1==n];
}
#define PROBLEM "https://judge.yosupo.jp/problem/inverse_matrix"
#include "matrix/matrix.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  std::cin>>n;
  std::vector<std::vector<mint>>a(n,std::vector<mint>(n));
  for(int i=0;i<n;i++)for(int j=0;j<n;j++)std::cin>>a[i][j];
  a=matrix_inv(a);
  if(a.empty())std::cout<<"-1\n";
  else{
    for(int i=0;i<n;i++)for(int j=0;j<n;j++)std::cout<<a[i][j]<<" \n"[j+1==n];
  }
}
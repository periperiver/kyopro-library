#define PROBLEM "https://judge.yosupo.jp/problem/matrix_product"
#include "matrix/matrix.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,m,k;
  std::cin>>n>>m>>k;
  std::vector<std::vector<mint>>a(n,std::vector<mint>(m)),b(m,std::vector<mint>(k));
  for(int i=0;i<n;i++)for(int j=0;j<m;j++)std::cin>>a[i][j];
  for(int i=0;i<m;i++)for(int j=0;j<k;j++)std::cin>>b[i][j];
  a=matrix_mul(a,b);
  for(int i=0;i<n;i++)for(int j=0;j<k;j++)std::cout<<a[i][j]<<" \n"[j+1==k];
}
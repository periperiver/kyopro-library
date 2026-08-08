#define PROBLEM "https://judge.yosupo.jp/problem/matrix_rank"
#include "math/modint.hpp"
#include "matrix/gauss_elimination.hpp"
using mint=mint998;
int main(){
  std::cin.tie(nullptr)->sync_with_stdio(false);
  int n,m;
  std::cin>>n>>m;
  std::vector<std::vector<mint>>a(n,std::vector<mint>(m));
  for(int i=0;i<n;i++)for(int j=0;j<m;j++)std::cin>>a[i][j];
  if(n>m){
    std::vector<std::vector<mint>>na(m,std::vector<mint>(n));
    for(int i=0;i<n;i++)for(int j=0;j<m;j++)na[j][i]=a[i][j];
    a=std::move(na);
    std::swap(n,m);
  }
  gauss_elimination(a);
  std::cout<<a.size()<<std::endl;
}
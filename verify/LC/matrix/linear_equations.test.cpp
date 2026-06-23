#define PROBLEM "https://judge.yosupo.jp/problem/system_of_linear_equations"
#include "matrix/linear_equations.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,m;
  std::cin>>n>>m;
  std::vector<std::vector<mint>>a(n,std::vector<mint>(m));
  std::vector<mint>c(n);
  for(int i=0;i<n;i++)for(int j=0;j<m;j++)std::cin>>a[i][j];
  for(int i=0;i<n;i++)std::cin>>c[i];
  auto [c,d]=matrix_linear_equation(a,c);
  if(c.empty())std::cout<<"-1\n";
  else{
    std::cout<<d.size()<<std::endl;
    for(int i=0;i<m;i++)std::cout<<c[i]<<" \n"[i+1==m];
    for(int i=0;i<(int)d.size();i++)for(int j=0;j<m;j++)std::cout<<d[i][j]<<" \n"[j+1==m];
  }
}
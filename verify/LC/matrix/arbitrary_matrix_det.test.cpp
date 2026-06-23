#define PROBLEM "https://judge.yosupo.jp/problem/matrix_det_arbitrary_mod"
#include "matrix/arbitrary_matrix_det.hpp"
#include "math/arbitrary_modint.hpp"
using mint=arbitrary_modint<1>;
int main(){
  int n,m;
  std::cin>>n>>m;
  mint::set_mod(m);
  std::vector<std::vector<mint>>a(n,std::vector<mint>(n));
  for(int i=0;i<n;i++)for(int j=0;j<n;j++)std::cin>>a[i][j];
  std::cout<<arbitrary_matrix_det(a)<<std::endl;
}
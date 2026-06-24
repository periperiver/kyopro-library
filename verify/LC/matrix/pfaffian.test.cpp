#define PROBLEM "https://judge.yosupo.jp/problem/pfaffian_of_matrix"
#include "matrix/pfaffian.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  std::cin>>n;
  std::vector<std::vector<mint>>a(n*2,std::vector<mint>(n*2));
  for(int i=0;i<n*2;i++)for(int j=0;j<n*2;j++)std::cin>>a[i][j];
  std::cout<<pfaffian(a)<<std::endl;
}
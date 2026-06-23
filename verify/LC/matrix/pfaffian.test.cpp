#define PROBLEM "https://judge.yosupo.jp/problem/pfaffian_of_matrix"
#include "matrix/pfaffian.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  std::cin>>n;
  std::vector<std::vector<mint>>a(n*2,std::vector<mint>(n*2));
  std::cout<<pfaffian(a)<<std::endl;
}
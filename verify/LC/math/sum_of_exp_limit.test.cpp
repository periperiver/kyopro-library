#define PROBLEM "https://judge.yosupo.jp/problem/sum_of_exponential_times_polynomial_limit"
#include "math/sum_of_exp_limit.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  mint r;
  int d;
  std::cin>>r>>d;
  std::cout<<sum_of_exp_limit(r,d)<<std::endl;
}
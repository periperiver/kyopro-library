#define PROBLEM "https://judge.yosupo.jp/problem/sum_of_exponential_times_polynomial"
#include "math/sum_of_exp.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  mint r;
  int d;
  long long n;
  std::cin>>r>>d>>n;
  std::cout<<sum_of_exp(r,d,n)<<std::endl;
}
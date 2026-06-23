#define PROBLEM "https://judge.yosupo.jp/problem/sum_of_totient_function"
#include "math/totient_sum.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  long long n;
  std::cin>>n;
  std::cout<<totient_sum<mint>(n).back()<<'\n';
}
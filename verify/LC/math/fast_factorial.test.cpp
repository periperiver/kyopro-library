#define PROBLEM "https://judge.yosupo.jp/problem/many_factorials"
#include "math/fast_factorial.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int t;
  std::cin>>t;
  while(t--){
    int n;
    std::cin>>n;
    std::cout<<fast_factorial<mint>(n)<<'\n';
  }
}
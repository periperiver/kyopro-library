#define PROBLEM "https://judge.yosupo.jp/problem/gcd_of_gaussian_integers"
#include "math/gaussint.hpp"
int main(){
  int t;
  std::cin>>t;
  while(t--){
    gauss_int<long long> a,b;
    std::cin>>a>>b;
    std::cout<<gcd(a,b)<<'\n';
  }
}
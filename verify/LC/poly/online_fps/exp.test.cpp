#define PROBLEM "https://judge.yosupo.jp/problem/exp_of_formal_power_series"
#include "poly/online_fps/exp.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  std::cin.tie(nullptr)->sync_with_stdio(false);
  int n;
  std::cin>>n;
  OnlineExp<mint>f;
  for(int i=0;i<n;i++){
    mint a;
    std::cin>>a;
    std::cout<<f.query(a)<<" \n"[i+1==n];
  }
}
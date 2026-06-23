#define PROBLEM "https://judge.yosupo.jp/problem/discrete_logarithm_mod"
#include "math/discrete_logarithm.hpp"
int main(){
  int t;
  std::cin>>t;
  while(t--){
    int x,y,m;
    std::cin>>x>>y>>m;
    std::cout<<discrete_logarithm(x,y,m)<<'\n';
  }
}
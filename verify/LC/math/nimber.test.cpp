#define PROBLEM "https://judge.yosupo.jp/problem/nim_product_64"
#include "math/nimber.hpp"
int main(){
  int t;
  std::cin>>t;
  while(t--){
    Nimber a,b;
    std::cin>>a>>b;
    std::cout<<a*b<<'\n';
  }
}
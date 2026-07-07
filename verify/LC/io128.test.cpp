#define PROBLEM "https://judge.yosupo.jp/problem/many_aplusb_128bit"
#include "io128.hpp"
int main(){
  std::cin.tie(nullptr)->sync_with_stdio(false);
  int t;
  std::cin>>t;
  while(t--){
    __int128_t a,b;
    std::cin>>a>>b;
    std::cout<<a+b<<'\n';
  }
}
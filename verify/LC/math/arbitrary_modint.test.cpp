#define PROBLEM "https://judge.yosupo.jp/problem/sqrt_mod"
#include "math/arbitrary_modint.hpp"
using mint=arbitrary_modint<int,12>;
int main(){
  std::cin.tie(nullptr)->sync_with_stdio(false);
  int t;
  std::cin>>t;
  while(t--){
    int y,p;
    std::cin>>y>>p;
    mint::set_mod(p);
    auto ans=mint::raw(y).sqrt();
    if(ans)std::cout<<*ans<<'\n';
    else std::cout<<"-1\n";
  }
}
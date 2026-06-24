#define PROBLEM "https://judge.yosupo.jp/problem/sum_of_multiplicative_function"
#include "math/multiplicative_sum.hpp"
#include "math/modint.hpp"
using mint=mod_int<469762049>;
int main(){
  mint inv2=mint(2).inv();
  int t;
  std::cin>>t;
  while(t--){
    long long n;
    mint a,b;
    std::cin>>n>>a>>b;
    MultiplicativeSum ms(n);
    auto pc=ms.calc<mint>([&](long long x){return 1;},[&](long long x){return x;});
    auto ps=ms.calc<mint>([&](mint x){return x;},[&](mint x){return x*(x+1)*inv2;});
    for(int i=0;i<(int)pc.size();i++)pc[i]=pc[i]*a+ps[i]*b;
    std::cout<<ms.solve<mint,false>(pc,[&](int p,int e){return a*mint::raw(e)+b*mint::raw(p);})<<'\n';
  }
}
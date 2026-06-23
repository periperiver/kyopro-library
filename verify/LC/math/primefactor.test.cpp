#define PROBLEM "https://judge.yosupo.jp/problem/factorize"
#include<iostream>
#include<algorithm>
#include "math/primefactor.hpp"
int main(){
  int q;
  std::cin>>q;
  while(q--){
    long long n;
    std::cin>>n;
    auto ans=factorize(n);
    std::sort(ans.begin(),ans.end());
    std::cout<<ans.size();
    for(auto p:ans)std::cout<<' '<<p;
    std::cout<<std::endl;
  }
}
#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "math/pow_sum.hpp"
#include "math/modint.hpp"
#include "random/generator.hpp"
#include<cassert>
using mint1=mint998;
using mint2=mint107;
using mint3=mint61;
template<typename T,int k>
T naive(int n){
  T res;
  for(int i=1;i<=n;i++)res+=T(i).pow(k);
  return res;
}
template<typename T,int k>
void test(T n){
  assert((pow_sum<T,k>(n)==naive<T,k>(n.val())));
}
template<int k>
void test_rec(int nlim,int t){
  if constexpr(k>=1){
    test_rec<k-1>(nlim,t);
    while(t--){
      int n=Random::range(1,nlim+1);
      test<mint1,k>(n);
      test<mint2,k>(n);
      test<mint3,k>(n);
    }
  }
}
int main(){
  test_rec<20>(10000,100);
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}
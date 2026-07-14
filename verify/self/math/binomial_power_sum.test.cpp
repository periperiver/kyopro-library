#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "math/binomial_power_sum.hpp"
#include "math/modint.hpp"
#include "random/generator.hpp"
template<typename T>
T naive(int n,int k){
  T res=0;
  for(int i=0;i<=n;i++)res+=F<T>::C(n,i)*T(i).pow(k);
  return res;
}
template<typename T>
void test(int n,int k){
  T na=naive<T>(n,k);
  T ans=binomial_power_sum<T>(n,k);
  assert(na==ans);
}
int main(){
  for(int n=0;n<=300;n++){
    for(int k=0;k<=300;k++){
      test<mint998>(n,k);
      test<mint107>(n,k);
    }
  }
  for(int i=0;i<100;i++){
    int n=Random::range(301,100000);
    int k=Random::range(301,100000);
    test<mint998>(n,k);
    test<mint107>(n,k);
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}
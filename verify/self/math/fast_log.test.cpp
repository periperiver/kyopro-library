#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "math/fast_log.hpp"
#include "random/generator.hpp"
#include "math/discrete_logarithm.hpp"
#include<cassert>
constexpr int mod1=998244353;
constexpr int mod2=1000000007;
constexpr int mod3=101;
template<int mod>
void test(int a,int b){
  int ans=fast_log<mod>::log(a,b);
  int na=discrete_logarithm(a,b,mod);
  assert(ans==na);
}
int main(){
  for(int i=0;i<10000;i++){
    int a=Random::range(mod1),b=Random::range(mod1);
    test<mod1>(a,b);
    a=Random::range(mod2),b=Random::range(mod2);
  }
  for(int i=0;i<mod3;i++)for(int j=0;j<mod3;j++)test<mod3>(i,j);
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}
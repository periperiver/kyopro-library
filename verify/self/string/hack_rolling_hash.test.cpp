#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include "string/hack_rolling_hash.hpp"
#include "random/generator.hpp"
#include "math/arbitrary_modint.hpp"
constexpr int sigma=26;
template<typename T>
void test(T g,T mod){
  using mint=arbitrary_modint<T,1>;
  using type=std::conditional_t<(std::numeric_limits<T>::digits<=32),long long,__int128_t>;
  auto a=hack_rolling_hash<type>(g,mod,sigma);
  assert(std::any_of(a.begin(),a.end(),[](T x){return x!=0;}));
  assert(std::all_of(a.begin(),a.end(),[&](T x){return (x>0?x:-x)<sigma;}));
  mint::set_mod(mod);
  mint sum=0;
  for(int i=0;i<(int)a.size();i++)sum+=a[i]*mint::raw(g).pow(i);
  assert(sum.val()==0);
}
int main(){
  for(int mod:{123457,998244353}){
    for(int i=0;i<100;i++){
      int g=Random::range(1,mod);
      test(g,mod);
    }
  }
  for(long long mod:{92709568269121ll,(1ll<<61)-1}){
    for(int i=0;i<100;i++){
      long long g=Random::range(1ll,mod);
      test(g,mod);
    }
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}
#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include "string/hack_rolling_hash.hpp"
#include "random/prime.hpp"
#include "math/arbitrary_modint.hpp"
#include "math/modint.hpp"
constexpr int sigma=26;
template<typename U,typename T>
T eval(std::vector<U>f,T x){
  T res=f.back();
  for(int i=(int)f.size()-2;i>=0;i--)res=res*x+f[i];
  return res;
}
template<typename T>
void test(std::vector<T>g){
  auto a=hack_rolling_hash(g,sigma);
  assert(std::any_of(a.begin(),a.end(),[](auto x){return x!=0;}));
  assert(std::all_of(a.begin(),a.end(),[&](auto x){return (x>0?x:-x)<sigma;}));
  for(T v:g)assert(eval(a,v).val()==0);
}
using mint1=arbitrary_modint<int,1>;
using mint2=arbitrary_modint<long long,1>;
int main(){
  for(int n=1;n<5;n++){
    for(int i=0;i<30;i++){
      std::vector<mint998>g;
      for(int j=0;j<n;j++)g.push_back(Random::range(mint998::mod()));
      test(g);
    }
    for(int i=0;i<30;i++){
      std::vector<mint107>g;
      for(int j=0;j<n;j++)g.push_back(Random::range(mint107::mod()));
      test(g);
    }
    for(int i=0;i<30;i++){
      int mod=Random::prime(100,1000000000);
      mint1::set_mod(mod);
      std::vector<mint1>g;
      for(int j=0;j<n;j++)g.push_back(Random::range(mod));
      test(g);
    }
    if(n<=2){
      for(int i=0;i<30;i++){
        std::vector<mint61>g;
        for(int j=0;j<n;j++)g.push_back(Random::range(mint61::mod()));
        test(g);
      }
      for(int i=0;i<30;i++){
        long long mod=Random::prime<long long>(1000000000,1000000000000000000);
        mint2::set_mod(mod);
        std::vector<mint2>g;
        for(int j=0;j<n;j++)g.push_back(Random::range(mod));
        test(g);
      }
    }
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}
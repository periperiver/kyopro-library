#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "poly/arbitrary_polynomial_root_finding.hpp"
#include "math/modint.hpp"
#include "math/arbitrary_modint.hpp"
#include "random/generator.hpp"
#include "random/prime.hpp"
using mint1=arbitrary_modint<int,1>;
using mint2=arbitrary_modint<long long,1>;
template<typename T>
std::pair<std::vector<T>,std::vector<T>>gen(int n){
  std::vector<T>root(n);
  for(int i=0;i<n;i++)root[i]=Random::get<unsigned long long>();
  std::sort(root.begin(),root.end());
  root.erase(std::unique(root.begin(),root.end()),root.end());
  n=root.size();
  std::vector<T>dp(n+1);
  dp[0]=1;
  for(T c:root){
    for(int i=n;i>=1;i--)dp[i]=-dp[i]*c+dp[i-1];
    dp[0]*=-c;
  }
  T coef=Random::range<typename T::value_type>(1,T::mod());
  for(int i=0;i<=n;i++)dp[i]*=coef;
  return std::make_pair(root,dp);
}
template<typename T>
void test1(int n){
  auto [root,f]=gen<T>(n);
  auto res=arbitrary_polynomial_root_finding(f);
  assert(root==res);
}
template<typename T>
std::vector<T>naive(std::vector<T>f){
  std::vector<T>res;
  for(int i=0;i<T::mod();i++){
    T now=f.back();
    for(int j=(int)f.size()-2;j>=0;j--)now=now*i+f[j];
    if(now.val()==0)res.push_back(i);
  }
  return res;
}
template<typename T>
void test2(int n){
  std::vector<T>f(n);
  while(std::all_of(f.begin(),f.end(),[](T x){return x.val()==0;})){
    for(int i=0;i<n;i++)f[i]=Random::get<unsigned>();
  }
  auto na=naive(f);
  auto ans=arbitrary_polynomial_root_finding(f);
  assert(na==ans);
}
int main(){
  for(int mod:{2,3,5,7,11,13,17,19,23}){
    mint1::set_mod(mod);
    for(int n=0;n<=20;n++){
      for(int i=0;i<100;i++){
        test1<mint1>(n);
      }
    }
    for(int n=1;n<=20;n++){
      for(int i=0;i<100;i++){
        test2<mint1>(n);
      }
    }
  }
  for(int t=0;t<100;t++){
    int mod=Random::prime(1,1000000000);
    mint1::set_mod(mod);
    int n=Random::range(100);
    test1<mint1>(n);
  }
  for(int t=0;t<100;t++){
    long long mod=Random::prime<long long>(1,1ll<<61);
    mint2::set_mod(mod);
    int n=Random::range(100);
    test1<mint2>(n);
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}
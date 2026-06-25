#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include<cassert>
#include "poly/power_prod_sum.hpp"
#include "math/modint.hpp"
#include "random/generator.hpp"
using mint=mint998;
std::vector<mint>naive(std::vector<mint>f,std::vector<mint>g,int m){
  assert(f.size()==g.size());
  int n=f.size();
  std::vector<mint>res(m);
  for(int j=0;j<m;j++){
    for(int i=0;i<n;i++)res[j]+=f[i]*g[i].pow(j);
  }
  return res;
}
void test(){
  int n=Random::range(1,3000);
  int m=Random::range(1,3000);
  std::vector<mint>f(n),g(n);
  for(int i=0;i<n;i++)f[i]=Random::range(mint::mod());
  for(int i=0;i<n;i++)g[i]=Random::range(mint::mod());
  assert(power_prod_sum(f,g,m)==naive(f,g,m));
}
void zero(){
  int n=Random::range(1,1000);
  int m=Random::range(1,1000);
  std::vector<mint>f(n),g(n);
  assert(power_prod_sum(f,g,m)==naive(f,g,m));
}
int main(){
  for(int i=0;i<30;i++)test();
  zero();
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}
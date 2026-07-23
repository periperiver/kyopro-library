#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "convolution/semi_relaxed_convolution.hpp"
#include "math/modint.hpp"
#include "random/generator.hpp"
using mint1=modint<4605071356474687489>;
using mint2=modint<4603910272195756033>;
template<typename T>
std::vector<T>solve(std::vector<T>a,std::vector<T>b){
  std::vector<T>res(a.size()+b.size()-1);
  a.resize(a.size()+b.size()-1);
  SemiRelaxedConvolution<T>rc(a);
  for(int i=0;i<(int)res.size();i++){
    T v=i<(int)b.size()?b[i]:T();
    res[i]=rc.query(v);
  }
  return res;
}
template<typename T>
void test(std::vector<T>a,std::vector<T>b){
  assert(solve(a,b)==ntt_convolution(a,b));
}
template<typename T>
std::vector<T>gen(int n){
  std::vector<T>res(n);
  for(T&x:res)x=T::raw(Random::range(T::mod()));
  return res;
}
int main(){
  for(int i=0;i<100;i++){
    int n=Random::range(1,10000),m=Random::range(1,10000);
    test(gen<mint1>(n),gen<mint1>(m));
    test(gen<mint2>(n),gen<mint2>(m));
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}
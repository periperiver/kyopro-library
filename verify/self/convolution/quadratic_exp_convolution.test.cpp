#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "convolution/quadratic_exp_convolution.hpp"
#include "math/modint.hpp"
#include "random/generator.hpp"
#include<cassert>
using mint1=mint998;
using mint2=modint<897581057>;
template<typename T>
std::vector<T> naive(std::vector<T>a,std::vector<T>b,T c){
  std::vector<T>res(a.size()+b.size()-1);
  for(int i=0;i<(int)a.size();i++)for(int j=0;j<(int)b.size();j++){
    res[i+j]+=a[i]*b[j]*c.pow((long long)i*j);
  }
  return res;
}
template<typename T>
void test(std::vector<T>a,std::vector<T>b,T c){
  assert(quadratic_exp_convolution(a,b,c)==naive(a,b,c));
}
template<typename T>
std::vector<T>gen(int n){
  std::vector<T>res(n);
  for(int i=0;i<n;i++)res[i]=Random::range(T::mod());
  return res;
}
int main(){
  for(int i=1;i<=40;i++)for(int j=1;j<=40;j++){
    for(int k=0;k<20;k++){
      test<mint1>(gen<mint1>(i),gen<mint1>(j),k);
      test<mint2>(gen<mint2>(i),gen<mint2>(j),k);
    }
  }
  for(int k=0;k<100;k++){
    int n=Random::range(1,1000),m=Random::range(1,1000);
    test<mint1>(gen<mint1>(n),gen<mint1>(m),k);
    test<mint2>(gen<mint2>(n),gen<mint2>(m),k);
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}
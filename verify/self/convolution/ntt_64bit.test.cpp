#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "convolution/ntt.hpp"
#include "math/modint.hpp"
#include "random/generator.hpp"
using mint1=modint<9214540759460478977>;
using mint2=modint<9223336852482686977>;
template<typename T>
std::vector<T>naive(std::vector<T>a,std::vector<T>b){
  std::vector<T>res(a.size()+b.size()-1);
  for(int i=0;i<(int)a.size();i++)for(int j=0;j<(int)b.size();j++)res[i+j]+=a[i]*b[j];
  return res;
}
template<typename T>
void test(std::vector<T>a,std::vector<T>b){
  assert(ntt_convolution(a,b)==naive(a,b));
}
template<typename T>
std::vector<T>gen(int n){
  std::vector<T>res(n);
  for(T&x:res)x=T::raw(Random::range(T::mod()));
  return res;
}
int main(){
  for(int i=0;i<100;i++){
    int n=Random::range(100,5000);
    int m=Random::range(100,5000);
    test(gen<mint1>(n),gen<mint1>(m));
    test(gen<mint2>(n),gen<mint2>(m));
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}
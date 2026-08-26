#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "poly/vandermonde_det.hpp"
#include "math/modint.hpp"
#include "random/generator.hpp"
using mint=mint998;
mint naive(std::vector<mint>f){
  mint res=1;
  for(int i=0;i<(int)f.size();i++)for(int j=i+1;j<(int)f.size();j++)res*=f[j]-f[i];
  return res;
}
void test(std::vector<mint>f){
  assert(vandermonde_det(f)==naive(f));
}
std::vector<mint>gen(int n){
  std::vector<mint>res(n);
  for(int i=0;i<n;i++)res[i]=Random::get<unsigned long long>();
  return res;
}
int main(){
  for(int n=1;n<100;n++)test(gen(n));
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}
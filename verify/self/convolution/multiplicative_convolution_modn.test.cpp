#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "convolution/multiplicative_convolution_modn.hpp"
#include "random/generator.hpp"
#include "math/arbitrary_modint.hpp"
using mint=arbitrary_modint<1>;
std::vector<mint>naive(const std::vector<mint>&a,const std::vector<mint>&b){
  assert(a.size()==b.size());
  int n=a.size();
  std::vector<mint>res(n);
  for(int i=0;i<n;i++)for(int j=0;j<n;j++)res[(long long)i*j%n]+=a[i]*b[j];
  return res;
}
std::vector<mint>solve(const std::vector<mint>&a,const std::vector<mint>&b){
  return multiplicative_convolution_modn(a,b);
}
void test(int n){
  int mod=Random::range(2,1000000000);
  mint::set_mod(mod);
  std::vector<mint>a(n),b(n);
  for(int i=0;i<n;i++)a[i]=mint::raw(Random::range(mod));
  for(int i=0;i<n;i++)b[i]=mint::raw(Random::range(mod));
  assert(solve(a,b)==naive(a,b));
}
int main(){
  for(int n=1;n<=300;n++)test(n);
  /*
  960=2^6*3*5
  9000=2^3*3^2*5^3
  9991=97*103
  30030=2*3*5*7*11*13
  */
  for(int n:{960,9000,9991,30030})test(n);
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}
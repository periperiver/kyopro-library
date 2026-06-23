#define PROBLEM "https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence"
#include "poly/bostan_mori.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int d;
  long long k;
  std::cin>>d>>k;
  std::vector<mint>a(d),c(d+1);
  for(int i=0;i<d;i++)std::cin>>a[i];
  c[0]=1;
  for(int i=1;i<=d;i++){
    std::cin>>c[i];
    c[i]=-c[i];
  }
  a=ntt_convolution(a,c);
  a.resize(d);
  std::cout<<bostan_mori(a,c,k)<<std::endl;
}
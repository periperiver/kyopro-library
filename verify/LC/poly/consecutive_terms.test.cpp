#define PROBLEM "https://judge.yosupo.jp/problem/consecutive_terms_of_linear_recurrent_sequence"
#include "poly/consecutive_terms.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int d,m;
  long long k;
  std::cin>>d>>k>>m;
  std::vector<mint>a(d),c(d+1);
  for(int i=0;i<d;i++)std::cin>>a[i];
  c[0]=1;
  for(int i=1;i<=d;i++){
    std::cin>>c[i];
    c[i]=-c[i];
  }
  a=ntt_convolution(a,c);
  a.resize(d);
  a=consecutive_terms(a,c,k,m);
  for(int i=0;i<m;i++)std::cout<<a[i]<<" \n"[i+1==m];
}
#define PROBLEM "https://judge.yosupo.jp/problem/pow_of_matrix"
#include "fastio.hpp"
#include "other/semigroup_pow.hpp"
#include "matrix/strassen.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  long long k;
  rd(n),rd(k);
  std::vector<std::vector<mint>>a(n,std::vector<mint>(n));
  for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      int x;
      rd(x);
      a[i][j]=mint::raw(x);
    }
  }
  if(k==0){
    for(int i=0;i<n;i++)for(int j=0;j<n;j++)wt(i==j),wt(" \n"[j+1==n]);
    return 0;
  }
  a=semigroup_pow(a,k,strassen<mint>);
  for(int i=0;i<n;i++)for(int j=0;j<n;j++)wt(a[i][j].val()),wt(" \n"[j+1==n]);
}
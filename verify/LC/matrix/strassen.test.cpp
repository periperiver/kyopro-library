#define PROBLEM "https://judge.yosupo.jp/problem/matrix_product"
#include "fastio.hpp"
#include "matrix/strassen.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,m,k;
  rd(n),rd(m),rd(k);
  std::vector<std::vector<mint>>a(n,std::vector<mint>(m)),b(m,std::vector<mint>(k));
  for(int i=0;i<n;i++)for(int j=0;j<m;j++){
    int x;
    rd(x);
    a[i][j]=mint::raw(x);
  }
  for(int i=0;i<m;i++)for(int j=0;j<k;j++){
    int x;
    rd(x);
    b[i][j]=mint::raw(x);
  }
  a=strassen(a,b);
  for(int i=0;i<n;i++)for(int j=0;j<k;j++)wt(a[i][j].val()),wt(" \n"[j+1==k]);
}
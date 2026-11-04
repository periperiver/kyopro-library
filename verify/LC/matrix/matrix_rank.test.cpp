#define PROBLEM "https://judge.yosupo.jp/problem/matrix_rank"
#include "matrix/matrix_rank.hpp"
#include "fastio.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,m;
  rd(n),rd(m);
  std::vector<std::vector<mint>>a(n,std::vector<mint>(m));
  for(int i=0;i<n;i++)for(int j=0;j<m;j++){
    int x;
    rd(x);
    a[i][j]=mint::raw(x);
  }
  wt(matrix_rank(a)),wt('\n');
}
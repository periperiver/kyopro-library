#define PROBLEM "https://judge.yosupo.jp/problem/sparse_matrix_det"
#include "matrix/matrix_det_sparse.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,k;
  std::cin>>n>>k;
  std::vector<std::tuple<int,int,mint>>a(k);
  for(int i=0;i<k;i++){
    int r,c;
    mint v;
    std::cin>>r>>c>>v;
    a[i]={r,c,v};
  }
  std::cout<<matrix_det_sparse(n,a)<<std::endl;
}
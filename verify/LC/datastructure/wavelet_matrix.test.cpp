#define PROBLEM "https://judge.yosupo.jp/problem/range_kth_smallest"
#include "fastio.hpp"
#include "datastructure/wavelet_matrix.hpp"
int main(){
  int n,q;
  rd(n),rd(q);
  std::vector<int>a(n);
  WaveletMatrix wm(a);
  while(q--){
    int l,r,k;
    rd(l),rd(r),rd(k);
    wt(wm.kth_smallest(l,r,k)),wt('\n');
  }
}
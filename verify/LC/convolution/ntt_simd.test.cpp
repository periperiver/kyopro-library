#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod"
#include "convolution/ntt_simd.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  std::cin.tie(nullptr)->sync_with_stdio(false);
  int n,m;
  std::cin>>n>>m;
  int z=std::max(8,ceil_pow2(n+m-1));
  std::vector<mint>a(n),b(m);
  for(mint&x:a)std::cin>>x;
  for(mint&x:b)std::cin>>x;
  a.resize(z),b.resize(z);
  ntt_simd_impl::dft_simd(a);
  ntt_simd_impl::dft_simd(b);
  for(int i=0;i<z;i++)a[i]*=b[i];
  ntt_simd_impl::idft_simd(a);
  mint inv=mint(z).inv();
  for(int i=0;i<n+m-1;i++)std::cout<<a[i]*inv<<" \n"[i+1==n+m-1];
}
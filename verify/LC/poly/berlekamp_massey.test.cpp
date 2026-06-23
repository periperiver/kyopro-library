#define PROBLEM "https://judge.yosupo.jp/problem/find_linear_recurrence"
#include "poly/berlekamp_massey.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  std::cin>>n;
  std::vector<mint>a(n);
  for(mint&x:a)std::cin>>x;
  a=berlekamp_massey(a);
  std::cout<<a.size()-1<<'\n';
  for(int i=1;i<(int)a.size();i++)std::cout<<-a[i]<<" \n"[i+1==(int)a.size()];
}
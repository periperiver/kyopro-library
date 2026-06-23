#define PROBLEM "https://judge.yosupo.jp/problem/number_of_subsequences"
#include "dp/count_subsequence.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n;
  std::cin>>n;
  std::vector<int>a(n);
  for(int&x:a)std::cin>>x;
  std::cout<<count_subsequence<mint>(a)<<std::endl;
}
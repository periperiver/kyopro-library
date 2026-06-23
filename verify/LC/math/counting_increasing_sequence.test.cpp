#define PROBLEM "https://judge.yosupo.jp/problem/number_of_increasing_sequences_between_two_sequences"
#include "math/counting_increasing_sequence.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,m;
  std::cin>>n>>m;
  std::vector<int>a(n),b(n);
  for(int&x:a)std::cin>>x;
  for(int&x:b)std::cin>>x,x--;
  std::cout<<counting_increasing_sequence<mint>(a,b)<<std::endl;
}
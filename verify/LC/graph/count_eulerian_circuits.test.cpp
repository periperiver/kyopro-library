#define PROBLEM "https://judge.yosupo.jp/problem/counting_eulerian_circuits"
#include "graph/count_eulerian_circuits.hpp"
#include "math/modint.hpp"
using mint=mint998;
int main(){
  int n,m;
  std::cin>>n>>m;
  Graph g(n,true);
  g.read<0,0>(m);
  std::cout<<count_eulerian_circuits<mint>(g)<<std::endl;
}
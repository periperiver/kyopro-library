#define PROBLEM "https://judge.yosupo.jp/problem/chromatic_number"
#include "graph/chromatic_number.hpp"
int main(){
  int n,m;
  std::cin>>n>>m;
  Graph g(n,false);
  g.read<0,0>(m);
  std::cout<<chromatic_number(g)<<std::endl;
}
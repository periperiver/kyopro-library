#define PROBLEM "https://judge.yosupo.jp/problem/manhattanmst"
#include "geo/manhattan_mst.hpp"
int main(){
  int n;
  std::cin>>n;
  std::vector<Point<long long>>a(n);
  for(int i=0;i<n;i++)std::cin>>a[i];
  auto t=manhattan_mst(a);
  long long cost=0;
  for(auto e:t)cost+=e.weight;
  std::cout<<cost<<'\n';
  for(auto e:t)std::cout<<e.from<<' '<<e.to<<'\n';
}
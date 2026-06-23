#define PROBLEM "https://judge.yosupo.jp/problem/euclidean_mst"
#include "geo/euclidean_mst.hpp"
int main(){
  int n;
  std::cin>>n;
  std::vector<Point<long long>>a(n);
  for(int i=0;i<n;i++)std::cin>>a[i];
  auto t=euclidean_mst<long long,long long>(a);
  for(auto e:t)std::cout<<e.from<<' '<<e.to<<'\n';
}
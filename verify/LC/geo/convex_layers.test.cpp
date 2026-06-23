#define PROBLEM "https://judge.yosupo.jp/problem/convex_layers"
#include "geo/convex_layers.hpp"
int main(){
  int n;
  std::cin>>n;
  std::vector<Point<long long>>a(n);
  for(int i=0;i<n;i++)std::cin>>a[i];
  std::vector<int>ans=convex_layers(a);
  for(int i=0;i<n;i++)std::cout<<ans[i]<<'\n';
}
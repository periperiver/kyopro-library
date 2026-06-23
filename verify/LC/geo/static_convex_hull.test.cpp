#define PROBLEM "https://judge.yosupo.jp/problem/static_convex_hull"
#include "geo/static_convex_hull.hpp"
int main(){
  int t;
  std::cin>>t;
  while(t--){
    int n;
    std::cin>>n;
    std::vector<Point<long long>>a(n);
    for(int i=0;i<n;i++)std::cin>>a[i];
    a=static_convex_hull(a);
    std::cout<<a.size()<<'\n';
    for(int i=0;i<(int)a.size();i++)std::cout<<a[i]<<'\n';
  }
}
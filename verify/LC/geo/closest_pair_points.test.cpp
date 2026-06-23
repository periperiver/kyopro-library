#define PROBLEM "https://judge.yosupo.jp/problem/closest_pair"
#include "geo/closest_pair_points.hpp"
int main(){
  int t;
  std::cin>>t;
  while(t--){
    int n;
    std::cin>>n;
    std::vector<Point<long long>>a(n);
    for(int i=0;i<n;i++)std::cin>>a[i];
    auto[i,j]=closest_pair_points(a);
    std::cout<<i<<' '<<j<<'\n';
  }
}
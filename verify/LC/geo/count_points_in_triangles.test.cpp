#define PROBLEM "https://judge.yosupo.jp/problem/count_points_in_triangle"
#include "geo/count_points_in_triangles.hpp"
int main(){
  int n;
  std::cin>>n;
  std::vector<Point<long long>>a(n);
  for(Point<long long>&x:a)std::cin>>x;
  int m;
  std::cin>>m;
  std::vector<Point<long long>>b(m);
  for(Point<long long>&x:b)std::cin>>x;
  CountPointsInTriangles<long long>c(a,b);
  int q;
  std::cin>>q;
  while(q--){
    int x,y,z;
    std::cin>>x>>y>>z;
    std::cout<<c.query(x,y,z)<<'\n';
  }
}
#define PROBLEM "https://judge.yosupo.jp/problem/count_points_in_triangle"
#include "geo/count_points_in_triangles.hpp"
int main(){
  int n,m;
  std::cin>>n>>m;
  std::vector<Point<long long>>a(n),b(m);
  for(int i=0;i<n;i++)std::cin>>a[i];
  for(int i=0;i<m;i++)std::cin>>b[i];
  CountPointsInTriangles<long long>c(a,b);
  int q;
  std::cin>>q;
  while(q--){
    int x,y,z;
    std::cin>>x>>y>>z;
    std::cout<<c.query(x,y,z)<<'\n';
  }
}
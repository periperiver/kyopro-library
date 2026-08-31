#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/2/DSL_2_C"
#include<iostream>
#include "datastructure/rectangle_query.hpp"
int main(){
  int n;
  std::cin>>n;
  std::vector<std::pair<int,int>>a(n);
  for(auto&[x,y]:a)std::cin>>x>>y;
  RectangleQuery rq(a);
  std::vector<int>vs(rq.size());
  for(int i=0;i<n;i++)rq.get_vs(a[i].first,a[i].second,[&](int p){vs[p]=i;});
  int q;
  std::cin>>q;
  while(q--){
    int lx,rx,ly,ry;
    std::cin>>lx>>rx>>ly>>ry;
    rx++,ry++;
    std::vector<int>ans;
    rq.get_range(lx,rx,ly,ry,[&](int l,int r){
      for(int i=l;i<r;i++)ans.push_back(vs[i]);
    });
    std::sort(ans.begin(),ans.end());
    for(int x:ans)std::cout<<x<<'\n';
    std::cout<<'\n';
  }
}
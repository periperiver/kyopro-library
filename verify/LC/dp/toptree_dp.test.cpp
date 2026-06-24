#define PROBLEM "https://judge.yosupo.jp/problem/point_set_tree_path_composite_sum"
#include<ranges>
#include "dp/toptree_dp.hpp"
#include "math/modint.hpp"
using mint=mint998;
struct DP{
  struct Path{
    mint a,b;
    mint val;
    int sz;
  };
  using Data=Path;
  struct Point{
    mint val;
    int sz;
  };
  static Path compress(Path lhs,Path rhs){
    Path res;
    res.a=lhs.a*rhs.a;
    res.b=lhs.b+lhs.a*rhs.b;
    res.val=lhs.val+rhs.val*lhs.a+mint::raw(rhs.sz)*lhs.b;
    res.sz=lhs.sz+rhs.sz;
    return res;
  }
  static Point rake(Point lhs,Point rhs){
    Point res;
    res.val=lhs.val+rhs.val;
    res.sz=lhs.sz+rhs.sz;
    return res;
  }
  static Path add_vertex(Point p,Data d){
    Path res;
    res.a=d.a;
    res.b=d.b;
    res.val=d.val+d.a*p.val+d.b*mint::raw(p.sz);
    res.sz=p.sz+d.sz;
    return res;
  }
  static Point add_edge(Path p){
    Point res;
    res.val=p.val;
    res.sz=p.sz;
    return res;
  }
};
int main(){
  int n,q;
  std::cin>>n>>q;
  TopTreeDP<DP>dp(n*2-1);
  for(int i=0;i<n;i++){
    mint a;
    std::cin>>a;
    dp.set(i,{1,0,a,1});
  }
  std::vector<std::pair<mint,mint>>bc(n-1);
  for(int i=0;i<n-1;i++){
    int u,v;
    std::cin>>u>>v>>bc[i].first>>bc[i].second;
    dp.link(u,i+n);
    dp.link(v,i+n);
  }
  for(auto[id,v]:bc|std::views::enumerate){
    dp.set(id+n,{v.first,v.second,0,0});
  }
  while(q--){
    int t;
    std::cin>>t;
    if(t==0){
      int w,x;
      std::cin>>w>>x;
      dp.set(w,{1,0,x,1});
    }
    else{
      int e,y,z;
      std::cin>>e>>y>>z;
      dp.set(e+n,{y,z,0,0});
    }
    int r;
    std::cin>>r;
    std::cout<<dp.get(r).val<<'\n';
  }
}
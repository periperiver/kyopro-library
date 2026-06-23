#define PROBLEM "https://judge.yosupo.jp/problem/point_set_tree_path_composite_sum_fixed_root"
#include "dp/dynamic_tree_dp.hpp"
#include "math/modint.hpp"
using mint=mint998;
struct DP{
  struct Data{
    mint a,b,v,sz;
  };
  static Data compress(Data lhs,Data rhs){
    Data res;
    res.a=lhs.a*rhs.a;
    res.b=lhs.b*rhs.a+rhs.b;
    res.v=lhs.v+rhs.v*lhs.a+rhs.sz*lhs.b;
    res.sz=lhs.sz+rhs.sz;
    return res;
  }
  static Data rake(Data lhs,Data rhs){
    Data res;
    res.a=lhs.a,res.b=lhs.a;
    res.v=lhs.a+rhs.a;
    res.sz=lhs.sz+rhs.sz;
    return res;
  }
};
int main(){
  int n,q;
  std::cin>>n>>q;
  std::vector<DP::Data>init(n);
  for(int i=0;i<n;i++){
    mint a;
    std::cin>>a;
    init[i].v=a;
    init[i].sz=1;
  }
  Tree t(n);
  std::vector<std::pair<mint,mint>>bc(n-1);
  for(int i=0;i<n-1;i++){
    int u,v;
    mint b,c;
    std::cin>>u>>v>>b>>c;
    t.add_edge(u,v);
    bc[i]={b,c};
  }
  t.build();
  t.remove_parent();
  std::vector<int>eid(n-1);
  for(int i=1;i<n;i++)eid[t.parent_edge(i).index]=i;
  for(int i=0;i<n-1;i++){
    init[i].a=bc[i].first;
    init[i].b=bc[i].second;
    init[i].v=init[i].a*init[i].v+init[i].b;
  }
  DynamicTreeDP<DP>dp(t,init);
  while(q--){
    int type;
    std::cin>>type;
    if(type==0){
      int w;
      mint x;
      std::cin>>w>>x;
    }
  }
}
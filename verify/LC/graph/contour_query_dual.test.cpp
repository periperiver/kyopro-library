#define PROBLEM "https://judge.yosupo.jp/problem/vertex_get_range_contour_add_on_tree"
#include "graph/contour_query.hpp"
#include "datastructure/bit.hpp"
#include "monoid/add.hpp"
int main(){
  int n,q;
  std::cin>>n>>q;
  std::vector<int>a(n);
  for(int&x:a)std::cin>>x;
  Tree t(n);
  t.read<0,0>();
  ContourQuery cq(t);
  BinaryIndexedTree<MonoidAdd<long long>>BIT(cq.size());
  while(q--){
    int type;
    std::cin>>type;
    if(type==0){
      int p,l,r,x;
      std::cin>>p>>l>>r>>x;
      cq.get_range(p,l,r,[&](int l2,int r2){
        BIT.add(l2,x);
        if(r2!=cq.size())BIT.add(r2,-x);
      });
    }
    else{
      int p;
      std::cin>>p;
      long long ans=a[p];
      cq.get_vs(p,[&](int x){ans+=BIT.sum(0,x+1);});
      std::cout<<ans<<'\n';
    }
  }
}
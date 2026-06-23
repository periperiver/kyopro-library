#define PROBLEM "https://judge.yosupo.jp/problem/vertex_add_range_contour_sum_on_tree"
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
  std::vector<long long>init(cq.size());
  for(int i=0;i<n;i++)cq.get_vs(i,[&](int x){init[x]+=a[i];});
  BinaryIndexedTree<MonoidAdd<long long>>BIT(std::move(init));
  while(q--){
    int type;
    std::cin>>type;
    if(type==0){
      int p,x;
      std::cin>>p>>x;
      cq.get_vs(p,[&](int i){BIT.add(i,x);});
    }
    else{
      int p,l,r;
      std::cin>>p>>l>>r;
      long long ans=0;
      cq.get_range(p,l,r,[&](int l2,int r2){ans+=BIT.sum(l2,r2);});
      std::cout<<ans<<'\n';
    }
  }
}
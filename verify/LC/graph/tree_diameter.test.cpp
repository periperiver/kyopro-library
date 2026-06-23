#define PROBLEM "https://judge.yosupo.jp/problem/tree_diameter"
#include "graph/tree_base.hpp"
int main(){
  int n;
  std::cin>>n;
  Tree<long long>t(n);
  t.read<1,0>();
  auto [len,ans]=t.diameter();
  std::cout<<len<<' '<<ans.size()<<'\n';
  for(int i=0;i<(int)ans.size();i++)std::cout<<ans[i]<<" \n"[i+1==(int)ans.size()];
}
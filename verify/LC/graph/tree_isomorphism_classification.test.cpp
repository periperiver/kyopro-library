#define PROBLEM "https://judge.yosupo.jp/problem/rooted_tree_isomorphism_classification"
#include "graph/tree_isomorphism_classification.hpp"
int main(){
  int n;
  std::cin>>n;
  Tree t(n);
  t.readp<0>();
  auto ans=tree_isomorphism_classification(t);
  std::cout<<*std::max_element(ans.begin(),ans.end())+1<<'\n';
  for(int i=0;i<n;i++)std::cout<<ans[i]<<" \n"[i+1==n];
}
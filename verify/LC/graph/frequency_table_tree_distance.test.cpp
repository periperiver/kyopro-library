#define PROBLEM "https://judge.yosupo.jp/problem/frequency_table_of_tree_distance"
#include "graph/frequency_table_tree_distance.hpp"
int main(){
  int n;
  std::cin>>n;
  Tree t(n);
  t.read<0,0>();
  auto ans=frequency_table_tree_distance(t);
  for(int i=1;i<n;i++)std::cout<<ans[i]<<" \n"[i+1==n];
}
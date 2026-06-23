#define PROBLEM "https://judge.yosupo.jp/problem/jump_on_tree"
#include "graph/jump_on_tree.hpp"
int main(){
  int n,q;
  std::cin>>n>>q;
  Tree t(n);
  t.read<0,0>();
  JumponTree jt(t);
  while(q--){
    int s,t,i;
    std::cin>>s>>t>>i;
    std::cout<<jt.jump(s,t,i)<<'\n';
  }
}
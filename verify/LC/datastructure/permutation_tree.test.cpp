#define PROBLEM "https://judge.yosupo.jp/problem/common_interval_decomposition_tree"
#include "datastructure/permutation_tree.hpp"
int main(){
  int n;
  std::cin>>n;
  std::vector<int>p(n);
  for(int&x:p)std::cin>>x;
  PermutationTree pt(p);
  std::cout<<pt.size()<<'\n';
  for(int i=0;i<pt.size();i++){
    std::cout<<pt[i].par<<' '<<pt[i].l<<' '<<pt[i].r<<' ';
    if(pt[i].type==pt.prime)std::cout<<"prime\n";
    else std::cout<<"linear\n";
  }
}
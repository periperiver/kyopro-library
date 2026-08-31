#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/beta/room.html#HUPC2020Day1/problems/B"
#include<iostream>
#include "datastructure/dual_sparse_table.hpp"
#include "monoid/add_pair.hpp"
int main(){
  int n,q;
  std::cin>>n>>q;
  std::vector<std::tuple<int,int,std::pair<int,int>>>query(q);
  for(int i=0;i<q;i++){
    int l,k;
    std::cin>>l>>k;
    l--;
    query[i]={l,l+k,{1,1-l}};
  }
  auto ans=dual_sparse_table<MonoidAddPair<int,int>>(n,query);
  for(int i=0;i<n;i++)std::cout<<(long long)ans[i].first*i+ans[i].second<<" \n"[i+1==n];
}
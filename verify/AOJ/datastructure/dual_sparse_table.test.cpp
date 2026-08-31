#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/challenges/sources/VPC/HUPC/3165?year=2020"
#include<iostream>
#include "datastructure/dual_sparse_table.hpp"
#include "monoid/add_pair.hpp"
int main(){
  int n,q;
  std::cin>>n>>q;
  std::vector<std::tuple<int,int,std::pair<int,long long>>>query(q);
  for(int i=0;i<q;i++){
    int l,k;
    std::cin>>l>>k;
    l--;
    query[i]={l,l+k,{1,1-l}};
  }
  auto ans=dual_sparse_table<MonoidAddPair<int,long long>>(n,query);
  for(int i=0;i<n;i++)std::cout<<(long long)ans[i].first*i+ans[i].second<<" \n"[i+1==n];
}
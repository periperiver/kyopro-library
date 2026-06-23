#define PROBLEM "https://judge.yosupo.jp/problem/sort_points_by_argument"
#include<iostream>
#include "geo/argsort.hpp"
int main(){
  int n;
  std::cin>>n;
  std::vector<Point<long long>>a(n);
  for(int i=0;i<n;i++)std::cin>>a[i];
  std::vector<int>ord=arg_sort(a);
  for(int i=0;i<n;i++)std::cout<<a[ord[i]]<<'\n';
}
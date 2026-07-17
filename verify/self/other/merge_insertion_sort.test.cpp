#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include<iostream>
#include<algorithm>
#include<cassert>
#include "other/merge_insertion_sort.hpp"
#include "random/permutation.hpp"
template<int lim>
void test(std::vector<int>a){
  int n=a.size();
  int cnt=0;
  std::vector<int>ord=merge_insertion_sort(n,[&](int lhs,int rhs){
    cnt++;
    return a[lhs]<a[rhs];
  });
  assert(cnt<=lim);
  for(int i=0;i<n-1;i++)assert(a[ord[i]]<=a[ord[i+1]]);
}
int main(){
  test<0>({});
  test<0>({0});
  for(int n=2;n<5;n++){
    std::vector<int>a(n);
    std::iota(a.begin(),a.end(),0);
    do{
      test<7>(a);
    }while(std::next_permutation(a.begin(),a.end()));
  }
  for(int i=0;i<100;i++){
    int n=Random::range(2,30000);
    test<1<<23>(Random::permutation(n));
  }
  for(int i=0;i<100;i++){
    int n=Random::range(2,30000);
    std::vector<int>a(n);
    for(int i=0;i<n;i++)a[i]=Random::range(5);
    test<1<<23>(std::move(a));
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}
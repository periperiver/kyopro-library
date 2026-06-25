#define PROBLEM "https://yukicoder.me/problems/no/705"
#include<iostream>
#include "dp/monge_shortest_path.hpp"
int main(){
  std::cin.tie(nullptr)->sync_with_stdio(false);
  int n;
  std::cin>>n;
  std::vector<long long>a(n),x(n),y(n);
  for(long long&v:a)std::cin>>v;
  for(long long&v:x)std::cin>>v;
  for(long long&v:y)std::cin>>v;
  std::cout<<monge_shortest_path(n+1,[&](int i,int j){
    long long dx=std::abs(a[j-1]-x[i]);
    long long dy=y[i];
    return dx*dx*dx+dy*dy*dy;
  })[n]<<std::endl;
}
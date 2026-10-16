#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/3171"
#include<iostream>
#include "graph/mct.hpp"
#include "math/binary_search.hpp"
int main(){
  int n,m,x;
  std::cin>>n>>m>>x;
  std::vector<std::tuple<int,int,int>>A(n),B(m);
  for(auto&[a,b,p]:A)std::cin>>a>>b>>p;
  for(auto&[u,v,f]:B)std::cin>>u>>v>>f,u--,v--;
  int ans=bin_search<int>(10000000,-1,[&](int y){
    MinCostTension<long long,long long>mct(n*2+2,1000000000);
    int L=n*2,R=n*2+1;
    mct.equal(L,R,y);
    for(int i=0;i<n;i++){
      auto [a,b,p]=A[i];
      mct.less_equal(L,i*2);
      mct.less(i*2,i*2+1);
      mct.less_equal(i*2+1,R);
      if(i+1<n)mct.less_equal(i*2,(i+1)*2);
      mct.add_single(i*2,a);
      mct.add_single(i*2+1,-a);
      mct.add_cost(i*2,i*2+1,a-b,p);
    }
    for(auto [u,v,f]:B){
      if(f==1)mct.less(v*2,u*2+1);
      else mct.less_equal(u*2+1,v*2);
    }
    return -*mct.solve()>=x;
  });
  if(ans==10000000)ans=-1;
  std::cout<<ans<<std::endl;
}
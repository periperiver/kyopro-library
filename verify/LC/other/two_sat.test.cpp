#define PROBLEM "https://judge.yosupo.jp/problem/two_sat"
#include "other/two_sat.hpp"
int main(){
  int n,m;
  std::string gomi;
  std::cin>>gomi>>gomi>>n>>m;
  std::vector<std::pair<int,int>>a(m);
  for(int i=0;i<m;i++){
    int x,y;
    std::cin>>x>>y>>gomi;
    if(x>=0)x--;
    if(y>=0)y--;
    a[i]={x,y};
  }
  auto ans=two_sat(n,a);
  if(ans.empty())std::cout<<"s UNSATISFIABLE\n";
  else{
    std::cout<<"s SATISFIABLE\nv";
    for(int i=0;i<n;i++){
      int x=i+1;
      if(!ans[i])x=-x;
      std::cout<<x<<' ';
    }
    std::cout<<"0\n";
  }
}
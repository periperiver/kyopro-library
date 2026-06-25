#define PROBLEM "https://yukicoder.me/problems/no/382"
#include "graph/maximum_independent_set.hpp"
int main(){
  long long S;
  std::cin>>S;
  auto get=[&](){return S=(S*12345)%1000003;};
  int n=get()%120+2;
  int p=get();
  Graph g(n,false);
  for(int i=0;i<n;i++)for(int j=i+1;j<n;j++){
    if(get()>=p)g.add_edge(i,j);
  }
  g.build();
  auto ans=maximum_independent_set(g);
  std::cout<<ans.size()+1<<std::endl;
  for(int i=0;i<(int)ans.size();i++)std::cout<<ans[i]+1<<" \n"[i+1==(int)ans.size()];
}
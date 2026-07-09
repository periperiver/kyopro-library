#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/problems/2423"
#include "geo/minimum_enclosing_cycle.hpp"
int main(){
  int n,m;
  std::cin>>n>>m;
  std::vector<int>r(n);
  for(int&x:r)std::cin>>x;
  std::vector<int>l(m);
  for(int i=0;i<m;i++){
    int c;
    std::cin>>c;
    std::vector<Point<int>>p(c);
    for(Point<int>&x:p)std::cin>>x;
    l[i]=(minimum_enclosing_cycle_diameter<int,long long>(p)+1)/2;
  }
  std::vector<int>ans(m,-1);
  std::vector<bool>used(n);
  for(int i=0;i<m;i++){
    for(int j=0;j<n;j++)if(!used[j]&&l[i]<=r[j]){
      std::vector<int>ordl(m-i-1),ordr;
      std::iota(ordl.begin(),ordl.end(),i+1);
      for(int k=0;k<n;k++)if(!used[k]&&k!=j)ordr.push_back(k);
      std::sort(ordl.begin(),ordl.end(),[&](int lhs,int rhs){return l[lhs]>l[rhs];});
      std::sort(ordr.begin(),ordr.end(),[&](int lhs,int rhs){return r[lhs]>r[rhs];});
      bool ok=true;
      for(int k=0;k<(int)ordl.size();k++)ok&=l[ordl[k]]<=r[ordr[k]];
      if(ok){
        ans[i]=j;
        used[j]=true;
        break;
      }
    }
    if(ans[i]==-1){
      std::cout<<"NG\n";
      return 0;
    }
  }
  for(int i=0;i<m;i++)std::cout<<ans[i]+1<<'\n';
}
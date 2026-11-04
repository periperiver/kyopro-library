#pragma once
#include<vector>
#include<cassert>
#include<algorithm>
template<typename T>
int matrix_rank(std::vector<std::vector<T>>a){
  if(a.empty())return 0;
  if(a[0].empty())return 0;
  assert(std::all_of(a.begin(),a.end(),[&](const std::vector<T>&x){return x.size()==a[0].size();}));
  int n=a.size(),m=a[0].size();
  if(n<m){
    std::vector<std::vector<T>>na(m,std::vector<T>(n));
    for(int i=0;i<n;i++)for(int j=0;j<m;j++)na[j][i]=a[i][j];
    a=std::move(na);
    std::swap(n,m);
  }
  int pos=0;
  for(int j=0;j<m;j++){
    int idx=-1;
    for(int i=pos;i<n;i++)if(a[i][j].val()){
      idx=i;
      break;
    }
    if(idx==-1)continue;
    std::swap(a[idx],a[pos]);
    T inv=a[pos][j].inv();
    for(int k=j;k<m;k++)a[pos][k]*=inv;
    for(int i=pos+1;i<n;i++)if(a[i][j].val()){
      T coef=-a[i][j];
      for(int k=j;k<m;k++)a[i][k]+=coef*a[pos][k];
    }
    pos++;
  }
  return pos;
}
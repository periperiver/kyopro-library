#pragma once
#include<vector>
#include<cassert>
#include<algorithm>
template<typename T>
void gauss_elimination(std::vector<std::vector<T>>&a){
  int n=a.size();
  if(n==0)return;
  int m=a[0].size();
  assert(std::all_of(a.begin(),a.end(),[&m](const std::vector<T>&b){return (int)b.size()==m;}));
  for(int i=0,j=0;j<m;){
    int pos=i;
    while(pos<n&&a[pos][j].val()==0)pos++;
    if(pos==n){
      j++;
      continue;
    }
    std::swap(a[i],a[pos]);
    T inv=a[i][j].inv();
    for(int k=j;k<m;k++)a[i][k]*=inv;
    for(int k=i+1;k<n;k++){
      T coef=a[k][j];
      for(int l=j;l<m;l++)a[k][l]-=a[i][l]*coef;
    }
    i++;
  }
  while(n>0&&std::all_of(a[n-1].begin(),a[n-1].end(),[](T&x){return x.val()==0;}))n--,a.pop_back();
}
#pragma once
#include<vector>
#include<cassert>
template<typename T>
std::vector<T>dirichlet_kth_root(std::vector<T>a,int k){
  int n=a.size()-1;
  assert(a[0]==T());
  assert(a[1]==T(1));
  std::vector<T>omega(n+1);
  std::vector<int>lpf(n+1,-1);
  for(int i=2;i<=n;i++){
    if(lpf[i]==-1){
      for(int j=1;i*j<=n;j++)if(lpf[i*j]==-1)lpf[i*j]=i;
    }
    omega[i]=omega[i/lpf[i]];
    omega[i]++;
  }
  T tk=k;
  std::vector<T>res(n+1);
  for(int i=1;i<=n;i++){
    if(i==1)res[i]=1;
    else res[i]/=omega[i]*tk;
    for(int j=2;i*j<=n;j++){
      res[i*j]+=res[i]*a[j]*omega[j];
      res[i*j]-=tk*(res[i]*a[j])*omega[i];
    }
  }
  return res;
}
#pragma once
#include<vector>
#include<cassert>
template<typename T>
T pfaffian(std::vector<std::vector<T>>a){
  int n=a.size();
  assert(n%2==0);
  T res=1;
  for(int k=0;k<n;k+=2){
    int p=-1;
    for(int i=k+1;i<n;i++)if(a[k][i].val()){
      p=i;
      break;
    }
    if(p==-1)return 0;
    if(p!=k+1){
      std::swap(a[k+1],a[p]);
      for(int j=0;j<n;j++){
        std::swap(a[j][k+1],a[j][p]);
      }
      res=-res;
    }
    res*=a[k][k+1];
    T inv=a[k][k+1].inv();
    for(int i=k+2;i<n;i++){
      T c=a[k][i]*inv;
      for(int j=k+2;j<n;j++){
        a[i][j]-=c*a[k+1][j];
        a[j][i]=-a[i][j];
      }
    }
  }
  return res;
}
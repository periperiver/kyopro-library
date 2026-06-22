#pragma once
#include<vector>
//行列式の符号無しのやつ
template<typename T>
T permanent(std::vector<std::vector<T>>mat){
  int n=mat.size();
  T res=T();
  for(int i=0;i<(1<<n);i++){
    T now=1;
    for(int j=0;j<n;j++){
      T sum=0;
      for(int k=0;k<n;k++)if(i>>k&1){
        sum+=mat[j][k];
      }
      now*=sum;
    }
    if(__builtin_parity(i))res-=now;
    else res+=now;
  }
  if(n&1)res=-res;
  return res;
}
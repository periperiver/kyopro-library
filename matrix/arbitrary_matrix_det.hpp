#pragma once
#include<vector>
#include<type_traits>
#include "math/euclid.hpp"
#include "other/type.hpp"
template<typename T>
std::enable_if_t<is_modint_v<T>,T>arbitrary_matrix_det(std::vector<std::vector<T>>mat){
  int n=mat.size();
  T res=T::raw(1);
  bool sign=false;
  for(int i=0;i<n;i++){
    if(mat[i][i].val()==0){
      for(int j=i+1;j<n;j++)if(mat[j][i].val()){
        std::swap(mat[i],mat[j]);
        sign^=1;
        break;
      }
    }
    if(mat[i][i].val()==0)return T::raw(0);
    for(int j=i+1;j<n;j++)if(mat[j][i].val()){
      auto coef=euclid(mat[i][i].val(),mat[j][i].val());
      T a=coef[0][0],b=coef[0][1],c=coef[1][0],d=coef[1][1];
      res*=a*d-b*c;
      for(int k=i;k<n;k++){
        T x=mat[i][k],y=mat[j][k];
        mat[i][k]=a*x+b*y;
        mat[j][k]=c*x+d*y;
      }
    }
  }
  for(int i=0;i<n;i++)res*=mat[i][i];
  if(sign)res=-res;
  return res;
}
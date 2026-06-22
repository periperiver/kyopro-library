#pragma once
#include<iostream>
#include<array>
#include<vector>
#include<concepts>
template<typename T,int N,int M=N>
struct ConstantMatrix{
private:
  std::array<std::array<T,M>,N>dat;
public:
  ConstantMatrix(){
    dat.fill([](){std::array<T,M>res;res.fill(T());return res;}());
  }
  static ConstantMatrix E(){
    static_assert(N==M);
    ConstantMatrix res;
    for(int i=0;i<N;i++)res[i][i]=T(1);
    return res;
  }
  inline  ConstantMatrix &operator+=(const ConstantMatrix&rhs){
    for(int i=0;i<N;i++)for(int j=0;j<M;j++)(*this)[i][j]+=rhs[i][j];
    return *this;
  }
  inline ConstantMatrix &operator-=(const ConstantMatrix&rhs){
    for(int i=0;i<N;i++)for(int j=0;j<M;j++)(*this)[i][j]-=rhs[i][j];
    return *this;
  }
  inline ConstantMatrix &operator*=(const ConstantMatrix&rhs){
    static_assert(N==M);
    ConstantMatrix res;
    for(int i=0;i<N;i++)for(int j=0;j<M;j++)for(int k=0;k<M;k++)res[i][k]+=(*this)[i][j]*rhs[j][k];
    (*this)=std::move(res);
    return *this;
  }
  friend ConstantMatrix operator+(const ConstantMatrix&lhs,const ConstantMatrix&rhs){return ConstantMatrix(lhs)+=rhs;}
  friend ConstantMatrix operator-(const ConstantMatrix&lhs,const ConstantMatrix&rhs){return ConstantMatrix(lhs)-=rhs;}
  template<int K>friend ConstantMatrix<T,N,K> operator*(const ConstantMatrix&lhs,const ConstantMatrix<T,M,K>&rhs){
    ConstantMatrix<T,N,K>res;
    for(int i=0;i<N;i++)for(int j=0;j<M;j++)for(int k=0;k<K;k++)res[i][k]+=lhs[i][j]*rhs[j][k];
    return res;
  }
  std::array<T,M> &operator[](int i){return dat[i];}
  const std::array<T,M> &operator[](int i)const{return dat[i];}
  friend bool operator==(const ConstantMatrix&lhs,const ConstantMatrix&rhs){
    for(int i=0;i<N;i++)for(int j=0;j<M;j++)if(lhs[i][j]!=rhs[i][j])return false;
    return true;
  }
  friend bool operator!=(const ConstantMatrix&lhs,const ConstantMatrix&rhs){
    for(int i=0;i<N;i++)for(int j=0;j<M;j++)if(lhs[i][j]!=rhs[i][j])return true;
    return false;
  }
  T det()const{
    static_assert(N==M);
    T res=1;
    ConstantMatrix mat(*this);
    for(int i=0;i<M;i++){
      int idx=-1;
      for(int j=i;j<N;j++)if(mat[j][i]!=T()){
        idx=j;
        break;
      }
      if(idx==-1)return T();
      if(i!=idx)std::swap(mat[i],mat[idx]),res=-res;
      res*=mat[i][i];
      T inv_ii=mat[i][i].inv();
      for(int j=i+1;j<M;j++)mat[i][j]*=inv_ii;
      for(int j=i+1;j<N;j++){
        T coef=-mat[j][i];
        for(int k=i+1;k<M;k++)mat[j][k]+=coef*mat[i][k];
      }
    }
    return res;
  }
  ConstantMatrix inv()const{
    static_assert(N==M);
    ConstantMatrix res=E(),mat=(*this);
    for(int i=0;i<N;i++){
      int id=-1;
      for(int j=i;j<N;j++)if(mat[j][i]!=T()){
        id=j;
        break;
      }
      if(id==-1)return ConstantMatrix();
      if(i!=id){
        std::swap(mat[i],mat[id]);
        std::swap(res[i],res[id]);
      }
      T inv=mat[i][i].inv();
      for(int j=0;j<N;j++)if(i!=j){
        T x=mat[j][i];
        for(int k=0;k<N;k++){
          mat[j][k]-=mat[i][k]*x;
          res[j][k]-=res[i][k]*x;
        }
      }
    }
    return res;
  }
  template<std::integral U>
  ConstantMatrix pow(U k)const{
    static_assert(N==M);
    ConstantMatrix res=ConstantMatrix::E(),a(*this);
    while(k){
      if(k&1)res*=a;
      a*=a;
      k>>=1;
    }
    return res;
  }
  std::vector<std::vector<T>>to_vector()const{
    std::vector<std::vector<T>>res(N,std::vector<T>(M));
    for(int i=0;i<N;i++)for(int j=0;j<M;j++)res[i][j]=dat[i][j];
    return res;
  }
  friend std::istream &operator>>(std::istream&is,ConstantMatrix&rhs){
    for(int i=0;i<N;i++)for(int j=0;j<M;j++)is>>rhs[i][j];
    return is;
  }
  friend std::ostream &operator<<(std::ostream &os,const ConstantMatrix&rhs){
    for(int i=0;i<N;i++){
      for(int j=0;j<M;j++)os<<rhs[i][j]<<" \n"[j+1==M];
    }
    return os;
  }
};
#pragma once
#include<vector>
#include<string.h>
#include<type_traits>
#include<iostream>
template<typename T>
struct BinaryIndexedTreeSimd{
  static_assert(std::is_integral_v<T>);
private:
  static constexpr unsigned int t_size=sizeof(T)*8;
  static constexpr unsigned int buf_size=512/t_size;
  static constexpr unsigned int shift=__builtin_ctz(buf_size);
  struct Pre{
    T mask[buf_size*buf_size];
    constexpr Pre(){
      for(int i=0;i<buf_size;i++){
        for(int j=0;j<buf_size;j++){
          if(i<j)mask[i*buf_size+j]=-1;
          else mask[i*buf_size+j]=0;
        }
      }
    }
  }static constexpr pre{};
  T **dat;
  int n,logsn;
public:
  BinaryIndexedTreeSimd(){}
  explicit BinaryIndexedTreeSimd(int n_):n(n_),logsn(0){
    int z=1;
    while(z<n){
      z<<=shift;
      logsn++;
    }
    dat=new T*[logsn+1];
    z=n;
    for(int i=logsn;i>=0;i--){
      const unsigned int now_size=(z+buf_size-1)&~(buf_size-1);
      dat[i]=new T[now_size];
      memset(dat[i],0,now_size*sizeof(T));
      z=(z+buf_size-1)>>shift;
    }
  }
  inline void add(int i,const T x){
    for(int j=logsn;j>=0;j--){
      const int buf_id=i&~(buf_size-1);
      const int l=i&(buf_size-1);
      for(int k=0;k<buf_size;k++){
        dat[j][buf_id+k]+=x&pre.mask[(l<<shift)+k];
      }
      i>>=shift;
    }
  }
  inline T sum(int l,int r)const{
    T res=0;
    for(int j=logsn;l<r;j--){
      res+=dat[j][r]-dat[j][l];
      l>>=shift,r>>=shift;
    }
    return res;
  }
};
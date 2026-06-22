#pragma once
#include<array>
#include<type_traits>
namespace base64_decode_impl{
constexpr int ctoi(char c){
  if('A'<=c&&c<='Z')return c-'A';
  else if('a'<=c&&c<='z')return c-'a'+26;
  else if('0'<=c&&c<='9')return c-'0'+52;
  else if(c=='+')return 62;
  else if(c=='/')return 63;
  else return -1;
}
template<int N>
constexpr std::array<bool,N>bin_decode(const char*c){
  std::array<bool,N>res;
  for(int i=0,j=0;i<N;i+=6,j++){
    int now=ctoi(c[j]);
    for(int k=0;k<6;k++)if(i+k<N)res[i+k]=now>>k&1;
  }
  return res;
}
template<typename T,int N,int bit_width>
constexpr std::array<T,N>base64_decode(const char*c){
  static_assert(std::is_integral_v<T>);
  std::array<T,N>res;
  std::array<bool,N*bit_width>bin=bin_decode<N*bit_width>(c);
  for(int i=0;i<N;i++){
    res[i]=0;
    for(int j=0;j<bit_width;j++){
      if(bin[i*bit_width+j]){
        res[i]|=T(1)<<j;
      }
    }
  }
  return res;
}
}
using base64_decode_impl::base64_decode;
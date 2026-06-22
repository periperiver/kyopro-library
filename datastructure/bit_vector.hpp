#pragma once
#include<vector>
struct bit_vector{
  using u64=unsigned long long;
  std::vector<u64>dat;
  std::vector<int>sum;
  int zero;
  bit_vector(int n):dat(n/64+1),sum(n/64+1),zero(n){}
  bit_vector(int n,int):dat(n/64+1),sum(n/64+1),zero(n){}
  void set(int i){dat[i>>6]|=u64(1)<<(i&63),zero--;}
  void build(){
    sum[0]=__builtin_popcountll(dat[0]);
    for(int i=1;i<(int)dat.size();i++){
      sum[i]=sum[i-1]+__builtin_popcountll(dat[i]);
    }
  }
  inline int get(int x)const{return (x<64?0:sum[(x>>6)-1])+((x&63)?__builtin_popcountll(dat[x>>6]&(u64(-1)>>(64-(x&63)))):0);}
};
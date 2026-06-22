#pragma once
#include<limits>
#include<type_traits>
#include<cstdint>
#include<array>
template<typename T,int pool_size,int dep>
struct SetXorMin{
  static_assert(dep%4==0);
private:
  struct node{
    int par=-1;
    uint16_t c_mask;
    int ch[16];
  };
  static constexpr int lc=dep/4;
  static std::array<std::array<uint8_t,16>,1<<16>pre;
  static node pool[];
  static int ptr;
public:
  static void add(T x){
    int nd=0;
    for(int i=lc;i--;){
      uint8_t k=x>>i*4&15;
      if(!(pool[nd].c_mask>>k&1)){
        pool[nd].c_mask|=1<<k;
        pool[nd].ch[k]=ptr;
        pool[ptr].par=nd;
        pool[ptr].c_mask=0;
        ptr++;
      }
      nd=pool[nd].ch[k];
    }
  }
  static void erase(T x){
    int nd=0;
    for(int i=lc;i--;){
      uint8_t k=x>>i*4&15;
      if(!(pool[nd].c_mask>>k&1))return;
      nd=pool[nd].ch[k];
    }
    for(int i=0;i<lc;i++){
      if(pool[nd].c_mask)break;
      int nxt=pool[nd].par;
      pool[nxt].c_mask^=1<<(x>>i*4&15);
      nd=nxt;
    }
  }
  static T query(T x){
    if(pool[0].c_mask==0)return ~x;
    T res=0;
    int nd=0;
    for(int i=lc;i--;){
      uint8_t best=pre[pool[nd].c_mask][x>>i*4&15];
      nd=pool[nd].ch[best];
      res|=best<<i*4;
    }
    return res;
  }
  static void clear(){
    ptr=1;
    pool[0].c_mask=0;
  }
};
template<typename T,int pool_size,int dep>std::array<std::array<uint8_t,16>,1<<16>SetXorMin<T,pool_size,dep>::pre=[](){
  std::array<std::array<uint8_t,16>,1<<16>res;
  for(int i=1;i<(1<<16);i++)for(int j=0;j<16;j++){
    for(int k=0;k<16;k++){
      int now=j^k;
      if(i>>now&1){
        res[i][j]=now;
        break;
      }
    }
  }
  return res;
}();
template<typename T,int pool_size,int dep>typename SetXorMin<T,pool_size,dep>::node SetXorMin<T,pool_size,dep>::pool[pool_size];
template<typename T,int pool_size,int dep>int SetXorMin<T,pool_size,dep>::ptr=1;
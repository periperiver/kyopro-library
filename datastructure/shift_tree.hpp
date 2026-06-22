#pragma once
#include<vector>
#include<array>
#include<cassert>
#include "../math/primitive_root.hpp"
#include "../math/modint61.hpp"
#include "../math/util.hpp"
struct ShiftTree{
  int n,log2n,delta;
  std::vector<mint61>dat;
  static std::array<mint61,30>pb;
  ShiftTree(int log2n_):n(1<<log2n_),log2n(log2n_),delta(0),dat(2<<log2n_){}
  inline int skew(int k)const{return delta>>(log2n-k)&1;}
  inline void set(int i,mint61 x){
    if(delta==0){
      dat[i+=n]=x;
      int k=0;
      while(i>>=1)dat[i]=dat[i*2]*pb[k++]+dat[i*2+1];
      return;
    }
    i=n+((i+n-delta)&(n-1));
    dat[i]=x;
    int k=log2n;
    while(i!=1){
      int s=skew(k);
      i=(((i+s)&((1<<k)-1))+(1<<k))>>1;
      int l=(i*2-s)&((1<<k)-1);
      int r=l+1;
      if(r==(1<<k))r=0;
      dat[i]=dat[l+(1<<k)]*pb[log2n-k]+dat[r+(1<<k)];
      k--;
    }
  }
  void shift(int k){
    k&=n-1;
    if(k==0)return;
    int s=lsb(k);
    delta+=k;
    if(delta>=n)delta-=n;
    for(int j=log2n-s-1;j>=0;j--){
      mint61 b=pb[log2n-j-1];
      if(skew(j+1)){
        int i=1<<j;
        dat[i]=dat[(2<<j)-1]*b+dat[i*2];
        i++;
        while(i<(2<<j)){
          dat[i]=dat[i*2-1]*b+dat[i*2];
          i++;
        }
      }
      else{
        for(int i=1<<j;i<(2<<j);i++)dat[i]=dat[i*2]*b+dat[i*2+1];
      }
    }
  }
  static void diff(const ShiftTree&l,const ShiftTree&r,int a,int b,std::vector<int>&buf){
    assert(l.log2n==r.log2n);
    static std::vector<int>st;
    for(a+=l.n,b+=l.n;a<b;a>>=1,b>>=1){
      if(a&1)st.push_back(a++);
      if(b&1)st.push_back(--b);
    }
    while(!st.empty()){
      int x=st.back();st.pop_back();
      int m=msb(x);
      int lidx=x-(l.delta>>(l.log2n-m)),ridx=x-(r.delta>>(r.log2n-m));
      int sz=1<<m;
      if(lidx<sz)lidx+=sz;
      if(ridx<sz)ridx+=sz;
      if(l.dat[lidx]==r.dat[ridx])continue;
      if(m==l.log2n)buf.push_back(x-l.n);
      else{
        st.push_back(x*2);
        st.push_back(x*2+1);
      }
    }
  }
};
std::array<mint61,30>ShiftTree::pb=[](){
  std::array<mint61,30>res;
  res[0]=primitive_root(mint61::mod());
  for(int i=1;i<30;i++)res[i]=res[i-1]*res[i-1];
  return res;
}();
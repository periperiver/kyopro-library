#pragma once
#include<iostream>
#include<vector>
#include<cassert>
using namespace std;
struct BitSet{
private:
  using u64=uint64_t;
  vector<u64>dat;
  int N;
public:
  BitSet():N(0){}
  explicit BitSet(int n):N(n),dat((n+63)/64){}
  explicit BitSet(const string&s):N(s.size()),dat((s.size()+63)/64){
    for(int i=0;i<N;i++)if(s[i]=='1')dat[i>>6]|=u64(1)<<(i&63);
  }
  void set(int i){dat[i>>6]|=u64(1)<<(i&63);}
  void reset(int i){dat[i>>6]&=~(u64(1)<<(i&63));}
  void flip(int i){dat[i>>6]^=u64(1)<<(i&63);}
  int size()const{return N;}
  int popcount()const{
    int res=0;
    for(int i=0;i<dat.size();i++)res+=__builtin_popcountll(dat[i]);
    return res;
  }
  bool operator[](int i)const{return dat[i>>6]>>(i&63)&1;}
  BitSet operator>>(int i)const{
    BitSet res(N);
    if(i>=N)return res;
    int s=i>>6,t=i&63;
    if(t==0){
      for(int j=s;j<dat.size();j++)res.dat[j-s]=dat[j];
    }
    else{
      for(int j=s;j<dat.size()-1;j++){
        res.dat[j-s]=(dat[j]>>t)|(dat[j+1]<<(64-t));
      }
      res.dat[dat.size()-1-s]=dat[dat.size()-1]>>t;
    }
    return res;
  }
  BitSet operator<<(int i)const{
    BitSet res(N);
    if(i>=N)return res;
    int s=i>>6,t=i&63;
    if(t==0){
      for(int j=s;j<dat.size();j++)res.dat[j]=dat[j-s];
    }
    else{
      res.dat[s]=dat[0]<<t;
      for(int j=s+1;j<dat.size();j++){
        res.dat[j]=(dat[j-s]<<t)|(dat[j-s-1]>>(64-t));
      }
      if(N&63)res.dat.back()&=(u64(1)<<(N&63))-1;
    }
    return res;
  }
  BitSet &operator<<=(int i){
    (*this)=(*this)<<i;
    return *this;
  }
  BitSet &operator>>=(int i){
    (*this)=(*this)>>i;
    return *this;
  }
  BitSet &operator^=(const BitSet&rhs){
    assert(N==rhs.N);
    for(int i=0;i<dat.size();i++)dat[i]^=rhs.dat[i];
    return *this;
  }
  BitSet &operator&=(const BitSet&rhs){
    assert(N==rhs.N);
    for(int i=0;i<dat.size();i++)dat[i]&=rhs.dat[i];
    return *this;
  }
  BitSet &operator|=(const BitSet&rhs){
    assert(N==rhs.N);
    for(int i=0;i<dat.size();i++)dat[i]|=rhs.dat[i];
    return *this;
  }
  BitSet operator^(const BitSet&rhs){return BitSet(*this)^=rhs;}
  BitSet operator&(const BitSet&rhs){return BitSet(*this)&=rhs;}
  BitSet operator|(const BitSet&rhs){return BitSet(*this)|=rhs;}
  friend istream &operator>>(istream &is,BitSet&bs){
    char c=is.get();
    while(isspace(c))c=is.get();
    is.unget();
    for(int i=0;i<bs.N;i++){
      char c=is.get();
      if(c=='1')bs.dat[i>>6]|=u64(1)<<(i&63);
    }
    return is;
  }
  friend ostream &operator<<(ostream&os,const BitSet&bs){
    for(int i=0;i<bs.N;i++){
      if(bs.dat[i>>6]>>(i&63)&1)os<<'1';
      else os<<'0';
    }
    return os;
  }
};
#pragma once
#include<iostream>
#include<array>
struct Nimber{
private:
  using u64=unsigned long long;
  u64 v;
  static std::array<std::array<u64,256>,256>small;
  static std::array<std::array<std::array<u64,256>,8>,8>pre;
  static u64 pre_calc(int l,u64 a,u64 b){
    if(l==1)return a*b;
    int mid=l>>1;
    u64 au=a>>mid,av=a&((1ull<<mid)-1),bu=b>>mid,bv=b&((1ull<<mid)-1);
    u64 abu=pre_calc(mid,au,bu);
    u64 c=(abu^pre_calc(mid,au,bv)^pre_calc(mid,av,bu))<<mid;
    u64 d=pre_calc(mid,abu,1ull<<(mid-1));
    u64 e=pre_calc(mid,av,bv);
    return c^d^e;
  }
  static u64 nim_product(int l,u64 a,u64 b){
    if(l<=8)return small[a][b];
    int mid=l>>1;
    u64 au=a>>mid,av=a&((1ull<<mid)-1),bu=b>>mid,bv=b&((1ull<<mid)-1);
    u64 abu=nim_product(mid,au,bu);
    u64 c=(abu^nim_product(mid,au,bv)^nim_product(mid,av,bu))<<mid;
    u64 d=nim_product(mid,abu,1ull<<(mid-1));
    u64 e=nim_product(mid,av,bv);
    return c^d^e;
  }
public:
  Nimber(u64 v_=0):v(v_){}
  Nimber operator-()const{return *this;}
  Nimber operator+(const Nimber&rhs)const{return Nimber(this->v^rhs.v);}
  Nimber &operator+=(const Nimber&rhs){return *this=*this+rhs;}
  Nimber operator-(const Nimber&rhs)const{return Nimber(this->v^rhs.v);}
  Nimber &operator-=(const Nimber&rhs){return *this=*this+rhs;}
  Nimber operator*(const Nimber&rhs)const{
    u64 ret=0;
    for(int i=0;i<8;i++)for(int j=0;j<8;j++){
      ret^=pre[i][j][small[(this->v>>(i*8))&255][(rhs.v>>(j*8))&255]];
    }
    return ret;
  }
  friend bool operator==(const Nimber&lhs,const Nimber&rhs){return lhs.val()==rhs.val();}
  friend bool operator!=(const Nimber&lhs,const Nimber&rhs){return lhs.val()!=rhs.val();}
  friend bool operator<(const Nimber&lhs,const Nimber&rhs){return lhs.val()<rhs.val();}
  friend bool operator>(const Nimber&lhs,const Nimber&rhs){return lhs.val()>rhs.val();}
  friend bool operator<=(const Nimber&lhs,const Nimber&rhs){return lhs.val()<=rhs.val();}
  friend bool operator>=(const Nimber&lhs,const Nimber&rhs){return lhs.val()>=rhs.val();}
  Nimber &operator*=(const Nimber&rhs){return *this=*this*rhs;}
  friend std::istream &operator>>(std::istream &is,Nimber &nim){
    is>>nim.v;
    return is;
  }
  friend std::ostream &operator<<(std::ostream &os,const Nimber &nim){
    os<<nim.v;
    return os;
  }
  inline u64 val()const{return v;}
};
std::array<std::array<unsigned long long,256>,256>Nimber::small=[](){
  std::array<std::array<unsigned long long,256>,256>ret;
  for(int i=0;i<256;i++)for(int j=0;j<256;j++){
    ret[i][j]=Nimber::pre_calc(8,i,j);
  }
  return ret;
}();
std::array<std::array<std::array<unsigned long long,256>,8>,8>Nimber::pre=[](){
  std::array<std::array<std::array<unsigned long long,256>,8>,8>ret;
  for(int i=0;i<8;i++)for(int j=0;j<8;j++){
    for(int k=0;k<256;k++)ret[i][j][k]=Nimber::nim_product(64,Nimber::nim_product(64,1ull<<(8*i),1ull<<(8*j)),k);
  }
  return ret;
}();
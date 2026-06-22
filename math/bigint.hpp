#pragma once
#include<iostream>
#include<vector>
#include "convolution/ntt.hpp"
#include "modint.hpp"
#include "crt3.hpp"
using namespace std;
struct bint{
private:
  static constexpr int ten[10]={1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};
  static constexpr int mod=1000000000;
  bool neg;
  using i128=__int128_t;
  std::vector<int>dat;
  static void add(std::vector<int>&lhs,const std::vector<int>&rhs){
    if(lhs.size()<rhs.size())lhs.resize(rhs.size());
    int u=0;
    for(int i=0;i<rhs.size();i++){
      lhs[i]+=rhs[i]+u;
      if(lhs[i]>=mod){
        lhs[i]-=mod;
        u=1;
      }
      else u=0;
    }
    if(u){
      int pos=rhs.size();
      while(true){
        if(lhs.size()==pos){
          lhs.emplace_back(1);
          break;
        }
        lhs[pos]++;
        if(lhs[pos]==mod)pos++;
        else break;
      }
    }
  }
  static void sub(std::vector<int>&lhs,const std::vector<int>&rhs){
    int u=0;
    for(int i=0;i<rhs.size();i++){
      lhs[i]-=rhs[i]+u;
      if(lhs[i]<0){
        lhs[i]+=mod;
        u=1;
      }
      else u=0;
    }
    if(u){
      int pos=rhs.size();
      while(true){
        lhs[pos]--;
        if(lhs[pos]<0){
          lhs[pos]+=mod;
          pos++;
        }
        else break;
      }
    }
    while(!lhs.empty()&&lhs.back()==0)lhs.pop_back();
  }
  static int comp(const std::vector<int>&lhs,const std::vector<int>&rhs){
    if(lhs.size()!=rhs.size())return lhs.size()>rhs.size()?1:-1;
    for(int i=lhs.size();i--;){
      if(lhs[i]!=rhs[i])return lhs[i]>rhs[i]?1:-1;
    }
    return 0;
  }
  static void mul(std::vector<int>&lhs,const std::vector<int>&rhs){
    int n=lhs.size(),m=rhs.size();
    if(n==0||m==0){
      lhs.clear();
      return;
    }
    if(min(n,m)<200){
      std::vector<i128>a(n+m-1,0);
      if(n<m)for(int i=0;i<n;i++)for(int j=0;j<m;j++)a[i+j]+=(i128)lhs[i]*rhs[j];
      else for(int j=0;j<m;j++)for(int i=0;i<n;i++)a[i+j]+=(i128)lhs[i]*rhs[j];
      lhs.resize(n+m-1);
      i128 u=0;
      for(int i=0;i<n+m-1;i++){
        u+=a[i];
        lhs[i]=u%mod;
        u/=mod;
      }
      while(u){
        lhs.emplace_back(u%mod);
        u/=mod;
      }
      return;
    }
    static constexpr int mod1=167772161;
    static constexpr int mod2=469762049;
    static constexpr int mod3=754974721;
    using mint1=mod_int<mod1>;
    using mint2=mod_int<mod2>;
    using mint3=mod_int<mod3>;
    std::vector<mint1>lhs1(lhs.begin(),lhs.end()),rhs1(rhs.begin(),rhs.end());
    std::vector<mint2>lhs2(lhs.begin(),lhs.end()),rhs2(rhs.begin(),rhs.end());
    std::vector<mint3>lhs3(lhs.begin(),lhs.end()),rhs3(rhs.begin(),rhs.end());
    lhs1=ntt_convolution(lhs1,rhs1);
    lhs2=ntt_convolution(lhs2,rhs2);
    lhs3=ntt_convolution(lhs3,rhs3);
    i128 u=0;
    lhs.resize(n+m-1,0);
    for(int i=0;i<n+m-1;i++){
      u+=crt3<i128,mod1,mod2,mod3>(lhs1[i].val(),lhs2[i].val(),lhs3[i].val());
      lhs[i]=u%mod;
      u/=mod;
    }
    while(u){
      lhs.emplace_back(u%mod);
      u/=mod;
    }
  }
public:
  bint():neg(false){}
  bint(long long n){
    if(n<0)neg=true,n=-n;
    else neg=false;
    while(n){
      dat.emplace_back(n%mod);
      n/=mod;
    }
  }
  bint &operator+=(const bint&rhs){
    if(neg==rhs.neg){
      add(dat,rhs.dat);
    }
    else{
      int c=comp(dat,rhs.dat);
      if(c==0){
        neg=false;
        dat.clear();
      }
      else if(c==1){
        sub(dat,rhs.dat);
      }
      else{
        std::vector<int>temp(rhs.dat);
        sub(temp,dat);
        swap(dat,temp);
        neg^=1;
      }
    }
    return *this;
  }
  bint &operator-=(const bint&rhs){
    if(neg!=rhs.neg){
      add(dat,rhs.dat);
    }
    else{
      int c=comp(dat,rhs.dat);
      if(c==0){
        neg=false;
        dat.clear();
      }
      else if(c==1){
        sub(dat,rhs.dat);
      }
      else{
        std::vector<int>temp(rhs.dat);
        sub(temp,dat);
        swap(dat,temp);
        neg^=1;
      }
    }
    return *this;
  }
  bint &operator*=(const bint&rhs){
    neg^=rhs.neg;
    mul(dat,rhs.dat);
    return *this;
  }
  bint operator+()const{return *this;}
  bint operator-(){
    if(!dat.empty())neg^=1;
    return *this;
  }
  friend bint operator+(const bint&lhs,const bint&rhs){return bint(lhs)+=rhs;}
  friend bint operator-(const bint&lhs,const bint&rhs){return bint(lhs)-=rhs;}
  friend bint operator*(const bint &lhs,const bint&rhs){return bint(lhs)*=rhs;}
  friend bool operator<(const bint&lhs,const bint&rhs){
    if(lhs.neg!=rhs.neg)return lhs.neg;
    int c=comp(lhs.dat,rhs.dat);
    if(c==0)return false;
    return (c==1)==lhs.neg;
  }
  friend bool operator>(const bint&lhs,const bint&rhs){
    if(lhs.neg!=rhs.neg)return !lhs.neg;
    int c=comp(lhs.dat,rhs.dat);
    if(c==0)return false;
    return (c==1)!=lhs.neg;
  }
  friend bool operator<=(const bint&lhs,const bint&rhs){return !(lhs>rhs);}
  friend bool operator>=(const bint&lhs,const bint&rhs){return !(lhs<rhs);}
  friend bool operator==(const bint&lhs,const bint&rhs){return lhs.neg==rhs.neg&&!comp(lhs.dat,rhs.dat);}
  friend bool operator!=(const bint&lhs,const bint&rhs){return lhs.neg!=rhs.neg||comp(lhs.dat,rhs.dat);}
  friend std::istream &operator>>(std::istream &is,bint&a){
    std::string s;
    is>>s;
    if(s[0]=='-')a.neg=true,s.erase(s.begin());
    int sz=s.size();
    a.dat.resize((sz+8)/9);
    for(int i=0;i<a.dat.size();i++){
      int l=sz-(i+1)*9;
      int r=sz-i*9;
      if(l<0)l=0;
      a.dat[i]=stoi(s.substr(l,r-l));
    }
    while(!a.dat.empty()&&a.dat.back()==0)a.dat.pop_back();
    if(a.dat.empty())a.neg=false;
    return is;
  }
  friend std::ostream &operator<<(std::ostream &os,const bint&a){
    os<<a.to_string();
    return os;
  }
  std::string to_string()const{
    if(dat.empty())return "0";
    std::string res;
    if(neg)res+='-';
    res+=std::to_string(dat.back());
    for(int i=std::ssize(dat)-1;i--;){
      std::string s=std::to_string(dat[i]);
      res+=std::string(9-s.size(),'0');
      res+=s;
    }
    return res;
  }
  int digit_sum10()const{
    int res=0;
    for(int x:dat){
      while(x){
        res+=x%10;
        x/=10;
      }
    }
    return res;
  }
  int digit()const{
    if(dat.empty())return 0;
    int res=9*((int)dat.size()-1);
    int x=dat.back();
    while(x){
      res++;
      x/=10;
    }
    return res;
  }
};
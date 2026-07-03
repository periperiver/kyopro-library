#pragma once
#include "../math/arbitrary_modint.hpp"
#include "../math/primitive_root.hpp"
template<int id>
struct Convolution64{
  using mint=arbitrary_montgomery_modint64<long long,id>;
  int rank2;
  std::vector<mint>root,invroot;
  std::vector<mint>rate2,invrate2;
  std::vector<mint>rate3,invrate3;
  using u128=__uint128_t;
  Convolution64(){
    rank2=lsb(mint::mod()-1);
    mint g=primitive_root(mint::mod());
    root.resize(rank2+1),invroot.resize(rank2+1);
    root[rank2]=g.pow((mint::mod()-1)>>rank2);
    invroot[rank2]=root[rank2].inv();
    for(int i=rank2-1;i>=0;i--){
      root[i]=root[i+1]*root[i+1];
      invroot[i]=invroot[i+1]*invroot[i+1];
    }
    mint prod=mint::one(),invprod=mint::one();
    rate2.resize(std::max(0,rank2-1)),invrate2.resize(std::max(0,rank2-1));
    for(int i=0;i<rank2-1;i++){
      rate2[i]=root[i+2]*prod;
      invrate2[i]=invroot[i+2]*invprod;
      prod*=invroot[i+2];
      invprod*=root[i+2];
    }
    rate3.resize(std::max(0,rank2-2)),invrate3.resize(std::max(0,rank2-2));
    prod=mint::one(),invprod=mint::one();
    for(int i=0;i<rank2-2;i++){
      rate3[i]=root[i+3]*prod;
      invrate3[i]=invroot[i+3]*invprod;
      prod*=invroot[i+3];
      invprod*=root[i+3];
    }
  }
  void dft(std::vector<mint>&a)const{
    int n=a.size();
    int h=lsb(n);
    int len=0;
    while(len<h){
      if(h-len==1){
        mint rot=mint::one();
        for(int s=0;s<(1<<len);s++){
          int of=s*2;
          mint u=a[of],v=a[of+1]*rot;
          a[of]=u+v;
          a[of+1]=u-v;
          rot*=rate2[lsb(~(unsigned int)s)];
        }
        len++;
      }
      else{
        int p=1<<(h-len-2);
        mint rot=mint::one(),imag=root[2];
        for(int s=0;s<(1<<len);s++){
          mint rot2=rot*rot,rot3=rot*rot2;
          int of=s<<(h-len);
          for(int i=0;i<p;i++){
            mint a0=a[i+of],a1=a[i+of+p]*rot,a2=a[i+of+p*2]*rot2,a3=a[i+of+p*3]*rot3;
            mint m=(a1-a3)*imag;
            a[i+of]=a0+a1+a2+a3;
            a[i+of+p]=a0-a1+a2-a3;
            a[i+of+p*2]=a0-a2+m;
            a[i+of+p*3]=a0-a2-m;
          }
          rot*=rate3[lsb(~(unsigned int)s)];
        }
        len+=2;
      }
    }
  }
  void idft(std::vector<mint>&a)const{
    int n=a.size();
    int h=lsb(n);
    int len=h;
    while(len){
      if(len==1){
        int p=1<<(h-1);
        for(int i=0;i<p;i++){
          mint u=a[i],v=a[i+p];
          a[i]=u+v;
          a[i+p]=u-v;
        }
        len--;
      }
      else{
        int p=1<<(h-len);
        mint rot=mint::one(),imag=invroot[2];
        for(int s=0;s<(1<<(len-2));s++){
          mint rot2=rot*rot,rot3=rot*rot2;
          int of=s<<(h-len+2);
          for(int i=0;i<p;i++){
            mint a0=a[i+of],a1=a[i+of+p],a2=a[i+of+p*2],a3=a[i+of+p*3];
            mint k=(a2-a3)*imag;
            a[i+of]=a0+a1+a2+a3;
            a[i+of+p]=(a0-a1+k)*rot;
            a[i+of+p*2]=(a0+a1-a2-a3)*rot2;
            a[i+of+p*3]=(a0-a1-k)*rot3;
          }
          rot*=invrate3[lsb(~(unsigned int)s)];
        }
        len-=2;
      }
    }
  }
  std::vector<mint> operator()(std::vector<mint>a,std::vector<mint>b)const{
    int n=a.size(),m=b.size(),s=n+m-1;
    int z=ceil_pow2(s);
    a.resize(z),b.resize(z);
    dft(a),dft(b);
    for(int i=0;i<z;i++)a[i]*=b[i];
    idft(a);
    mint inv=mint(z).inv();
    for(int i=0;i<s;i++)a[i]*=inv;
    a.resize(s);
    return a;
  }
};
#pragma once
#include<vector>
#include<cmath>
#include "../math/quotients.hpp"
#include "ntt.hpp"
#include "../other/type.hpp"
#include "../math/range_factorize.hpp"
template<typename T>
std::vector<T>dirichlet_ntt_convolution(const std::vector<T>&f,const std::vector<T>&g,const Quotients&quo){
  static_assert(is_static_modint_v<T>);
  static constexpr int ntt_max=1<<lsb(T::mod()-1);
  assert((int)f.size()==quo.size());
  assert((int)g.size()==quo.size());
  std::vector<T>fsmall(f.begin(),f.begin()+quo.s),gsmall(g.begin(),g.begin()+quo.s);
  std::vector<T>res(quo.size());
  for(int i=quo.s-1;i>=1;i--){
    fsmall[i]-=fsmall[i-1];
    gsmall[i]-=gsmall[i-1];
  }
  int b=quo.s;
  while(true){
    int deg=(int)std::ceil(b*std::log(quo.s))*2;
    if(deg+1<=ntt_max)break;
    b--;
  }
  int deg=(int)std::ceil(b*std::log(quo.s));
  std::vector<T>f2(deg+1),g2(deg+1);
  for(int i=1;i<=quo.s;i++){
    int idx=(int)std::ceil(b*std::log(i));
    f2[idx]+=fsmall[i-1];
    g2[idx]+=gsmall[i-1];
  }
  f2=ntt_convolution(f2,g2);
  int idx=0;
  double logn=std::log(quo.n);
  int s=quo.s-(quo.s==quo.n/quo.s);
  while(idx<(int)f2.size()&&idx<=b*(logn-std::log(s+1)))idx++;
  for(int i=s;i>=1;i--){
    T now=T();
    while(idx<(int)f2.size()&&idx<=b*(logn-std::log(i)))now+=f2[idx++];
    res[res.size()-i]+=now;
  }
  {
    long long lhs=quo.n*std::exp((double)-2/b);
    lhs++;
    csr_array<long long>fac=range_factorize(lhs,quo.n+1);
    auto dfs=[&](auto self,int i,int j,long long iprod,long long jprod)->void {
      if(j==fac[i].size()){
        if(iprod*jprod<=i)return;
        int wa=(int)std::ceil(b*std::log(iprod))+(int)std::ceil(b*std::log(jprod));
        T val=fsmall[iprod-1]*gsmall[jprod-1];
        int pre=std::ceil(std::exp(logn-(double)wa/b))+1;
        pre=std::clamp(pre,0,s+1);
        while(pre>0&&wa>b*(logn-std::log(pre)))pre--;
        if(pre!=s+1&&pre!=0)res[res.size()-pre]-=val;
        if(iprod*jprod>quo.s)res[quo.get_key(iprod*jprod)]+=val;
      }
      else{
        int nj=j;
        while(nj<fac[i].size()&&fac[i][j]==fac[i][nj])nj++;
        long long jprod2=jprod;
        for(int k=0;k<=nj-j;k++){
          jprod=jprod2;
          for(int l=0;k+l<=nj-j;l++){
            self(self,i,nj,iprod,jprod);
            jprod*=fac[i][j];
            if(jprod>quo.s)break;
          }
          iprod*=fac[i][j];
          if(iprod>quo.s)break;
        }
      }
    };
    for(int i=0;i<=quo.n-lhs;i++)dfs(dfs,i,0,1,1);
    // for(int i=1;i<=quo.s;i++)for(int j=1;j<=quo.s;j++){
    //   if(quo.n/(i*j)!=lhs/(i*j)){
    //     int wa=(int)std::ceil(b*std::log(i))+(int)std::ceil(b*std::log(j));
    //     T val=fsmall[i-1]*gsmall[j-1];
    //     int pre=s+1;
    //     while(pre>0&&wa>b*(logn-std::log(pre)))pre--;
    //     if(pre!=s+1&&pre!=0)res[res.size()-pre]-=val;
    //     if((long long)i*j>quo.s)res[quo.get_key((long long)i*j)]+=val;
    //   }
    // }
  }
  for(int i=1;i<=quo.s;i++)for(int j=1;i*j<=quo.s;j++){
    res[i*j-1]+=fsmall[i-1]*gsmall[j-1];
    res[quo.size()-i]+=(f[quo.size()-i*j]-f[std::max<int>(quo.s-1,quo.size()-(i+1)*j)])*gsmall[j-1];
    res[quo.size()-j]+=(g[quo.size()-i*j]-g[std::max<int>(quo.s-1,quo.size()-i*(j+1))])*fsmall[i-1];
  }
  for(int i=1;i<quo.size();i++)res[i]+=res[i-1];
  return res;
}
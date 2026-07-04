#pragma once
#include "multidimensional_cyclic_convolution.hpp"
#include "convolution64.hpp"
#include "../other/safe_mul.hpp"
#include "../math/carmichael.hpp"
#include<algorithm>
namespace multiplicative_convolution_modn_impl{
int ipow(int x,int y){
  int res=1;
  while(y--)res*=x;
  return res;
}
struct MultiplicativeConvolutionSolver{
  int n;
  std::vector<int>lpf;
  std::vector<std::pair<int,int>>n_factorized;
  std::vector<int>roots;
  std::vector<int>divisors;
  MultiplicativeConvolutionSolver(int n_):n(n_),lpf(n+1,-1){
    init_lpf();
    n_factorized=factorize(n);
    divisors=enumerate_divisors(n_factorized);
    for(auto [p,e]:n_factorized){
      if(p==2){
        if(e>=2){
          roots.push_back(-5);
        }
        roots.push_back(5);
      }
      else{
        roots.push_back(primitive_root_pe(p,e));
      }
    }
    if(!n_factorized.empty()&&n_factorized[0].first==2&&n_factorized[0].second>=2)n_factorized.insert(n_factorized.begin(),std::make_pair(-1,-1));
    std::reverse(n_factorized.begin(),n_factorized.end());
    std::reverse(roots.begin(),roots.end());
  }
  void init_lpf(){
    for(int i=2;i<=n;i++)if(lpf[i]==-1){
      for(int j=1;i*j<=n;j++)if(lpf[i*j]==-1)lpf[i*j]=i;
    }
  }
  std::vector<std::pair<int,int>>factorize(int x)const{
    std::vector<std::pair<int,int>>res;
    while(x>1){
      int p=lpf[x];
      int&e=res.emplace_back(p,0).second;
      do x/=p,e++; while(x%p==0);
    }
    return res;
  }
  int primitive_root_pe(int p,int e)const{
    int res=2;
    std::vector<std::pair<int,int>>f=factorize(p-1);
    while(true){
      bool ok=true;
      for(auto [p2,e2]:f){
        if(pow_mod(res,(p-1)/p2,p)==1){
          ok=false;
          break;
        }
      }
      if(ok&&(e==1||pow_mod(res,p-1,p*p)!=1))break;
      res++;
    }
    return res;
  }
  std::vector<int>enumerate_divisors(const std::vector<std::pair<int,int>>&factorized){
    std::vector<int>res{1};
    for(const auto&[p,e]:factorized){
      std::vector<int>nres;
      for(int x:res){
        nres.push_back(x);
        for(int i=0;i<e;i++){
          x*=p;
          nres.push_back(x);
        }
      }
      res=std::move(nres);
    }
    std::sort(res.begin(),res.end());
    return res;
  }
  int d_idx(int x)const{
    int res=std::lower_bound(divisors.begin(),divisors.end(),x)-divisors.begin();
    assert(divisors[res]==x);
    return res;
  }
  template<typename T,typename Func>
  std::vector<T>solve(const std::vector<T>&a,const std::vector<T>&b,const Func&conv)const{
    using T2=safe_mul_t<typename T::value_type>;
    std::vector<std::vector<T>>f(divisors.size()),g(divisors.size()),h(divisors.size());
    std::vector<std::vector<int>>ns(divisors.size());
    T r=primitive_root(T::mod());
    auto shape=[&](int x)->std::vector<int> {
      x=n/x;
      std::vector<int>res(n_factorized.size());
      for(int i=0;i<std::ssize(n_factorized);i++){
        int e=0;
        while(x%n_factorized[i].first==0)x/=n_factorized[i].first,e++;
        if(n_factorized[i].first==2){
          res[i]=1<<std::max(0,e-2);
          if(i+1<std::ssize(n_factorized)&&roots[i+1]==-5){
            res[i+1]=e>=2?2:1;
            i++;
          }
        }
        else{
          if(e==0)res[i]=1;
          else res[i]=(n_factorized[i].first-1)*ipow(n_factorized[i].first,e-1);
        }
      }
      return res;
    };
    auto get_mods=[&](int x)->std::vector<int> {
      x=n/x;
      std::vector<int>res(n_factorized.size(),1);
      for(int i=0;i<std::ssize(n_factorized);i++)if(n_factorized[i].first!=-1){
        while(x%n_factorized[i].first==0){
          x/=n_factorized[i].first;
          res[i]*=n_factorized[i].first;
        }
      }
      return res;
    };
    for(int i=0;i<std::ssize(divisors);i++){
      std::vector<int>ns_i=shape(divisors[i]);
      std::vector<int>mods=get_mods(divisors[i]);
      std::vector<int>mods_sprod(mods);
      mods_sprod.push_back(1);
      std::vector<int>invs(mods.size());
      for(int j=mods.size();j--;){
        invs[j]=inv_mod(mods_sprod[j+1],mods[j]);
        mods_sprod[j]*=mods_sprod[j+1];
      }
      int sz=1;
      for(int x:ns_i)sz*=x;
      std::vector<T>f_i(sz),g_i(sz);
      auto dfs=[&](auto self,int pos,int idx,int to)->void {
        if(pos==0){
          f_i[to]=a[idx*divisors[i]];
          g_i[to]=b[idx*divisors[i]];
        }
        else if(roots[pos-1]==-5&&ns_i[pos-1]==2){
          int now=1;
          for(int j=0;j<ns_i[pos-2];j++){
            int x=(long long)(now-idx)*invs[pos-2]%mods[pos-2];
            int nidx=idx+x*mods_sprod[pos-1];
            if(nidx<0)nidx+=mods_sprod[pos-2];
            self(self,pos-2,nidx,to*2*ns_i[pos-2]+j);
            self(self,pos-2,mods_sprod[pos-2]-nidx,(to*2+1)*ns_i[pos-2]+j);
            now=now*5%mods[pos-2];
          }
        }
        else{
          int now=1;
          for(int j=0;j<ns_i[pos-1];j++){
            int x=(long long)(now-idx)*invs[pos-1]%mods[pos-1];
            int nidx=idx+x*mods_sprod[pos];
            if(nidx<0)nidx+=mods_sprod[pos-1];
            self(self,pos-1,nidx,to*ns_i[pos-1]+j);
            now=now*roots[pos-1]%mods[pos-1];
          }
        }
      };
      dfs(dfs,n_factorized.size(),0,0);
      multidimensional_dft(ns_i,f_i,r,conv);
      multidimensional_dft(ns_i,g_i,r,conv);
      f[i]=std::move(f_i);
      g[i]=std::move(g_i);
      ns[i]=std::move(ns_i);
      h[i].resize(sz);
    }
    auto discrete_log=[&](int x,int did)->std::vector<int> {
      const std::vector<int>&ns_i=ns[did];
      std::vector<int>res(n_factorized.size());
      for(int i=0;i<std::ssize(res);i++){
        int mod=1;
        for(int j=n/divisors[did];j%n_factorized[i].first==0;j/=n_factorized[i].first,mod*=n_factorized[i].first);
        int v=x%mod;
        if(i+1<std::ssize(res)&&roots[i+1]==-5){
          if(v==1){
            i++;
          }
          else{
            int now=roots[i];
            res[i]=1;
            while(true){
              if(now==v)break;
              if(now==1){
                res[i]=-1;
                break;
              }
              res[i]++;
              now=((long long)now*roots[i])%mod;
            }
            if(res[i]==-1){
              now=1;
              res[i]=0;
              while(true){
                if(now+v==mod)break;
                res[i]++;
                now=((long long)now*roots[i])%mod;
              }
              res[i+1]=1;
            }
            i++;
          }
        }
        else{
          int now=1;
          while(true){
            if(now==v)break;
            now=((long long)now*roots[i])%mod;
            res[i]++;
          }
        }
      }
      return res;
    };
    for(int i=0;i<std::ssize(divisors);i++)for(int j=0;j<std::ssize(divisors);j++){
      int to=d_idx(std::gcd<long long>((long long)divisors[i]*divisors[j],n));
      int over=(long long)divisors[i]*divisors[j]/divisors[to];
      std::vector<int>shift=discrete_log(over,to);
      const std::vector<int>&f_s=ns[i];
      const std::vector<int>&g_s=ns[j];
      const std::vector<int>&to_s=ns[to];
      auto dfs=[&](auto self,int pos,int f_idx,int g_idx,int to_idx,long long e_sum)->void {
        if(pos==0){
          h[to][to_idx]+=f[i][f_idx]*g[j][g_idx]*r.pow(e_sum);
        }
        else{
          const int f_step=f_s[pos-1]/to_s[pos-1];
          const int g_step=g_s[pos-1]/to_s[pos-1];
          for(int k=0;k<to_s[pos-1];k++){
            self(self,pos-1,f_idx*f_s[pos-1]+k*f_step,g_idx*g_s[pos-1]+k*g_step,to_idx*to_s[pos-1]+k,(e_sum+(T2)k*(T::mod()-1)/to_s[pos-1]*shift[pos-1])%(T::mod()-1));
          }
        }
      };
      dfs(dfs,n_factorized.size(),0,0,0,0);
    }
    std::vector<T>res(n);
    for(int i=0;i<std::ssize(divisors);i++){
      multidimensional_dft(ns[i],h[i],r.inv(),conv);
      int l=lsb(n/divisors[i]);
      const std::vector<int>&ns_i=ns[i];
      std::vector<int>mods=get_mods(divisors[i]);
      std::vector<int>mods_sprod(mods);
      mods_sprod.push_back(1);
      std::vector<int>invs(mods.size());
      for(int j=mods.size();j--;){
        invs[j]=inv_mod(mods_sprod[j+1],mods[j]);
        mods_sprod[j]*=mods_sprod[j+1];
      }
      int sz=1;
      for(int x:ns[i])sz*=x;
      T szinv=T(sz).inv();
      auto dfs=[&](auto self,int pos,int idx,int from)->void {
        if(pos==0){
          res[idx*divisors[i]]+=h[i][from]*szinv;
        }
        else if(roots[pos-1]==-5&&ns_i[pos-1]==2){
          int now=1;
          for(int j=0;j<ns_i[pos-2];j++){
            int x=(long long)(now-idx)*invs[pos-2]%mods[pos-2];
            int nidx=idx+x*mods_sprod[pos-1];
            if(nidx<0)nidx+=mods_sprod[pos-2];
            self(self,pos-2,nidx,from*2*ns[i][pos-2]+j);
            self(self,pos-2,mods_sprod[pos-2]-nidx,(from*2+1)*ns[i][pos-2]+j);
            now=now*5%mods[pos-2];
          }
        }
        else{
          int now=1;
          for(int j=0;j<ns[i][pos-1];j++){
            int x=(long long)(now-idx)*invs[pos-1]%mods[pos-1];
            int nidx=idx+x*mods_sprod[pos];
            if(nidx<0)nidx+=mods_sprod[pos-1];
            self(self,pos-1,nidx,from*ns[i][pos-1]+j);
            now=now*roots[pos-1]%mods[pos-1];
          }
        }
      };
      dfs(dfs,n_factorized.size(),0,0);
    }
    return res;
  }
};
template<typename T>
std::vector<T>multiplicative_convolution_modn(std::vector<T>a,std::vector<T>b){
  assert(a.size()==b.size());
  int n=a.size();
  int c=carmichael(n);
  MultiplicativeConvolutionSolver solver(n);
  if((T::mod()-1)%c==0){
    if constexpr(is_static_modint_v<T>){
      if((1<<lsb(T::mod()-1))>=n*2){
        return solver.solve(a,b,ntt_convolution<T>);
      }
    }
  }
  unsigned long long k=(long long)c*ceil_pow2(n)*4;
  unsigned long long z=1000000000000000000/k;
  std::vector<unsigned long long>mods;
  while(std::ssize(mods)<2){
    if(isprime(k*z+1))mods.push_back(k*z+1);
    z++;
  }
  using mint1=arbitrary_modint<long long,20250912>;
  using mint2=arbitrary_modint<long long,20250913>;
  mint1::set_mod(mods[0]);
  mint2::set_mod(mods[1]);
  std::vector<mint1>a1(n),b1(n);
  std::vector<mint2>a2(n),b2(n);
  for(int i=0;i<n;i++){
    a1[i]=a[i].val(),b1[i]=b[i].val();
    a2[i]=a[i].val(),b2[i]=b[i].val();
  }
  a1=solver.solve(a1,b1,Convolution64<20250912>());
  a2=solver.solve(a2,b2,Convolution64<20250913>());
  long long inv=inv_mod<long long>(mods[0],mods[1]).second;
  T mod1t=mint1::mod();
  for(int i=0;i<n;i++){
    long long r1=a1[i].val(),r2=a2[i].val();
    long long m1=mint1::mod(),m2=mint2::mod();
    long long x=(r2-r1)%m2;
    if(x<0)x+=m2;
    x=__int128_t(x)*inv%m2;
    a[i]=r1+mod1t*x;
  }
  return a;
}
}
using multiplicative_convolution_modn_impl::multiplicative_convolution_modn;
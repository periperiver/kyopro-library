#pragma once
#include<vector>
#include<array>
#include<cassert>
#include<cmath>
#include<array>
#include<algorithm>
#include "../datastructure/csr_array.hpp"
#include "prime_sieve.hpp"
#include "quotients.hpp"
#include "kth_root.hpp"
#include "util.hpp"
#include "../other/type.hpp"
#include<iostream>
struct MultiplicativeSum{
  static constexpr int be=6;
  int nb;
  Quotients quo;
  std::vector<int>p,lpf;
  csr_array<std::pair<int,int>>powerful;
  explicit MultiplicativeSum(long long n):nb(kth_root(n,be)),quo(n),p(prime_sieve(quo.s)),lpf(quo.s+1){
    for(int x:p)lpf[x]=x;
    lpf[1]=quo.s+1;
    for(int i=2;i<=quo.s;i++){
      for(int j:p){
        if(j<=lpf[i]&&i*j<=quo.s)lpf[i*j]=j;
        else break;
      }
    }
    int sz=kth_root(n,3);
    std::vector<bool>sieve(sz+1,true);
    for(int i=2;i*i<=sz;i++){
      int ii=i*i;
      for(int j=1;ii*j<=sz;j++)sieve[ii*j]=false;
    }
    std::vector<std::pair<int,std::pair<int,int>>>csr_init;
    int ptr=0;
    for(int i=1;i<=sz;i++)if(sieve[i]&&lpf[i]>nb){
      long long i3=(long long)i*i*i;
      for(int j=1;i3*j*j<=n;j++)if(lpf[j]>nb){
        int x=i,y=j;
        while(x>1){
          int p=lpf[x];
          int e=0;
          do x/=p,e+=3;while(x%p==0);
          while(y%p==0)y/=p,e+=2;
          csr_init.emplace_back(ptr,std::make_pair(p,e));
        }
        while(y>1){
          int p=lpf[y];
          int e=0;
          do y/=p,e+=2;while(y%p==0);
          csr_init.emplace_back(ptr,std::make_pair(p,e));
        }
        ptr++;
      }
    }
    powerful=csr_array<std::pair<int,int>>(ptr,csr_init);
  }
  std::vector<unsigned long long>count_prime()const{
    std::vector<unsigned long long>dp(quo.size());
    for(int i=0;i<(int)dp.size();i++)dp[i]=quo[i]-1;
    for(int x:p){
      unsigned long long kx=(unsigned long long)x*x;
      for(int y=(int)dp.size()-1;y>=0;y--){
        if(kx<=quo[y]){
          unsigned long long key=(double)quo[y]/(double)x;
          dp[y]-=dp[quo.get_key(key)]-dp[x-2];
        }
        else break;
      }
    }
    return dp;
  }
  template<int mod>
  std::vector<std::array<unsigned long long,mod>>count_primd_mod()const{
    static_assert(1<=mod);
    std::vector<std::array<unsigned long long,mod>>dp(quo.size());
    for(int i=0;i<(int)dp.size();i++){
      dp[i][0]=quo[i]/mod;
      for(int j=1;j<mod;j++)dp[i][j]=(quo[i]-j+mod)/mod;
      dp[i][1%mod]--;
    }
    auto sub=[](std::array<unsigned long long,mod>&lhs,const std::array<unsigned long long,mod>&rhs)->void {
      for(int i=0;i<mod;i++)lhs[i]-=rhs[i];
    };
    auto mul=[](std::array<unsigned long long,mod>&lhs,int x)->void {
      std::array<unsigned long long,mod>a;
      a.fill(0);
      for(int i=0;i<mod;i++)a[(i*x)%mod]+=lhs[i];
      lhs=std::move(a);
    };
    for(int x:p){
      unsigned long long kx=(unsigned long long)x*x;
      for(int y=(int)dp.size()-1;y>=0;y--){
        if(kx<=quo[y]){
          unsigned long long key=(double)quo[y]/(double)x;
          std::array<unsigned long long,mod>a(dp[quo.get_key(key)]);
          sub(a,dp[x-2]);
          mul(a,x);
          sub(dp[y],a);
        }
        else break;
      }
    }
    return dp;
  }
  template<typename T,typename Func1,typename Func2>
  std::vector<T>calc(const Func1&f,const Func2&fsum){
    static_assert(is_modint_v<T>);
    std::vector<T>invs(std::max(be,msb(quo.n)+1)),finvs(std::max(be,msb(quo.n)+1));
    invs[0]=T();
    finvs[0]=T::raw(1);
    for(int i=1;i<(int)invs.size();i++)finvs[i]=(invs[i]=T::raw(i).inv())*finvs[i-1];
    std::vector<T>dp(quo.size());
    for(int i=0;i<(int)dp.size();i++)dp[i]=fsum(quo[i])-T::raw(1);
    std::vector<T>smalls(nb+1);
    for(int x:p){
      if(x>nb)break;
      int x2=x*x;
      T dx=f(x);
      smalls[x]=dx;
      for(int j=quo.size()-1;quo[j]>=x2;j--){
        dp[j]-=(dp[quo.get_key(quo[j]/x)]-dp[x-2])*dx;
      }
    }
    for(int i=1;i<=nb;i++){
      smalls[i]+=smalls[i-1];
      dp[i-1]=T::raw(1);
    }
    for(int i=nb;i<quo.size();i++)dp[i]=dp[i]-smalls[nb]+T::raw(1);
    std::vector<std::vector<T>>ply(quo.s+1);
    for(int x:p)if(x>nb){
      std::vector<T>fp{1},gp{1};
      int e=1;
      long long now=x;
      T fp1=f(x);
      T fp1pow=fp1;
      while(true){
        fp.emplace_back(f(now));
        gp.emplace_back(fp1pow*finvs[e]);
        e++;
        fp1pow*=fp1;
        if(now>quo.n/x)break;
        now*=x;
      }
      inplace_div(gp,fp);
      ply[x]=std::move(gp);
    }
    std::vector<T>h(quo.size());
    for(int i=0;i<powerful.size();i++){
      long long now=1;
      T val=T::raw(1);
      for(auto [p,e]:powerful[i]){
        for(int j=0;j<e;j++)now*=p;
        val*=ply[p][e];
      }
      h[quo.get_key(now)]+=val;
    }
    for(int i=1;i<quo.size();i++)h[i]+=h[i-1];
    dp=dirichlet_convolution_sparse(dp,h);
    for(T&x:dp)x=T::raw(1)-x;
    std::vector<T>res(dp);
    for(T&x:res)x=-x;
    std::vector<T>powdp(dp);
    for(int i=2;i<be;i++){
      powdp=dirichlet_convolution_sparse(powdp,dp);
      for(int j=0;j<quo.size();j++)res[j]-=powdp[j]*invs[i];
    }
    for(int i=1;i<=nb;i++)res[i-1]=smalls[i];
    for(int i=nb;i<quo.size();i++)res[i]+=smalls[nb];
    return res;
  }
  template<typename T>
  void inplace_mul(std::vector<T>&a,const std::vector<T>&b)const{
    for(int i=a.size()-1;i>=0;i--){
      a[i]*=b[0];
      for(int j=0;j<i;j++)a[i]+=a[j]*b[i-j];
    }
  }
  template<typename T>
  void inplace_div(std::vector<T>&a,const std::vector<T>&b)const{
    int n=a.size();
    T invb=b[0].inv();
    for(int i=0;i<n;i++){
      for(int j=1;j<=i;j++)a[i]-=a[i-j]*b[j];
      a[i]*=invb;
    }
  }
  template<typename U,double c=3.0>
  std::vector<U>dirichlet_convolution_sparse(const std::vector<U>&f,const std::vector<U>&g){
    static std::vector<std::pair<int,U>>fbuf,gbuf;
    fbuf.clear(),gbuf.clear();
    int k=kth_root(quo.n,3);
    int k2=std::max<int>(kth_root(quo.n,2),k*k*c);
    int b=std::prev(std::upper_bound(quo.dat.begin(),quo.dat.end(),k2))-quo.dat.begin();
    if(f[0]!=U(0))fbuf.emplace_back(1,f[0]);
    if(g[0]!=U(0))gbuf.emplace_back(1,g[0]);
    for(int i=1;i<=b;i++){
      if(f[i]!=f[i-1])fbuf.emplace_back(quo[i],f[i]-f[i-1]);
      if(g[i]!=g[i-1])gbuf.emplace_back(quo[i],g[i]-g[i-1]);
    }
    std::vector<U>res(quo.size());
    for(auto [i,fv]:fbuf)for(auto [j,gv]:gbuf){
      int to=quo.get_key((long long)i*j);
      if(to>b)break;
      res[to]+=fv*gv;
    }
    for(int i=1;i<=b;i++)res[i]+=res[i-1];
    for(int i=b+1;i<(int)res.size();i++){
      int s=std::sqrt(quo[i]);
      for(auto [j,v]:fbuf){
        if(j>s)break;
        int key=quo.get_key((double)quo[i]/(double)j);
        res[i]+=v*g[key];
      }
      for(auto [j,v]:gbuf){
        if(j>s)break;
        int key=quo.get_key((double)quo[i]/(double)j);
        res[i]+=v*(f[key]-f[s-1]);
      }
    }
    return res;
  }
  template<typename T,bool calc_all=true,typename Func>
  std::conditional_t<calc_all,std::vector<T>,T>solve(const std::vector<T>&f_prime,const Func&f){
    static_assert(is_modint_v<T>);
    std::vector<T>invs(std::max(be,msb(quo.n)+1)),finvs(std::max(be,msb(quo.n)+1));
    invs[0]=T();
    finvs[0]=T::raw(1);
    for(int i=1;i<(int)invs.size();i++)finvs[i]=(invs[i]=T::raw(i).inv())*finvs[i-1];
    std::vector<T>small(f_prime);
    int nbpid=(std::upper_bound(p.begin(),p.end(),nb)-p.begin())-1;
    if(nbpid!=-1){
      std::fill(small.begin()+p[nbpid],small.end(),small[p[nbpid]-1]);
      for(int i=nbpid;i>=0;i--){
        const int x=p[i];
        const long long x2=(long long)x*x;
        const T fpx=f_prime[x-1];
        for(int j=(int)small.size()-1;quo[j]>=x2;j--){
          int c=1;
          T fxc=f(x,c),fxc1=f(x,c+1);
          long long xc=x;
          while(true){
            small[j]+=fxc*(small[quo.get_key(quo[j]/xc)]-fpx)+fxc1;
            xc*=x;
            if(xc*x>quo[j])break;
            fxc=fxc1;
            fxc1=f(x,++c+1);
          }
        }
      }
      for(T&x:small)x++;
    }
    else{
      std::fill(small.begin(),small.end(),T::raw(1));
    }
    std::vector<T>medium_log(quo.size());
    std::vector<T>base,logpsum,powbase;
    base.reserve(be-1),logpsum.reserve(be-1),powbase.reserve(be-1);
    for(int i=nbpid+1;i<(int)p.size();i++){
      base.clear();
      base.emplace_back();
      const int x=p[i];
      long long xc=1;
      for(int j=1;j<be;j++){
        if(xc<=quo.n/x){
          xc*=x;
          base.emplace_back(-f(x,j));
        }
        else break;
      }
      logpsum.assign(base.size(),T());
      powbase.assign(base.size(),T());
      powbase[0]++;
      for(int j=1;j<(int)base.size();j++){
        inplace_mul(powbase,base);
        for(int k=1;k<base.size();k++)logpsum[k]-=powbase[k]*invs[j];
      }
      xc=1;
      for(int j=1;j<(int)base.size();j++){
        xc*=x;
        medium_log[quo.get_key(xc)]+=logpsum[j];
      }
    }
    for(int i=1;i<(int)medium_log.size();i++)medium_log[i]+=medium_log[i-1];
    std::vector<T>medium(medium_log);
    for(T&x:medium)x++;
    std::vector<T>powmediumlog(medium_log);
    T finv=invs[1];
    for(int i=2;i<be;i++){
      powmediumlog=dirichlet_convolution_sparse(powmediumlog,medium_log);
      finv*=invs[i];
      for(int j=0;j<(int)medium.size();j++)medium[j]+=powmediumlog[j]*finv;
    }
    std::vector<T>large(quo.size(),T::raw(1));
    for(int i=quo.s;i<(int)f_prime.size();i++)large[i]+=f_prime[i]-f_prime[quo.s-1];
    medium=dirichlet_convolution_sparse(medium,large);
    if constexpr(calc_all)return dirichlet_convolution_sparse(small,medium);
    else{
      T res=small[0]*medium.back()+medium[0]*(small.back()-small[quo.s-1]);
      for(int i=2;i<=quo.s;i++){
        res+=(small[i-1]-small[i-2])*medium[quo.size()-i];
        res+=(medium[i-1]-medium[i-2])*(small[quo.size()-i]-small[quo.s-1]);
      }
      return res;
    }
  }
};
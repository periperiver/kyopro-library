#pragma once
#include "arbitrary_mod_convolution.hpp"
#include "../poly/arbitrary_chirp_z_transform.hpp"
#include "../math/garner.hpp"
#include "../math/primality_test.hpp"
#include "../math/arbitrary_modint.hpp"
#include "../math/primitive_root.hpp"
#include "../other/type.hpp"
template<typename T,typename Func>
void multidimensional_dft(const std::vector<int>&ns,std::vector<T>&f,T g,const Func&conv){
  int n_prod=1;
  for(int i=0;i<std::ssize(ns);i++){
    T root=g.pow((T::mod()-1)/ns[i]);
    std::vector<T>buf(ns[i]);
    for(int j=0;j<std::ssize(f);j++)if((j/n_prod)%ns[i]==0){
      for(int k=0;k<ns[i];k++)buf[k]=f[j+n_prod*k];
      buf=arbitrary_chirp_z_transform(buf,root,ns[i],conv);
      for(int k=0;k<ns[i];k++)f[j+n_prod*k]=buf[k];
    }
    n_prod*=ns[i];
  }
}
template<typename T,std::enable_if_t<is_modint_v<T>,std::nullptr_t> =nullptr>
std::vector<T>multidimensional_cyclic_convolution(std::vector<int>ns,std::vector<T>a,std::vector<T>b){
  int n=1;
  bool use_crt=false;
  if(!isprime(T::mod()))use_crt=true;
  for(int x:ns){
    if((T::mod()-1)%x!=0)use_crt=true;
    n*=x;
  }
  assert(n==std::ssize(a)&&n==std::ssize(b));
  if(use_crt){
    using mint1=arbitrary_modint<20250909>;
    using mint2=arbitrary_modint<20250910>;
    using mint3=arbitrary_modint<20250911>;
    std::vector<int>mods;
    int z=1000000000/n;
    while(std::ssize(mods)<3){
      if(isprime(n*z+1))mods.push_back(n*z+1);
      z++;
    }
    mint1::set_mod(mods[0]);
    mint2::set_mod(mods[1]);
    mint3::set_mod(mods[2]);
    std::vector<mint1>a1(n),b1(n);
    std::vector<mint2>a2(n),b2(n);
    std::vector<mint3>a3(n),b3(n);
    for(int i=0;i<n;i++){
      a1[i]=a[i].val();
      a2[i]=a[i].val();
      a3[i]=a[i].val();
      b1[i]=b[i].val();
      b2[i]=b[i].val();
      b3[i]=b[i].val();
    }
    a1=multidimensional_cyclic_convolution(ns,std::move(a1),std::move(b1));
    a2=multidimensional_cyclic_convolution(ns,std::move(a2),std::move(b2));
    a3=multidimensional_cyclic_convolution(ns,std::move(a3),std::move(b3));
    for(int i=0;i<n;i++){
      std::vector<std::pair<int,int>>c(3);
      c[0]=std::make_pair(a1[i].val(),mint1::mod());
      c[1]=std::make_pair(a2[i].val(),mint2::mod());
      c[2]=std::make_pair(a3[i].val(),mint3::mod());
      a[i]=garner(c,T::mod());
    }
    return a;
  }
  T g=primitive_root(T::mod());
  multidimensional_dft(ns,a,g,arbitrary_mod_convolution<T>);
  multidimensional_dft(ns,b,g,arbitrary_mod_convolution<T>);
  for(int i=0;i<n;i++)a[i]*=b[i];
  multidimensional_dft(ns,a,g.inv(),arbitrary_mod_convolution<T>);
  T ninv=T(n).inv();
  for(int i=0;i<n;i++)a[i]*=ninv;
  return a;
}
#pragma once
#include "../convolution/ntt.hpp"
#include "factorial.hpp"
#include "../other/type.hpp"
template<typename T>
T fast_inv_sum(int n){
  static bool prepared=false;
  static std::vector<T>f;
  static constexpr int b=11;
  static constexpr int sz=T::mod()/(1<<b);
  if(!prepared){
    std::vector<T>num{1,1},den{1,3};
    for(int i=1;i<b;i++){
      std::vector<T>c(4<<i),nums(4<<i),dens(4<<i);
      for(int j=0;j<(4<<i);j++)c[j]=F<T>::inverse(j+1);
      for(int j=0;j<(1<<i);j++){
        T fv=F<T>::factorial_inv(j)*F<T>::factorial_inv((1<<i)-j-1);
        if(j%2==0)fv=-fv;
        nums[j]=num[j]*fv;
        dens[j]=den[j]*fv;
      }
      dft(c),dft(nums),dft(dens);
      for(int j=0;j<(4<<i);j++){
        nums[j]*=c[j];
        dens[j]*=c[j];
      }
      idft(nums),idft(dens);
      T inv=T::raw(4<<i).inv();
      for(int j=(4<<i)-1;j>=1<<i;j--){
        T fv=inv*F<T>::P(j,1<<i);
        nums[j]=nums[j-1]*fv;
        dens[j]=dens[j-1]*fv;
      }
      std::copy(num.begin(),num.end(),nums.begin());
      std::copy(den.begin(),den.end(),dens.begin());
      num.resize(2<<i),den.resize(2<<i);
      for(int j=0;j<(2<<i);j++){
          den[j]=dens[j*2]*dens[j*2+1];
          num[j]=(nums[j*2]*dens[j*2+1]+nums[j*2+1]*dens[j*2])*T::raw((j*2+1)<<i)+den[j];
          den[j]*=T::raw((j*2+1)<<i);
      }
    }
    static constexpr int s=ceil_pow2(sz-1);
    std::vector<T>c(s),nums(s),dens(s);
    for(int i=0;i<sz;i++)c[i]=F<T>::inverse(i+1);
    for(int i=0;i<(1<<b);i++){
      T fv=F<T>::factorial_inv(i)*F<T>::factorial_inv((1<<b)-i-1);
      if(i%2==0)fv=-fv;
      nums[i]=num[i]*fv;
      dens[i]=den[i]*fv;
    }
    dft(c),dft(nums),dft(dens);
    for(int i=0;i<s;i++){
      nums[i]*=c[i];
      dens[i]*=c[i];
    }
    idft(nums),idft(dens);
    f.resize(sz+1);
    T inv=T::raw(s).inv();
    for(int i=0;i<(1<<b);i++){
      f[i+1]=num[i]/den[i]+T::raw((i+1)<<b).inv()+f[i];
    }
    for(int i=(1<<b);i<sz;i++){
      f[i+1]=nums[i-1]/dens[i-1]+T::raw((i+1)<<b).inv()+f[i];
    }
    prepared=true;
  }
  if((n&((1<<b)-1))<=1<<(b-1)||(n>>b)>=sz){
    T num=f[n>>b],den=T::raw(1);
    for(int i=((n>>b)<<b)+1;i<=n;i++){
      num=num*T::raw(i)+den;
      den*=T::raw(i);
    }
    return num/den;
  }
  else{
    T num=f[(n>>b)+1],den=T::raw(1);
    for(int i=n+1;i<=(((n>>b)+1)<<b);i++){
      num=num*T::raw(i)-den;
      den*=T::raw(i);
    }
    return num/den;
  }
}

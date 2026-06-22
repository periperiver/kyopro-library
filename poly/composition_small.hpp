#pragma once
#include "polynomial_talor_shift.hpp"
#include "composition.hpp"
//f(ax^2+bx+c)
template<typename T>
std::vector<T>composition_small(std::vector<T>f,T a,T b,T c){
  if(f.empty())return f;
  if(a.val()==0&&b.val()==0){
    T s=T();
    for(int i=f.size()-1;i>=0;i--)s=s*c+f[i];
    f[0]=s;
    std::fill(f.begin()+1,f.end(),T());
    return f;
  }
  auto f_ax_b=[&f](T a,T b)->void {
    T powa=1;
    for(int i=0;i<(int)f.size();i++){
      f[i]*=powa;
      powa*=a;
    }
    f=polynomial_talor_shift(f,b/a);
  };
  if(a.val()==0)f_ax_b(b,c);
  else{
    f_ax_b(a,c-b*b/(a*4));
    int n=f.size()-1;
    f.resize(n*2+1);
    for(int i=n*2;i>=0;i--){
      if(i%2==0)f[i]=f[i/2];
      else f[i]=T();
    }
    f=polynomial_talor_shift(f,b/(a*2));
    f.resize(n+1);
  }
  return f;
}
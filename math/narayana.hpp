#pragma once
#include "factorial.hpp"
#include "other/type.hpp"
#include<cassert>
//(0,0)->(n,n)のパスでy≤xを満たす点のみを通り左折k回
template<typename T,std::enable_if_t<is_modint_v<T>,std::nullptr_t> =nullptr>
inline T narayana(int n,int k){
  return F<T>::C(n,k)*F<T>::C(n,k-1)*F<T>::inverse(n);
}
//(0,0)->(n,m)のパスでy≤xを満たす点のみを通り左折k回
template<typename T,std::enable_if_t<is_modint_v<T>,std::nullptr_t> =nullptr>
inline T narayana(int n,int m,int k){
  return n>=m?F<T>::C(n-1,k-1)*F<T>::C(m-1,k-1)-F<T>::C(m-1,k)*F<T>::C(n-1,k-2):T();
}
//(0,0)->(n,m)のパスでy≤x+aを満たす点のみを通り右左折合計k回
template<typename T,std::enable_if_t<is_modint_v<T>,std::nullptr_t> =nullptr>
inline T narayana(int n,int m,int a,int k){
  if(n+a<m)return T();
  if(k%2==0)return F<T>::C(n-1,k/2)*F<T>::C(m-1,k/2-1)+F<T>::C(n-1,k/2-1)*F<T>::C(m-1,k/2)-F<T>::C(m-a-1,k/2)*F<T>::C(n+a-1,k/2-1)*T::raw(2);
  else return F<T>::C(n-1,k/2)*F<T>::C(m-1,k/2)*T::raw(2)-F<T>::C(m-a-1,k/2)*F<T>::C(n+a-1,k/2)-F<T>::C(m-a-1,k/2+1)*F<T>::C(n+a-1,k/2-1);
}
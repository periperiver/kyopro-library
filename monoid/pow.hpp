#pragma once
template<typename M,typename U>
M::S monoid_pow(typename M::S x,U k){
  if constexpr(requires(){M::pow(x,k);})return M::pow(x,k);
  else{
    typename M::S res=M::e();
    while(k){
      if(k&1)res=M::op(res,x);
      x=M::op(x,x);
      k>>=1;
    }
    return res;
  }
}
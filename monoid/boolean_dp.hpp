#pragma once
#include<array>
#include<concepts>
#include<cstdint>
template<std::size_t N>
requires(0<N&&N<=64)
struct BooleanDP{
  using value_type=std::conditional_t<(N<=8),std::uint8_t,std::conditional_t<(N<=16),std::uint16_t,std::conditional_t<(N<=32),std::uint32_t,uint64_t>>>;
  struct S{
    std::array<value_type,N>dat;
    S():dat{}{}
    struct bit_ref{
      value_type&v;
      value_type mask;
      bit_ref(value_type&v,value_type mask):v(v),mask(mask){}
      operator bool()const{return v&mask;}
      bit_ref &operator=(bool b){
        if(b)v|=mask;
        else v&=~mask;
        return *this;
      }
      bit_ref &operator|=(bool b){
        if(b)v|=mask;
        return *this;
      }
      bit_ref &operator&=(bool b){
        if(!b)v&=~mask;
        return *this;
      }
    };
    struct row{
      value_type&v;
      row(value_type&v):v(v){}
      bit_ref operator[](int i){return bit_ref(v,value_type(1)<<i);}
      bool operator[](int i)const{return v>>i&1;}
    };
    row operator[](int i){return row(dat[i]);}
    row operator[](int i)const{return row(const_cast<value_type&>(dat[i]));}
  };
  static S op(S x,S y){
    S res;
    for(int i=0;i<N;i++)for(int j=0;j<N;j++)if(x[i][j])res.dat[i]|=y.dat[j];
    return res;
  }
  static S e(){
    S res;
    for(int i=0;i<N;i++)res[i][i]=true;
    return res;
  }
};
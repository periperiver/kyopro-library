#pragma once
#include<iostream>
#include<cassert>
struct SurrealNumber{
  long long a,b;//a*2^-b
  SurrealNumber():a(0),b(0){}
  SurrealNumber(long long n):a(n),b(0){}
  SurrealNumber(long long a,long long b):a(a),b(b){normalize();}
  void normalize(){
    if(a==0)b=0;
    else{
      while(a%2==0&&b>0)a/=2,b--;
    }
  }
  SurrealNumber operator+()const{return *this;}
  SurrealNumber operator-()const{
    SurrealNumber res(*this);
    res.a=-res.a;
    return res;
  }
  SurrealNumber &operator+=(const SurrealNumber&rhs){
    long long b2=std::max(b,rhs.b);
    a=(a<<(b2-b))+(rhs.a<<(b2-rhs.b));
    b=b2;
    normalize();
    return *this;
  }
  SurrealNumber &operator-=(const SurrealNumber&rhs){return *this+=-rhs;}
  friend SurrealNumber operator+(const SurrealNumber&lhs,const SurrealNumber&rhs){return SurrealNumber(lhs)+=rhs;}
  friend SurrealNumber operator-(const SurrealNumber&lhs,const SurrealNumber&rhs){return SurrealNumber(lhs)-=rhs;}
  friend bool operator<(const SurrealNumber&lhs,const SurrealNumber&rhs){return (lhs-rhs).a<0;}
  friend bool operator>(const SurrealNumber&lhs,const SurrealNumber&rhs){return (lhs-rhs).a>0;}
  friend bool operator<=(const SurrealNumber&lhs,const SurrealNumber&rhs){return (lhs-rhs).a<=0;}
  friend bool operator>=(const SurrealNumber&lhs,const SurrealNumber&rhs){return (lhs-rhs).a>=0;}
  friend bool operator==(const SurrealNumber&lhs,const SurrealNumber&rhs){return (lhs-rhs).a==0;}
  friend bool operator!=(const SurrealNumber&lhs,const SurrealNumber&rhs){return (lhs-rhs).a!=0;}
  SurrealNumber left()const{
    if(a==0)return -1;
    if(a<0)return -(-*this).right();
    return *this-SurrealNumber(1,b+1);
  }
  SurrealNumber right()const{
    if(a==0)return 1;
    if(a<0)return -(-*this).left();
    if(b==0)return *this+1;
    return *this+SurrealNumber(1,b+1);
  }
  double to_double()const{return (double)a/(double)(1<<b);}
  friend std::ostream &operator<<(std::ostream&os,const SurrealNumber&s){
    os<<s.to_double();
    return os;
  }
  static SurrealNumber reduce(const SurrealNumber&lhs,const SurrealNumber&rhs){
    assert(lhs<rhs);
    SurrealNumber res;
    while(true){
      if(res<=lhs)res=res.right();
      else if(rhs<=res)res=res.left();
      else break;
    }
    return res;
  }
};
#pragma once
#include<unordered_set>
template<typename T,typename Func1,typename Func2>
long long babystep_giantstep(T s,T g,long long n,long long m,const Func1&f,const Func2&fm){
  T c=g;
  std::unordered_set<T>st;
  for(int i=0;i<m;i++){
    g=f(g);
    st.insert(g);
  }
  g=c;
  bool fl=false;
  long long ret=0;
  while(true){
    c=s;
    s=fm(s);
    if(st.find(s)!=st.end()){
      for(int i=0;i<m;i++){
        if(c==g)return ret+i;
        c=f(c);
      }
      if(fl)break;
      fl=true;
    }
    if(ret>n)break;
    ret+=m;
  }
  return -1;
}
#pragma once
#include<vector>
#include<algorithm>
template<typename T,bool MIN=true>
std::vector<T>path_matching(std::vector<T>a){
  if constexpr(!MIN){
    for(T&x:a)x=-x;
  }
  int n=a.size();
  std::vector<T>vals(n/2+1),st;
  vals.clear();
  for(T x:a){
    st.push_back(x);
    while((int)st.size()>=3){
      T&x=st.end()[-3],y=st.end()[-2],z=st.end()[-1];
      if(x>z)break;
      if((int)st.size()>=4&&st.end()[-4]<=y)break;
      vals.push_back(x+y);
      x=z;
      st.pop_back();
      st.pop_back();
    }
  }
  while((int)st.size()>=2){
    vals.push_back(st.end()[-2]+st.end()[-1]);
    st.pop_back();
    st.pop_back();
  }
  std::sort(vals.begin(),vals.end());
  vals.insert(vals.begin(),0);
  for(int i=0;i<(int)vals.size()-1;i++)vals[i+1]+=vals[i];
  if constexpr(!MIN){
    for(T&x:vals)x=-x;
  }
  return vals;
}
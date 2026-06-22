#pragma once
#include<vector>
#include<algorithm>
#include<numeric>
#include<cassert>
template<typename T>
std::vector<int>manber_myers_algorithm(T s){
  auto mn=*std::min_element(s.begin(),s.end());
  for(auto&x:s)x-=mn;
  int n=s.size();
  std::vector<int>rank(n+1),sa(n+1),temp(n+1);
  for(int i=0;i<n;i++)rank[i]=s[i];
  rank[n]=-1;
  std::iota(sa.begin(),sa.end(),0);
  rank[n]=-1;
  for(int k=1;k<=n;k<<=1){
    auto comp=[&](int lhs,int rhs)->bool {
      if(rank[lhs]!=rank[rhs])return rank[lhs]<rank[rhs];
      int l=lhs+k<=n?rank[lhs+k]:-1;
      int r=rhs+k<=n?rank[rhs+k]:-1;
      return l<r;
    };
    std::sort(sa.begin(),sa.end(),comp);
    temp[sa[0]]=0;
    for(int i=1;i<=n;i++){
      temp[sa[i]]=temp[sa[i-1]]+comp(sa[i-1],sa[i]);
    }
    std::copy(temp.begin(),temp.end(),rank.begin());
  }
  sa.erase(sa.begin());
  return sa;
}
std::vector<int>sa_is(const std::vector<int>&s){
  int n=s.size();
  int upper=*std::max_element(s.begin(),s.end());
  std::vector<int>sa(n);
  int m=0;
  std::vector<int>ls(n),is_lms(n);
  for(int i=n-2;i>=0;i--){
    if(s[i]==s[i+1])ls[i]=ls[i+1];
    else ls[i]=s[i]>s[i+1];
    if(ls[i]&&!ls[i+1]){
      is_lms[i+1]=1;
      m++;
    }
  }
  auto induced_sort=[&](const std::vector<int>&lms)->void {
    std::vector<int>l(upper+2),r(upper+2);
    for(const int&c:s){
      l[c+1]++;
      r[c]++;
    }
    for(int i=1;i<upper+2;i++)l[i]+=l[i-1],r[i]+=r[i-1];
    std::fill(sa.begin(),sa.end(),-1);
    for(int i=lms.size();i--;)sa[--r[s[lms[i]]]]=lms[i];
    for(const int&c:sa)if(c>=1&&ls[c-1])sa[l[s[c-1]]++]=c-1;
    std::fill(r.begin(),r.end(),0);
    for(const int&c:s)r[c]++;
    for(int i=1;i<upper+2;i++)r[i]+=r[i-1];
    for(int k=(int)sa.size()-1,i=sa[k];k>=1;i=sa[--k]){
      if(i>=1&&!ls[i-1])sa[--r[s[i-1]]]=i-1;
    }
  };
  std::vector<int>lms;
  lms.reserve(m);
  for(int i=1;i<n;i++)if(is_lms[i])lms.push_back(i);
  induced_sort(lms);
  std::vector<int>nlms;
  nlms.reserve(m);
  for(int i=0;i<n;i++){
    if(!ls[sa[i]]&&sa[i]>0&&ls[sa[i]-1])nlms.push_back(sa[i]);
  }
  auto same=[&](int lhs,int rhs)->bool {
    if(s[lhs++]!=s[rhs++])return false;
    while(true){
      if(s[lhs]!=s[rhs])return false;
      if(is_lms[lhs]&&is_lms[rhs])return is_lms[lhs]&&is_lms[rhs];
      lhs++,rhs++;
    }
  };
  int rank=0;
  sa[n-1]=0;
  for(int i=1;i<m;i++){
    if(!same(nlms[i-1],nlms[i]))rank++;
    sa[nlms[i]]=rank;
  }
  if(rank+1<m){
    std::vector<int>ns(m);
    for(int i=0;i<m;i++)ns[i]=sa[lms[i]];
    std::vector<int>lms_sa=sa_is(ns);
    for(int i=0;i<m;i++)nlms[i]=lms[lms_sa[i]];
  }
  induced_sort(nlms);
  return sa;
}
template<typename T>
std::vector<int>suffix_array(const T&s){
  static_assert(std::is_integral_v<typename T::value_type>);
  if(s.size()<=50)return manber_myers_algorithm(s);
  std::vector<int>s2(s.size()+1);
  int mn=*std::min_element(s.begin(),s.end());
  for(int i=0;i<s.size();i++)s2[i]=s[i]-mn+1;
  std::vector<int>res=sa_is(s2);
  res.erase(res.begin());
  return res;
}
#pragma once
#include<iostream>
#include<array>
#include<algorithm>
#include "p_recursive.hpp"
#include "../geo/manhattan_mst.hpp"
#include "../other/type.hpp"
namespace p_recursive2d_impl{
template<typename T>
std::vector<std::vector<T>>transposed(const std::vector<std::vector<T>>&a){
  int n=a.size(),m=a[0].size();
  std::vector<std::vector<T>>res(m,std::vector<T>(n));
  for(int i=0;i<n;i++)for(int j=0;j<m;j++)res[j][i]=a[i][j];
  return res;
}
template<typename T>
T eval(const std::vector<T>f,T x){
  T res=0,p=1;
  for(T a:f){
    res+=a*p;
    p*=x;
  }
  return res;
}
template<typename T,size_t s>
inline T eval(const std::array<T,s>&f,T x){
  if constexpr(s==0)return T();
  if constexpr(s==1)return f[0];
  if constexpr(s==2)return f[0]+f[1]*x;
  if constexpr(s==3)return f[0]+(f[1]+f[2]*x)*x;
  if constexpr(s==4)return f[0]+(f[1]+(f[2]+f[3]*x)*x)*x;
  T res=T(),p=T::raw(1);
  for(const T&a:f){
    res+=a*p;
    p*=x;
  }
  return res;
}
template<typename T>
bool check(const std::vector<std::vector<T>>a,int i,std::vector<std::vector<T>>coef){
  int m=a[i].size();
  int r=coef.size();
  for(int j=0;j<=m-r;j++){
    T v=0;
    for(int k=0;k<r;k++){
      v+=eval(coef[k],T(j))*a[i][j+k];
    }
    if(v.val())return false;
  }
  return true;
}
template<typename T>
std::vector<T>interpolate(std::vector<T>x,std::vector<T>y){
  int n=x.size()-1;
  for(int i=0;i<=n;i++){
    T v=1;
    for(int j=0;j<=n;j++)if(i!=j)v*=x[i]-x[j];
    y[i]/=v;
  }
  std::vector<T>dp(n+2);
  dp[0]=-x[0],dp[1]=1;
  for(int i=1;i<=n;i++){
    std::vector<T>ndp(n+2);
    for(int j=0;j<n+2;j++){
      ndp[j]-=dp[j]*x[i];
      if(j)ndp[j]+=dp[j-1];
    }
    dp=std::move(ndp);
  }
  std::vector<T>res(n+1);
  for(T&i:x)if(i.val())i=i.inv();
  for(int i=0;i<=n;i++){
    if(y[i].val()==0)continue;
    if(x[i].val()==0){
      for(int j=0;j<=n;j++)res[j]+=dp[j+1]*y[i];
    }
    else{
      res[0]-=dp[0]*x[i]*y[i];
      T pre=-dp[0]*x[i];
      for(int j=1;j<=n;j++){
        res[j]-=(dp[j]-pre)*x[i]*y[i];
        pre=-(dp[j]-pre)*x[i];
      }
    }
  }
  return res;
}
template<typename T>
struct p_recursive2d_result{
  int r,d;
  std::vector<std::vector<std::vector<T>>>coef_poly;
};
template<typename T>
p_recursive2d_result<T> make_p_recursive2d_row(std::vector<std::vector<T>>a){
  int n=a.size(),m=a[0].size();
  for(int d=0;d+2<=m;d++){
    std::vector<std::vector<std::vector<T>>>coefs(n);
    bool fail=false;
    for(int i=0;i<n;i++){
      coefs[i]=find_p_recursive(std::vector<T>(a[i].begin(),a[i].end()-2),d);
      if(coefs[i].empty()){
        fail=true;
        break;
      }
    }
    std::cerr<<"-----------------------------------------------d="<<d<<"-----------------------------------------------"<<std::endl;
    if(fail){
      std::cerr<<"fail"<<std::endl;
    }
    else{
      for(int i=0;i<n;i++){
        std::cerr<<"coefs at i="<<i<<std::endl;
        for(int j=0;j<(int)coefs[i].size();j++){
          std::cerr<<'(';
          for(int k=0;k<(int)coefs[i][j].size();k++){
            std::cerr<<coefs[i][j][k];
            if(k+1<(int)coefs[i][j].size())std::cerr<<',';
          }
          std::cerr<<')';
          if(j+1<(int)coefs[j].size())cerr<<' ';
        }
        std::cerr<<std::endl;
        if(check(a,i,coefs[i]))std::cerr<<"\x1b[38;2;0;255;0mcorrect coef\x1b[m"<<std::endl;
        else{
          std::cerr<<"\x1b[38;2;255;0;0mwrong coef\x1b[m"<<std::endl;
          fail=true;
        }
      }
    }
    std::cerr<<"-------------------------------------------------------------------------------------------------"<<std::endl;
    if(fail||std::any_of(coefs.begin(),coefs.end(),[&](const std::vector<std::vector<T>>&v){return v.size()!=coefs[0].size();}))continue;
    int r=coefs[0].size();
    std::vector<std::vector<std::vector<T>>>poly(r,std::vector<std::vector<T>>(d+1));
    for(int i=n-1;i-->2;){
      for(int j=0;j<r;j++)for(int k=0;k<=d;k++){
        std::vector<T>x,y;
        for(int i2=i;i2<n;i2++){
          x.push_back(i2);
          y.push_back(coefs[i2][j][k]);
        }
        poly[j][k]=interpolate(x,y);
      }
      bool ok=true;
      for(int i2=0;i2<i;i2++){
        std::vector<std::vector<T>>coef_i(r,std::vector<T>(d+1));
        for(int j=0;j<r;j++)for(int k=0;k<=d;k++)coef_i[j][k]=eval(poly[j][k],T(i2));
        if(!check(a,i2,coef_i)){
          ok=false;
          break;
        }
      }
      if(ok){
        p_recursive2d_result<T>res;
        res.r=r;
        res.d=d;
        res.coef_poly=std::move(poly);
        return res;
      }
    }
  }
  p_recursive2d_result<T> res;
  res.r=res.d=-1;
  return res;
}
template<typename T>
void make_p_recursive2d(std::vector<std::vector<T>>a,std::string name="",std::string T_name="mint"){
  static_assert(is_modint_v<T>);
  assert(!a.empty());
  assert(!a[0].empty());
  int n=a.size(),m=a[0].size();
  for(int i=0;i<n;i++){
    assert((int)a[i].size()==m);
    assert(std::all_of(a[i].begin(),a[i].end(),[&](T x){return x.val()!=0;}));
  }
  std::cerr<<"\n\n\n\nrow\n\n\n";
  p_recursive2d_result<T>row=make_p_recursive2d_row(a);
  std::cerr<<"\n\n\n\ncol\n\n\n";
  p_recursive2d_result<T>col=make_p_recursive2d_row(transposed(a));
  std::cout<<row.d<<' '<<row.r<<std::endl;
  for(int i=0;i<row.r;i++){
    for(int j=0;j<=row.d;j++){
      std::cout<<'(';
      for(int k=0;k<(int)row.coef_poly[i][j].size();k++)std::cout<<row.coef_poly[i][j][k]<<' ';
      std::cout<<')';
    }
    std::cout<<std::endl;
  }
  std::cout<<col.d<<' '<<col.r<<std::endl;
  for(int i=0;i<col.r;i++){
    for(int j=0;j<=col.d;j++){
      std::cout<<'(';
      for(int k=0;k<(int)col.coef_poly[i][j].size();k++)std::cout<<col.coef_poly[i][j][k]<<' ';
      std::cout<<')';
    }
    std::cout<<std::endl;
  }
  if(row.r==2&&col.r==2){
    std::cout<<"fast form"<<std::endl;
    std::cout<<T_name<<' '<<name<<"_a="<<a[0][0]<<";\n";
    std::cout<<"std::array<std::array<"<<T_name<<","<<row.coef_poly[0][0].size()<<">,"<<row.d+1<<">"<<name<<"_rowl={";
    for(int i=0;i<=row.d;i++)for(int j=0;j<(int)row.coef_poly[0][i].size();j++){
      std::cout<<row.coef_poly[0][i][j];
      if(i==row.d&&j+1==(int)row.coef_poly[0][i].size())std::cout<<"};\n";
      else std::cout<<',';
    }
    std::cout<<"std::array<std::array<"<<T_name<<","<<row.coef_poly[1][0].size()<<">,"<<row.d+1<<">"<<name<<"_rowr={";
    for(int i=0;i<=row.d;i++)for(int j=0;j<(int)row.coef_poly[0][i].size();j++){
      std::cout<<-row.coef_poly[1][i][j];
      if(i==row.d&&j+1==(int)row.coef_poly[1][i].size())std::cout<<"};\n";
      else std::cout<<',';
    }
    std::cout<<"std::array<std::array<"<<T_name<<","<<col.coef_poly[0][0].size()<<">,"<<col.d+1<<">"<<name<<"_coll={";
    for(int i=0;i<=col.d;i++)for(int j=0;j<(int)col.coef_poly[0][i].size();j++){
      std::cout<<col.coef_poly[0][i][j];
      if(i==col.d&&j+1==(int)col.coef_poly[0][i].size())std::cout<<"};\n";
      else std::cout<<',';
    }
    std::cout<<"std::array<std::array<"<<T_name<<","<<col.coef_poly[1][0].size()<<">,"<<col.d+1<<">"<<name<<"_colr={";
    for(int i=0;i<=col.d;i++)for(int j=0;j<(int)col.coef_poly[0][i].size();j++){
      std::cout<<-col.coef_poly[1][i][j];
      if(i==col.d&&j+1==(int)col.coef_poly[1][i].size())std::cout<<"};\n";
      else std::cout<<',';
    }
  }
}
template<typename T,size_t d_row,size_t d_col,size_t p_row,size_t p_col>
void extend(T&num,T&den,const std::array<std::array<T,p_row>,d_row>&rowl,const std::array<std::array<T,p_row>,d_row>&rowr,const std::array<std::array<T,p_col>,d_col>&coll,const std::array<std::array<T,p_col>,d_col>&colr,int i,int j,int ni,int nj){
  if(ni<i){
    std::array<T,d_col+1>coefl,coefr;
    for(int k=0;k<(int)d_col;k++){
      coefl[k]=eval(coll[k],T::raw(j));
      coefr[k]=eval(colr[k],T::raw(j));
    }
    while(ni<i){
      i--;
      num*=eval(coefr,T::raw(i));
      den*=eval(coefl,T::raw(i));
    }
  }
  if(nj<j){
    std::array<T,d_row+1>coefl,coefr;
    for(int k=0;k<(int)d_row;k++){
      coefl[k]=eval(rowl[k],T::raw(i));
      coefr[k]=eval(rowr[k],T::raw(i));
    }
    while(nj<j){
      j--;
      num*=eval(coefr,T::raw(j));
      den*=eval(coefl,T::raw(j));
    }
  }
  if(ni>i){
    std::array<T,d_col+1>coefl,coefr;
    for(int k=0;k<(int)d_col;k++){
      coefl[k]=eval(coll[k],T::raw(j));
      coefr[k]=eval(colr[k],T::raw(j));
    }
    while(ni>i){
      num*=eval(coefl,T::raw(i));
      den*=eval(coefr,T::raw(i));
      i++;
    }
  }
  if(nj>j){
    std::array<T,d_row+1>coefl,coefr;
    for(int k=0;k<(int)d_row;k++){
      coefl[k]=eval(rowl[k],T::raw(i));
      coefr[k]=eval(rowr[k],T::raw(i));
    }
    while(nj>j){
      num*=eval(coefl,T::raw(j));
      den*=eval(coefr,T::raw(j));
      j++;
    }
  }
}
template<typename T,size_t d_row,size_t d_col,size_t p_row,size_t p_col>
std::vector<T>calc_p_recursive2d(T a,std::array<std::array<T,p_row>,d_row>rowl,std::array<std::array<T,p_row>,d_row>rowr,std::array<std::array<T,p_col>,d_col>coll,std::array<std::array<T,p_col>,d_col>colr,std::vector<std::pair<int,int>>point){
  int n=point.size();
  static constexpr int none=-1e8;
  for(std::pair<int,int>&p:point)if(p.first<0||p.second<0)p=std::make_pair(none,none);
  auto t=manhattan_mst(point);
  int root=-1;
  for(int i=0;i<n;i++)if(point[i].first>=0){
    if(root==-1)root=i;
    else if(point[i].first+point[i].second<point[root].first+point[root].second)root=i;
  }
  std::vector<T>res(n);
  if(root==-1)return res;
  auto dfs=[&](auto self,int x,int p,T num,T den)->void {
    res[x]=num/den;
    for(const Edge<>&e:t[x])if(e.to!=p){
      if(point[e.to].first<0)continue;
      T num2=num,den2=den;
      extend(num2,den2,rowl,rowr,coll,colr,point[x].first,point[x].second,point[e.to].first,point[e.to].second);
      self(self,e.to,x,num2,den2);
    }
  };
  T b=1;
  extend(a,b,rowl,rowr,coll,colr,0,0,point[root].first,point[root].second);
  dfs(dfs,root,-1,a,b);
  return res;
}
}
using p_recursive2d_impl::make_p_recursive2d;
using p_recursive2d_impl::calc_p_recursive2d;
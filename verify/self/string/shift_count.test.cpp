#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "string/shift_count.hpp"
#include "random/generator.hpp"
std::vector<int>naive(auto f,auto g){
  std::vector<int>res(f.size()-g.size()+1);
  for(int i=0;i<(int)res.size();i++)for(int j=0;j<(int)g.size();j++)res[i]+=f[i+j]==g[j];
  return res;
}
void test(auto f,auto g){
  assert(shift_count(f,g)==naive(f,g));
}
void test2(auto f,auto g){
  auto ans=shift_count(f,g);
  assert(ans.size()==f.size()-g.size()+1);
  int n=ans.size();
  for(int t=0;t<100;t++){
    int s=Random::range(n);
    int c=0;
    for(int i=0;i<(int)g.size();i++)c+=f[i+s]==g[i];
    assert(ans[s]==c);
  }
}
std::string gen_str(int n,int sigma){
  std::string res;
  for(int i=0;i<n;i++)res+='a'+Random::range(sigma);
  return res;
}
std::vector<int>gen_vec(int n,int sigma){
  std::vector<int>res(n);
  for(int i=0;i<n;i++)res[i]=Random::range(sigma);
  return res;
}
int main(){
  for(int n=1;n<100;n++){
    for(int m=1;m<=n;m++){
      for(int s:{1,2,3,5,10,26}){
        test(gen_str(n,s),gen_str(m,s));
      }
      test(gen_vec(n,1000000000),gen_vec(m,1000000000));
    }
  }
  for(int t=0;t<10;t++)test(gen_str(10000,10),gen_str(3000,10));
  for(int t=0;t<10;t++){
    int n=Random::range(1,500000);
    int m=Random::range(1,n+1);
    test2(gen_str(n,5),gen_str(m,5));
    test2(gen_str(n,26),gen_str(m,26));
    test2(gen_vec(n,100),gen_vec(m,100));
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}
#pragma ocne
#include<vector>
#include<cmath>
long long count_prime(long long n){
  if(n<=1)return 0;
  int v=std::sqrt(n);
  int s=(v+1)/2;
  std::vector<int>smalls(s),roughs(s);
  std::vector<long long>larges(s);
  for(int i=0;i<s;i++){
    smalls[i]=i;
    roughs[i]=2*i+1;
    larges[i]=(n/(2*i+1)-1)/2;
  }
  std::vector<bool>skip(v+1);
  int pc=0;
  for(int p=3;p<=v;p+=2)if(!skip[p]){
    int q=p*p;
    if((long long)q*q>n)break;
    skip[p]=true;
    for(int i=q;i<=v;i+=2*p)skip[i]=true;
    int ns=0;
    for(int k=0;k<s;k++){
      int i=roughs[k];
      if(skip[i])continue;
      long long d=(long long)i*p;
      larges[ns]=larges[k]-(d<=v?larges[smalls[d>>1]-pc]:smalls[((int)((double)n/d)-1)>>1])+pc;
      roughs[ns++]=i;
    }
    s=ns;
    for(int i=(v-1)>>1,j=((v/p)-1)|1;j>=p;j-=2){
      int c=smalls[j>>1]-pc;
      for(int e=(j*p)>>1;i>=e;i--)smalls[i]-=c;
    }
    pc++;
  }
  larges[0]+=(long long)(s+2*(pc-1))*(s-1)/2;
  for(int k=1;k<s;k++)larges[0]-=larges[k];
  for(int l=1;l<s;l++){
    int q=roughs[l];
    long long m=n/q;
    int e=smalls[(m/q-1)>>1]-pc;
    if(e<l+1)break;
    long long t=0;
    for(int k=l+1;k<=e;k++)t+=smalls[((int)((double)m/roughs[k])-1)>>1];
    larges[0]+=t-(long long)(e-l)*(pc+l-1);
  }
  return larges[0]+1;
}
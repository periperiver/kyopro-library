#pragma once
namespace Random{
constexpr unsigned int constexpr_random32(){
  char c[]=__TIME__;
  unsigned int x=(c[0]-'0')*10+(c[1]-'0');
  unsigned int y=(c[3]-'0')*10+(c[4]-'0');
  unsigned int z=(c[6]-'0')*10+(c[7]-'0');
  unsigned int a=x*3600+y*60+z;
  for(int i=0;i<10;i++){
    a^=a<<13;
    a^=a>>17;
    a^=a<<5;
  }
  return a;
}
constexpr unsigned long long constexpr_random64(){
  char c[]=__TIME__;
  unsigned int x=(c[0]-'0')*10+(c[1]-'0');
  unsigned int y=(c[3]-'0')*10+(c[4]-'0');
  unsigned int z=(c[6]-'0')*10+(c[7]-'0');
  unsigned long long a=x*3600+y*60+z;
  for(int i=0;i<10;i++){
    a^=a<<13;
    a^=a>>7;
    a^=a<<17;
  }
  return a;
}
}
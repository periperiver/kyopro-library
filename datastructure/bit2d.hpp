#pragma once
#include <vector>
using namespace std;
template<typename T>
struct BinaryIndexedTree2D{
  vector<T>dat;
  int h,w;
  BinaryIndexedTree2D(int h,int w):h(h),w(w),dat(h*w,0){}
  void add(int i,int j,T v){
    i++,j++;
    while(i<=h){
      int j2=j;
      while(j2<=w){
        dat[(i-1)*w+j2-1]+=v;
        j2+=j2&-j2;
      }
      i+=i&-i;
    }
  }
  T sum2(int x,int y)const{
    T ret=0;
    while(x>0){
      int y2=y;
      while(y2>0){
        ret+=dat[(x-1)*w+y2-1];
        y2-=y2&-y2;
      }
      x-=x&-x;
    }
    return ret;
  }
  T sum(int lx,int rx,int ly,int ry)const{
    return sum2(lx,ly)-sum2(rx,ly)-sum2(lx,ry)+sum2(rx,ry);
  }
};
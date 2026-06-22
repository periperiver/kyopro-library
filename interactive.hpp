#pragma once
#define INTERACTIVE
#include<iostream>
#include<fstream>
#include "random/test.hpp"
using namespace std;
namespace interactive{
ofstream ofs("err");
template<typename T>
void read_rec(T&x){
  cin>>x;
}
template<typename T1,typename...T2>
void read_rec(T1&x,T2&...y){
  cin>>x;
  read_rec(y...);
}
template<typename T>
void err_rec(T x){
  ofs<<x;
}
template<typename T1,typename...T2>
void err_rec(T1 x,T2...y){
  ofs<<x<<' ';
  err_rec(y...);
}
template<typename T>
void write_rec(T x){
  cout<<x;
}
template<typename T1,typename...T2>
void write_rec(T1 x,T2...y){
  cout<<x<<' ';
  write_rec(y...);
}
template<typename T1,typename...T2>
void read(T1&x,T2&...y){
  read_rec(x,y...);
  ofs<<"[main -> interactor] ";
  err_rec(x,y...);
  ofs<<endl;
}
template<typename T1,typename...T2>
void write(T1 x,T2...y){
  ofs<<"[interactor -> main] ";
  err_rec(x,y...);
  ofs<<endl;
  write_rec(x,y...);
  cout<<endl;
}
}
using interactive::read;
using interactive::write;
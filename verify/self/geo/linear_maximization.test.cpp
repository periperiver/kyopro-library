#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "geo/linear_maximization.hpp"
#include "random/generator.hpp"
namespace gen{
struct Matrix{
  std::array<std::array<long long,3>,3>a;
  //det!=0
  bool check(){
    long long c=0;
    std::vector<long long>p{0,1,2};
    do{
      long long prod=1;
      for(int i=0;i<3;i++)prod*=a[i][p[i]];
      long long inv=0;
      for(int i=0;i<3;i++)for(int j=i+1;j<3;j++)inv+=p[i]>p[j];
      c+=(inv&1?-prod:prod);
    }while(std::next_permutation(p.begin(),p.end()));
    return c!=0;
  }
  void make(){
    for(int i=0;i<3;i++)for(int j=0;j<3;j++){
      a[i][j]=Random::range(-10,11);
    }
  }
  Matrix(){
    do make();while(!check());
  }
  friend Point3d<long long> operator*(const Matrix&lhs,const Point3d<long long>&rhs){
    Point3d<long long>res;
    res.x=lhs.a[0][0]*rhs.x+lhs.a[0][1]*rhs.y+lhs.a[0][2]*rhs.z;
    res.y=lhs.a[1][0]*rhs.x+lhs.a[1][1]*rhs.y+lhs.a[1][2]*rhs.z;
    res.z=lhs.a[2][0]*rhs.x+lhs.a[2][1]*rhs.y+lhs.a[2][2]*rhs.z;
    return res;
  }
};
constexpr long long max_xyz=1000000000;
Point3d<long long>single(int lim){
  Point3d<long long>res;
  res.x=Random::range(-lim,lim+1);
  res.y=Random::range(-lim,lim+1);
  res.z=Random::range(-lim,lim+1);
  return res;
}
bool isok(Point3d<long long>p){
  if(std::abs(p.x)>max_xyz)return false;
  if(std::abs(p.y)>max_xyz)return false;
  if(std::abs(p.z)>max_xyz)return false;
  return true;
}
void shift(std::vector<Point3d<long long>>&points){
  long long xmin=max_xyz,xmax=-max_xyz;
  long long ymin=max_xyz,ymax=-max_xyz;
  long long zmin=max_xyz,zmax=-max_xyz;
  for(Point3d<long long>&p:points){
    xmin=std::min(xmin,p.x);
    xmax=std::max(xmax,p.x);
    ymin=std::min(ymin,p.y);
    ymax=std::max(ymax,p.y);
    zmin=std::min(zmin,p.z);
    zmax=std::max(zmax,p.z);
  }
  Point3d<long long>d;
  d.x=Random::range(-xmin-max_xyz,max_xyz-xmax+1);
  d.y=Random::range(-ymin-max_xyz,max_xyz-ymax+1);
  d.z=Random::range(-zmin-max_xyz,max_xyz-zmax+1);
  for(Point3d<long long>&p:points)p+=d;
  assert(std::all_of(points.begin(),points.end(),[&](Point3d<long long>p){return isok(p);}));
}
std::vector<Point3d<long long>>random(int n){
  std::vector<Point3d<long long>>res(n);
  for(int i=0;i<n;i++){
    res[i]=single(max_xyz);
  }
  return res;
}
std::vector<Point3d<long long>>line(int n){
  Matrix coef;
  std::vector<Point3d<long long>>res;
  while((int)res.size()<n){
    Point3d<long long>now=single(max_xyz/10);
    now.y=now.z=0;
    now=coef*now;
    if(isok(now))res.push_back(now);
  }
  shift(res);
  return res;
}
std::vector<Point3d<long long>>plane(int n){
  Matrix coef;
  std::vector<Point3d<long long>>res;
  while((int)res.size()<n){
    Point3d<long long>now=single(max_xyz/10);
    now.z=0;
    now=coef*now;
    if(isok(now))res.push_back(now);
  }
  shift(res);
  return res;
}
std::vector<Point3d<long long>>many_line(int n,int t){
  std::vector<Point3d<long long>>res;
  for(int i=0;i<t;i++){
    std::vector<Point3d<long long>>a=line(n);
    res.insert(res.end(),a.begin(),a.end());
  }
  std::shuffle(res.begin(),res.end(),Random::mt);
  return res;
}
std::vector<Point3d<long long>>many_plane(int n,int t){
  std::vector<Point3d<long long>>res;
  for(int i=0;i<t;i++){
    std::vector<Point3d<long long>>a=plane(n);
    res.insert(res.end(),a.begin(),a.end());
  }
  std::shuffle(res.begin(),res.end(),Random::mt);
  return res;
}
std::vector<Point3d<long long>>parallel(int n){
  std::vector<Point3d<long long>>a;
  Point3d<long long>d;
  do{
    a=plane(n);
    d=single(10);
  }while(std::any_of(a.begin(),a.end(),[&](Point3d<long long>p){return !isok(p+d);}));
  a.insert(a.end(),a.begin(),a.end());
  for(int i=0;i<n;i++)a[i]+=d;
  return a;
}
std::vector<Point3d<long long>>dense(int n){
  std::vector<Point3d<long long>>res(n);
  for(int i=0;i<n;i++){
    res[i]=single(10);
  }
  return res;
}
}
std::vector<long long>naive(std::vector<Point3d<long long>>points,std::vector<Point3d<long long>>query){
  std::vector<long long>res(query.size(),-9e18);
  for(int i=0;i<(int)points.size();i++)for(int j=0;j<(int)query.size();j++){
    long long now=dot(points[i],query[i]);
    if(res[j]>now)res[j]=now;
  }
  return res;
}
void test(std::vector<Point3d<long long>>points,std::vector<Point3d<long long>>query){
  auto ans=linear_maximization<long long,__int128_t>(points,query);
  auto na=naive(points,query);
  assert(ans==na);
}
int main(){
  for(int n:{1,2,3,5,10,100,200}){
    test(gen::random(n),gen::dense(n));
  }
  int a,b;
  std::cin>>a>>b;
  std::cout<<a+b<<std::endl;
}
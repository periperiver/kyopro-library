#pragma once
template<typename I,typename M>
struct DynamicSegmentTree{
  using S=typename M::S;
private:
  struct node{
    node *left,*right,*par;
    S v;
    node():left(nullptr),right(nullptr),par(nullptr),v(M::e()){}
    node(const S&v_):left(nullptr),right(nullptr),par(nullptr),v(v_){}
    inline void update(){this->v=M::op(left?left->v:M::e(),right?right->v:M::e());}
  };
  node *root;
  I L,R;
public:
  DynamicSegmentTree():root(nullptr),L(),R(){}
  DynamicSegmentTree(I L_,I R_):root(new node()),L(L_),R(R_){}
  void set(I k,const S&x){
    node *nd=root;
    I l=L,r=R;
    while(r-l>1){
      I mid=(l+r)/2;
      if(k<mid){
        if(!nd->left){
          nd->left=new node();
          nd->left->par=nd;
        }
        nd=nd->left;
        r=mid;
      }
      else{
        if(!nd->right){
          nd->right=new node();
          nd->right->par=nd;
        }
        nd=nd->right;
        l=mid;
      }
    }
    nd->v=x;
    nd=nd->par;
    while(nd){
      nd->update();
      nd=nd->par;
    }
  }
  S get(I k)const{
    node *nd=root;
    I l=L,r=R;
    while(r-l>1){
      I mid=(l+r)/2;
      if(k<mid){
        if(!nd->left)return M::e();
        nd=nd->left;
      }
      else{
        if(!nd->right)return M::e();
        nd=nd->right;
      }
    }
    return nd->v;
  }
  S prod(I l,I r)const{
    if(l==r)return M::e();
    I nl=L,nr=R;
    node *nd=root;
    while(nr-nl>1&&nd){
      I mid=(nl+nr)/2;
      if(r<=mid)nd=nd->left,nr=mid;
      else if(mid<=l)nd=nd->right,nl=mid;
      else break;
    }
    if(!nd)return M::e();
    if(l==nl&&r==nr)return nd->v;
    S res=M::e();
    I nlr,nrl;
    nlr=nrl=(nl+nr)/2;
    node *ndl=nd->left,*ndr=nd->right;
    while(nl<l&&ndl){
      I mid=(nl+nlr)/2;
      if(l<mid){
        if(ndl->right)res=M::op(ndl->right->v,res);
        ndl=ndl->left;
        nlr=mid;
      }
      else ndl=ndl->right,nl=mid;
    }
    while(r<nr&&ndr){
      I mid=(nrl+nr)/2;
      if(mid<r){
        if(ndr->left)res=M::op(res,ndr->left->v);
        ndr=ndr->right;
        nrl=mid;
      }
      else ndr=ndr->left,nr=mid;
    }
    if(ndl)res=M::op(ndl->v,res);
    if(ndr)res=M::op(res,ndr->v);
    return res;
  }
};
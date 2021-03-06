#include <gtest/gtest.h>
#include <stan/agrad/fwd.hpp>
#include <stan/agrad/rev.hpp>
#include <test/unit/agrad/util.hpp>
#include <test/unit/agrad/fwd/nan_util.hpp>

TEST(AgradFwdExp,Fvar) {
  using stan::agrad::fvar;
  using std::exp;

  fvar<double> x(0.5,1.0);
  
  fvar<double> a = exp(x);
  EXPECT_FLOAT_EQ(exp(0.5), a.val_);
  EXPECT_FLOAT_EQ(exp(0.5), a.d_);

  fvar<double> b = 2 * exp(x) + 4;
  EXPECT_FLOAT_EQ(2 * exp(0.5) + 4, b.val_);
  EXPECT_FLOAT_EQ(2 * exp(0.5), b.d_);

  fvar<double> c = -exp(x) + 5;
  EXPECT_FLOAT_EQ(-exp(0.5) + 5, c.val_);
  EXPECT_FLOAT_EQ(-exp(0.5), c.d_);

  fvar<double> d = -3 * exp(-x) + 5 * x;
  EXPECT_FLOAT_EQ(-3 * exp(-0.5) + 5 * 0.5, d.val_);
  EXPECT_FLOAT_EQ(3 * exp(-0.5) + 5, d.d_);

  fvar<double> y(-0.5,1.0);
  fvar<double> e = exp(y);
  EXPECT_FLOAT_EQ(exp(-0.5), e.val_);
  EXPECT_FLOAT_EQ(exp(-0.5), e.d_);

  fvar<double> z(0.0,1.0);
  fvar<double> f = exp(z);
  EXPECT_FLOAT_EQ(exp(0.0), f.val_);
  EXPECT_FLOAT_EQ(exp(0.0), f.d_);
}

TEST(AgradFwdExp,FvarVar_1stDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using std::exp;

  fvar<var> x(0.5,1.3);
  fvar<var> a = exp(x);

  EXPECT_FLOAT_EQ(exp(0.5), a.val_.val());
  EXPECT_FLOAT_EQ(1.3 * exp(0.5), a.d_.val());

  AVEC y = createAVEC(x.val_);
  VEC g;
  a.val_.grad(y,g);
  EXPECT_FLOAT_EQ(exp(0.5), g[0]);
}

TEST(AgradFwdExp,FvarVar_2ndDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using std::exp;

  fvar<var> x(0.5,1.3);
  fvar<var> a = exp(x);

  AVEC y = createAVEC(x.val_);
  VEC g;
  a.d_.grad(y,g);
  EXPECT_FLOAT_EQ(1.3 * exp(0.5), g[0]);
}
TEST(AgradFwdExp,FvarFvarDouble) {
  using stan::agrad::fvar;
  using std::exp;

  fvar<fvar<double> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;

  fvar<fvar<double> > a = exp(x);

  EXPECT_FLOAT_EQ(exp(0.5), a.val_.val_);
  EXPECT_FLOAT_EQ(exp(0.5), a.val_.d_);
  EXPECT_FLOAT_EQ(0, a.d_.val_);
  EXPECT_FLOAT_EQ(0, a.d_.d_);

  fvar<fvar<double> > y;
  y.val_.val_ = 0.5;
  y.d_.val_ = 1.0;

  a = exp(y);
  EXPECT_FLOAT_EQ(exp(0.5), a.val_.val_);
  EXPECT_FLOAT_EQ(0, a.val_.d_);
  EXPECT_FLOAT_EQ(exp(0.5), a.d_.val_);
  EXPECT_FLOAT_EQ(0, a.d_.d_);
}

TEST(AgradFwdExp,FvarFvarVar_1stDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using std::exp;

  fvar<fvar<var> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;

  fvar<fvar<var> > a = exp(x);

  EXPECT_FLOAT_EQ(exp(0.5), a.val_.val_.val());
  EXPECT_FLOAT_EQ(exp(0.5), a.val_.d_.val());
  EXPECT_FLOAT_EQ(0, a.d_.val_.val());
  EXPECT_FLOAT_EQ(0, a.d_.d_.val());

  AVEC p = createAVEC(x.val_.val_);
  VEC g;
  a.val_.val_.grad(p,g);
  EXPECT_FLOAT_EQ(exp(0.5), g[0]);

  fvar<fvar<var> > y;
  y.val_.val_ = 0.5;
  y.d_.val_ = 1.0;

  fvar<fvar<var> > b = exp(y);
  EXPECT_FLOAT_EQ(exp(0.5), b.val_.val_.val());
  EXPECT_FLOAT_EQ(0, b.val_.d_.val());
  EXPECT_FLOAT_EQ(exp(0.5), b.d_.val_.val());
  EXPECT_FLOAT_EQ(0, b.d_.d_.val());


  AVEC q = createAVEC(y.val_.val_);
  VEC r;
  b.val_.val_.grad(q,r);
  EXPECT_FLOAT_EQ(exp(0.5), r[0]);
}

TEST(AgradFwdExp,FvarFvarVar_2ndDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using std::exp;

  fvar<fvar<var> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;

  fvar<fvar<var> > a = exp(x);

  EXPECT_FLOAT_EQ(exp(0.5), a.val_.val_.val());
  EXPECT_FLOAT_EQ(exp(0.5), a.val_.d_.val());
  EXPECT_FLOAT_EQ(0, a.d_.val_.val());
  EXPECT_FLOAT_EQ(0, a.d_.d_.val());

  AVEC p = createAVEC(x.val_.val_);
  VEC g;
  a.val_.d_.grad(p,g);
  EXPECT_FLOAT_EQ(exp(0.5), g[0]);

  fvar<fvar<var> > y;
  y.val_.val_ = 0.5;
  y.d_.val_ = 1.0;

  fvar<fvar<var> > b = exp(y);
  EXPECT_FLOAT_EQ(exp(0.5), b.val_.val_.val());
  EXPECT_FLOAT_EQ(0, b.val_.d_.val());
  EXPECT_FLOAT_EQ(exp(0.5), b.d_.val_.val());
  EXPECT_FLOAT_EQ(0, b.d_.d_.val());


  AVEC q = createAVEC(y.val_.val_);
  VEC r;
  b.d_.val_.grad(q,r);
  EXPECT_FLOAT_EQ(exp(0.5), r[0]);
}
TEST(AgradFwdExp,FvarFvarVar_3rdDeriv) {
  using stan::agrad::fvar;
  using stan::agrad::var;
  using std::exp;

  fvar<fvar<var> > x;
  x.val_.val_ = 0.5;
  x.val_.d_ = 1.0;
  x.d_.val_ = 1.0;

  fvar<fvar<var> > a = exp(x);

  AVEC p = createAVEC(x.val_.val_);
  VEC g;
  a.d_.d_.grad(p,g);
  EXPECT_FLOAT_EQ(exp(0.5), g[0]);
}

struct exp_fun {
  template <typename T0>
  inline T0
  operator()(const T0& arg1) const {
    return exp(arg1);
  }
};

TEST(AgradFwdExp,exp_NaN) {
  exp_fun exp_;
  test_nan(exp_,false);
}

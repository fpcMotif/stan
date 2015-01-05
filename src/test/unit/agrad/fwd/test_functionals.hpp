#ifndef AGRAD_FWD_TEST_FUNCTIONALS_HPP
#define AGRAD_FWD_TEST_FUNCTIONALS_HPP

#include <gtest/gtest.h>
#include <vector>
#include <stan/agrad/autodiff.hpp>

namespace stan {

  namespace test {

    template <typename F>
    struct unary_functor {
      const F& f_;

      unary_functor(const F& f) 
      : f_(f)  
      { }

      template <typename T>
      T operator()(const Eigen::Matrix<T,Eigen::Dynamic,1>& x) const {
        return f_(x(0));
      }
    };
    template <typename F>
    struct binary_functor {
      const F& f_;

      binary_functor(const F& f) 
      : f_(f)  
      { }

      template <typename T>
      T operator()(const Eigen::Matrix<T,Eigen::Dynamic,1>& x) const {
        return f_(x(0), x(1));
      }
    };


    template <typename F>
    struct bind1of2 {
      const F& f_;
      const double x1_;

      bind1of2(const F& f, double x1) 
        : f_(f), x1_(x1)
      { }

      template <typename T>
      T operator()(const T& x2) const {
        return f_(x1_, x2);
      }
    };
    template <typename F>
    struct bind2of2 {
      const F& f_;
      const double x2_;

      bind2of2(const F& f, double x2) 
        : f_(f), x2_(x2)
      { }

      template <typename T>
      T operator()(const T& x1) const {
        return f_(x1,x2_);
      }
    };



    template <typename F>
    void test_vector(const F& f, const Eigen::VectorXd& x) {
      using Eigen::VectorXd;
      using stan::agrad::fvar;
      using stan::agrad::gradient;
      using stan::agrad::var;

      // double
      double fx = f(x);

      // reverse
      double fx1;
      VectorXd grad_fx1;
      gradient<F>(f,x,fx1,grad_fx1);
      
      // forward
      double fx2;
      VectorXd grad_fx2;
      gradient<double,F>(f,x,fx2,grad_fx2);
      
      EXPECT_FLOAT_EQ(fx,fx1)
        << "value from double equals rev";

      EXPECT_FLOAT_EQ(fx,fx2)
        << "value from double equals fwd";

      EXPECT_FLOAT_EQ(x.size(), grad_fx1.size())
        << "size of gradient in rev";

      EXPECT_FLOAT_EQ(x.size(), grad_fx2.size())
        << "size of gradient in fwd";
      
      for (int i = 0; i < grad_fx1.size(); ++i)
        EXPECT_FLOAT_EQ(grad_fx1(i), grad_fx2(i))
          << "value of gradient in rev vs. fwd; index=" << i;
    }

    template <typename F>
    void test_unary(const F& f, double x) {
      Eigen::VectorXd x_vec(1);
      x_vec << x;
      test_vector(unary_functor<F>(f),x_vec);
    }
    
    template <typename F>
    void test_binary_vv(const F& f, double x1, double x2) {
      Eigen::VectorXd x_vec(2);
      x_vec << x1, x2;
      test_vector(binary_functor<F>(f),x_vec);
    }      
    template <typename F>
    void test_binary_vd(const F& f, double x1, double x2) {
      Eigen::VectorXd x1_vec(1);
      x1_vec << x1;
      test_vector(unary_functor<bind2of2<F> >(bind2of2<F>(f,x2)), x1_vec);
    }
    template <typename F>
    void test_binary_dv(const F& f, double x1, double x2) {
      Eigen::VectorXd x2_vec(1);
      x2_vec << x2;
      test_vector(unary_functor<bind1of2<F> >(bind1of2<F>(f,x1)), x2_vec);
    }

    template <typename F>
    void test_binary(const F& f, double x1, double x2) {
      test_binary_vv(f,x1,x2);
      test_binary_vd(f,x1,x2);
      test_binary_dv(f,x1,x2);
    }

    template <typename F>
    void test_binary_op_eq(const F& f, double x1, double x2) {
      test_binary_vv(f,x1,x2);
      test_binary_vd(f,x1,x2);
    }

    template <typename F>
    void test_binary_relation(const F&f, double x1, double x2) {
      using stan::agrad::fvar;
      using stan::agrad::var;

      EXPECT_EQ(f(x1,x2), f(var(x1),var(x2)))
        << "rev:  dd and vv match";
      EXPECT_EQ(f(x1,x2), f(var(x1),x2))
        << "rev:  dd and vd match";
      EXPECT_EQ(f(x1,x2), f(x1,var(x2)))
        << "rev:  dd and dv match";

      EXPECT_EQ(f(x1,x2), f(fvar<double>(x1),fvar<double>(x2)))
        << "fwd:  dd and vv match";
      EXPECT_EQ(f(x1,x2), f(fvar<double>(x1),x2))
        << "fwd:  dd and vd match";
      EXPECT_EQ(f(x1,x2), f(x1,fvar<double>(x2)))
        << "fwd:  dd and dv match";

      EXPECT_EQ(f(x1,x2), f(fvar<fvar<double> >(x1),fvar<fvar<double> >(x2)))
        << "fwd:  dd and vv match";
      EXPECT_EQ(f(x1,x2), f(fvar<fvar<double> >(x1),x2))
        << "fwd:  dd and vd match";
      EXPECT_EQ(f(x1,x2), f(x1,fvar<fvar<double> >(x2)))
        << "fwd:  dd and dv match";

      EXPECT_EQ(f(x1,x2), f(fvar<var>(x1),fvar<var>(x2)))
        << "fwd:  dd and vv match";
      EXPECT_EQ(f(x1,x2), f(fvar<var>(x1),x2))
        << "fwd:  dd and vd match";
      EXPECT_EQ(f(x1,x2), f(x1,fvar<var>(x2)))
        << "fwd:  dd and dv match";

      EXPECT_EQ(f(x1,x2), f(fvar<fvar<var> >(x1),fvar<fvar<var> >(x2)))
        << "fwd:  dd and vv match";
      EXPECT_EQ(f(x1,x2), f(fvar<fvar<var> >(x1),x2))
        << "fwd:  dd and vd match";
      EXPECT_EQ(f(x1,x2), f(x1,fvar<fvar<var> >(x2)))
        << "fwd:  dd and dv match";
    }

  }

}

#endif

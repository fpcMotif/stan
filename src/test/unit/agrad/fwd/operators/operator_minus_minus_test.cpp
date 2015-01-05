#include <test/unit/agrad/fwd/test_functionals.hpp>
#include <boost/math/tools/promotion.hpp>
#include <stan/agrad/fwd/fvar.hpp>

struct op_minus_minus_functor1 {
  template <typename T1>
  T1 operator()(const T1& x1) const {
    T1 x1_copy(x1);
    return x1_copy--;  // return value just x1
  }
};

struct op_minus_minus_functor2 {
  template <typename T1>
  T1 operator()(const T1& x1) const {
    T1 x1_copy(x1);
    x1_copy--;
    return x1_copy;   // returns x1 - 1
  }
};

TEST(AgradFwdOperatorMinusMinus, Before) {
  using stan::test::test_unary;
  op_minus_minus_functor1 f1;
  test_unary(f1, 1.4);
  test_unary(f1, 0.0);
  test_unary(f1, -12.2);
}
TEST(AgradFwdOperatorMinusMinus, After) {
  using stan::test::test_unary;
  op_minus_minus_functor2 f2;
  test_unary(f2, 1.4);
  test_unary(f2, 0.0);
  test_unary(f2, -12.2);
}

// PREFIX VERSIONS

struct op_minus_minus_prefix_functor1 {
  template <typename T1>
  T1 operator()(const T1& x1) const {
    T1 x1_copy(x1);
    return --x1_copy;  // return value just x1
  }
};

struct op_minus_minus_prefix_functor2 {
  template <typename T1>
  T1 operator()(const T1& x1) const {
    T1 x1_copy(x1);
    --x1_copy;
    return x1_copy;   // returns x1 - 1
  }
};

TEST(AgradFwdOperatorMinusMinusPrefix, Before) {
  using stan::test::test_unary;
  op_minus_minus_prefix_functor1 f1;
  test_unary(f1, 1.4);
  test_unary(f1, 0.0);
  test_unary(f1, -12.2);
}
TEST(AgradFwdOperatorMinusMinusPrefix, After) {
  using stan::test::test_unary;
  op_minus_minus_prefix_functor2 f2;
  test_unary(f2, 1.4);
  test_unary(f2, 0.0);
  test_unary(f2, -12.2);
}

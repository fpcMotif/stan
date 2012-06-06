#include <gtest/gtest.h>
#include <test/models/model_test_fixture.hpp>

class Models_BugsExamples_Vol3_Seeds : 
  public ::testing::Model_Test_Fixture<Models_BugsExamples_Vol3_Seeds,
                                       true> {
protected:
  virtual void SetUp() {}
public:
  static std::vector<std::string> get_model_path() {
    std::vector<std::string> model_path;
    model_path.push_back("models");
    model_path.push_back("bugs_examples");
    model_path.push_back("vol3");
    model_path.push_back("data_cloning");
    model_path.push_back("seeds");
    return model_path;
  }
};

TEST_F(Models_BugsExamples_Vol3_Seeds,RunModel) {
  run_model();
}
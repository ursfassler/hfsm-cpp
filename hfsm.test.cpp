#include "hfsm.hpp"
#include "lambda.hpp"

#include <gtest/gtest.h>


using namespace testing;

class HfsmTest :
    public Test
{
  public:
    typedef std::vector<std::string> T;

    T allFunction{};
    T transitions{};

    LambdaState<char> stateA{{}, {}, [this]{
        allFunction.push_back("↓A");
    }, [this]{
        allFunction.push_back("↑A");
    }};
    LambdaState<char> stateB{{}, {}, [this]{
        allFunction.push_back("↓B");
    }, [this]{
        allFunction.push_back("↑B");
    }};

    LambdaTransition<char> transitionAB0{stateA, stateB, [](char symbol){
        return symbol == 0;
      }, [this](int value){
        transitions.push_back("A -> B (" + std::to_string(value) + ")");
        allFunction.push_back("A→B");
      }};
    LambdaTransition<char> transitionAB1{stateA, stateB, [](char symbol){
        return symbol == 1;
      }, [this](int value){
        transitions.push_back("A -> B (" + std::to_string(value) + ")");
        allFunction.push_back("A→B");
      }};
    LambdaTransition<char> transitionBA0{stateB, stateA, [](char symbol){
        return symbol == 0;
      }, [this](int value){
        transitions.push_back("B -> A (" + std::to_string(value) + ")");
        allFunction.push_back("B→A");
      }};

    LambdaState<char> stateTop{{&stateA, &stateB}, {&transitionAB0, &transitionAB1, &transitionBA0}, [this]{
        allFunction.push_back("↓Top");
    }, [this]{
        allFunction.push_back("↑Top");
    }};

    Hfsm<char> testee{&stateA};

};


TEST_F(HfsmTest, runs_entry_functions_down_to_initial_leave_on_construction)
{
  EXPECT_EQ(T({"↓Top", "↓A"}), allFunction);
}

TEST_F(HfsmTest, take_first_transition_when_matching)
{
  testee.handle(0);

  EXPECT_EQ(T({"A -> B (0)"}), transitions);
}

TEST_F(HfsmTest, calls_exit_transition_and_entry_function)
{
  allFunction.clear();

  testee.handle(0);

  EXPECT_EQ(T({"↑A", "A→B", "↓B"}), allFunction);
}

TEST_F(HfsmTest, take_transition_from_active_state)
{
  testee.handle(0);
  transitions.clear();

  testee.handle(0);

  EXPECT_EQ(T{"B -> A (0)"}, transitions);
}

TEST_F(HfsmTest, take_first_matching_transition)
{
  testee.handle(1);

  EXPECT_EQ(T{"A -> B (1)"}, transitions);

}

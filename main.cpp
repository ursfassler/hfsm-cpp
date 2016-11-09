#include <cstddef>
#include <string>
#include <iostream>

#include "debug.hpp"
#include "hfsmprint.hpp"



class FlashState :
    public DebugState
{
  public:
    using DebugState::DebugState;

    void entry()
    {
      DebugState::entry();
      std::cout << "led on" << std::endl;
    }

    void exit()
    {
      std::cout << "led off" << std::endl;
      DebugState::exit();
    }

};

class WaitState :
    public DebugState
{
  public:
    using DebugState::DebugState;

    void entry()
    {
      DebugState::entry();
      counterValue = 1;
    }

    bool isZero() const
    {
      return counterValue == 0;
    }

    void decCounter()
    {
      if (counterValue != 0)
      {
        counterValue--;
      }
    }

  private:
    uint8_t counterValue{};

};

static const Event toggle{};
static const Event tick{};


class WaitTransition :
    public DebugTransition
{
  public:
    WaitTransition(DebugState& source_, WaitState &context_, DebugState& destination_, const Event *event_) :
      DebugTransition{source_, destination_, event_},
      contextState{context_}
    {
    }

    bool canHandle(const Event* event) const override
    {
      return DebugTransition::canHandle(event) && !contextState.isZero();
    }

    void execute(const Event* event) const override
    {
      DebugTransition::execute(event);
      contextState.decCounter();
    }

  private:
    WaitState &contextState;

};

int main()
{
  // construct states

  FlashState Flash{"Flash"};
  WaitState Wait{"Wait"};

  DebugState Off{"Off"};
  DebugState On{"On"};
  On.addState(&Flash);
  On.addState(&Wait);

  DebugState Top{"Top"};
  Top.addState(&Off);
  Top.addState(&On);

  // construct transitions

  const DebugTransition switchOn{Off, On, &toggle};
  const DebugTransition switchOff{On, Off, &toggle};
  Top.addTransition(&switchOn);
  Top.addTransition(&switchOff);

  const DebugTransition turnLedOff{Flash, Wait, &tick};
  const DebugTransition turnLedOn{Wait, Flash, &tick};
  On.addTransition(&turnLedOff);
  On.addTransition(&turnLedOn);

  const WaitTransition waitForZero{Wait, Wait, Wait, &tick};
  Wait.addTransition(&waitForZero);


  // print structure

  HfsmPrint printer{Top.initial()};

  printer.printStates(std::cout);
  printer.printTransitions(std::cout);

  std::cout << std::endl << std::endl;


  // run the state machine

  {
    Hfsm hfsm{Top.initial()};

    std::cout << "----" << std::endl;
    hfsm.handle(&tick);
    std::cout << "----" << std::endl;
    hfsm.handle(&toggle);
    std::cout << "----" << std::endl;
    hfsm.handle(&tick);
    std::cout << "----" << std::endl;
    hfsm.handle(&tick);
    std::cout << "----" << std::endl;
    hfsm.handle(&tick);
    std::cout << "----" << std::endl;
    hfsm.handle(&toggle);
    std::cout << "----" << std::endl;
    hfsm.handle(&tick);
    std::cout << "----" << std::endl;
    hfsm.handle(&toggle);
    std::cout << "----" << std::endl;
  }


  return 0;
}

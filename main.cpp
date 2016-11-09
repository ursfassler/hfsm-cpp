#include <cstddef>
#include <stdexcept>

//void* operator new(std::size_t)
//{
//  throw std::runtime_error("do not allocate memory");
//}

//void* operator new[](std::size_t)
//{
//  throw std::runtime_error("do not allocate memory");
//}

#include <cstdio>

#include "fullstatemachine.hpp"



class PrintTransition :
    public SimpleTransition
{
  public:
    PrintTransition(State& source_, State& context_, State& destination_, const Event* event_, const char* name_) :
      SimpleTransition{source_, context_, destination_, event_},
      name{name_}
    {
    }

    const char* name{""};

    void handle(const Event*) const override
    {
      printf("%s\n", name);
    }

};

class FlashState :
    public LeafState
{
  public:
    using LeafState::LeafState;

    void entry()
    {
      printf("%s\n", "led on");
    }

    void exit()
    {
      printf("%s\n", "led off");
    }

};

class WaitState :
    public LeafState
{
  public:
    using LeafState::LeafState;

    void entry()
    {
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
    public AbstractTransition
{
  public:
    WaitTransition(State& source_, WaitState &context_, State& destination_) :
      AbstractTransition{source_, context_, destination_},
      contextState{context_}
    {
    }

    bool canHandle(const Event* event) const override
    {
      return (event == &tick) && !contextState.isZero();
    }

    void handle(const Event*) const override
    {
      contextState.decCounter();
      printf("%s\n", "wait -> wait");
    }

  private:
    WaitState &contextState;

};

int main()
{
  FlashState Flash{};
  WaitState Wait{};

  LeafState Off{};
  CompositeState On{};
  On.addState(&Flash);
  On.addState(&Wait);

  Hfsm hfsm{};
  hfsm.addState(&Off);
  hfsm.addState(&On);


  const PrintTransition switchOn{Off, hfsm, On, &toggle, "off -> on"};
  const PrintTransition switchOff{On, hfsm, Off, &toggle, "on -> off"};

  const PrintTransition turnLedOff{Flash, On, Wait, &tick, "flash -> wait"};
  const PrintTransition turnLedOn{Wait, On, Flash, &tick, "wait -> flash"};

  const WaitTransition waitForZero{Wait, Wait, Wait};


  hfsm.initialize();

  hfsm.handle(&tick);
  hfsm.handle(&toggle);
  hfsm.handle(&tick);
  hfsm.handle(&tick);
  hfsm.handle(&tick);
  hfsm.handle(&toggle);
  hfsm.handle(&tick);


  return 0;
}

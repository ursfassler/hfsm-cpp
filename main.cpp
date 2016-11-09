#include <cstddef>
#include <stdexcept>

void* operator new(std::size_t)
{
  throw std::runtime_error("do not allocate memory");
}

void* operator new[](std::size_t)
{
  throw std::runtime_error("do not allocate memory");
}

#include <cstdio>

#include "statemachine.hpp"



const Transition* firstTransitionTopDown(State* state, const Event *event, const State* active)
{
  if (state == nullptr)
  {
    return nullptr;
  }

  const auto transition = firstTransitionTopDown(state->parent(), event, active);
  if (transition)
  {
    return transition;
  }

  return state->transitionFor(event, active);
}

State* handle(State* active, const Event *event)
{
  const auto transition = firstTransitionTopDown(active, event, active);
  if (transition)
  {
    transition->handle(event);
    return transition->destination->initial();
  }

  return active;
}

class PrintTransition :
    public Transition
{
  public:
    PrintTransition(State* source_, State* destination_, Event* event_, const char* name_) :
      Transition{source_, destination_, event_},
      name{name_}
    {
    }

    const char* name{""};

    void handle(const Event*) const override
    {
      printf("%s\n", name);
    }

};


int main()
{
  Event toggle{};
  Event tick{};

  LeafState<0> Off{};
  LeafState<0> Flash{};
  LeafState<0> Wait{};
  CompositeState<2, 2> On{};
  CompositeState<2, 2> Top{};

  const PrintTransition switchOn{&Off, &On, &toggle, "switch on"};
  const PrintTransition switchOff{&On, &Off, &toggle, "switch off"};
  Top.addTransition(&switchOn);
  Top.addTransition(&switchOff);

  const PrintTransition turnLedOff{&Flash, &Wait, &tick, "off"};
  const PrintTransition turnLedOn{&Wait, &Flash, &tick, "on"};
  On.addTransition(&turnLedOff);
  On.addTransition(&turnLedOn);

  On.addState(&Flash);
  On.addState(&Wait);

  Top.addState(&Off);
  Top.addState(&On);


  Top.initialize(nullptr);


  State* active = Top.initial();

  active = handle(active, &tick);
  active = handle(active, &toggle);
  active = handle(active, &tick);
  active = handle(active, &tick);
  active = handle(active, &tick);
  active = handle(active, &tick);
  active = handle(active, &toggle);


  return 0;
}

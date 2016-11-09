#include "statemachine.hpp"

#include <tuple>
#include <stdexcept>

AbstractTransition::AbstractTransition(State *source, State *context, State *destination) :
  sourceState{source},
  destinationState{destination}
{
  context->addTransition(this);
}

State *AbstractTransition::destination() const
{
  return destinationState;
}

State *AbstractTransition::source() const
{
  return sourceState;
}

void AbstractTransition::handle(const Event *) const
{
}






















SimpleTransition::SimpleTransition(State *source_, State* context, State *destination_, const Event *event_) :
  AbstractTransition{source_, context, destination_},
  contextState{context},
  event{event_}
{
}

bool SimpleTransition::canHandle(const Event *event) const
{
  return event == this->event;
}











std::pair<const Transition*, State*> firstTransitionTopDown(State* state, const Event *event, const State* active)
{
  if (state == nullptr)
  {
    return {};
  }

  const auto transition = state->transitionFor(event, active);
  if (transition)
  {
    return {transition, state};
  }

  return firstTransitionTopDown(state->parent(), event, active);

//  const auto transition = firstTransitionTopDown(state->parent(), event, active);
//  if (transition)
//  {
//    return transition;
//  }

//  return state->transitionFor(event, active);
}

State* handle(State* active, const Event *event)
{
  const auto result = firstTransitionTopDown(active, event, active);
  if (result.first)
  {
    const auto transition = result.first;
    const auto context = result.second;
    const auto source = active;
    const auto destination = transition->destination()->initial();

    for (State* itr = source; itr != context; itr = itr->parent())
    {
      itr->exit();
    }
    transition->handle(event);
    for (State* itr = destination; itr != context; itr = itr->parent())  // TODO fix order
    {
      itr->entry();
    }


    return destination;
  }

  return active;
}


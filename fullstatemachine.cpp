#include "fullstatemachine.hpp"

#include <tuple>
#include <stdexcept>

AbstractTransition::AbstractTransition(State &source, State &context, State &destination) :
  sourceState{source},
  destinationState{destination}
{
  context.addTransition(this);
}

State &AbstractTransition::destination() const
{
  return destinationState;
}

State &AbstractTransition::source() const
{
  return sourceState;
}

void AbstractTransition::handle(const Event *) const
{
}






















SimpleTransition::SimpleTransition(State &source_, State& context, State &destination_, const Event *event_) :
  AbstractTransition{source_, context, destination_},
  event{event_}
{
}

bool SimpleTransition::canHandle(const Event *event) const
{
  return event == this->event;
}











void BaseState::entry()
{
}

void BaseState::exit()
{
}

void BaseState::initialize(State *parent)
{
  parentState = parent;
}

const Transition *BaseState::transitionFor(const Event *event, const State *active)
{
  for (const Transition *itr : transition)
  {
    if (itr->source().isAncestorOf(active) && itr->canHandle(event))
    {
      return itr;
    }
  }

  return nullptr;
}

bool BaseState::isAncestorOf(const State *active)
{
  if (active == active->parent())
  {
    return false;
  }

  if (this == active)
  {
    return true;
  }

  return isAncestorOf(active->parent());
}

State *BaseState::parent() const
{
  return parentState;
}

void BaseState::addTransition(const Transition *item)
{
  transition.push_back(item);
}

void CompositeState::initialize(State *parent)
{
  BaseState::initialize(parent);
  for (auto state : substate)
  {
    state->initialize(this);
  }
}

State *CompositeState::initial()
{
  return substate.front()->initial();
}

void CompositeState::addState(State *item)
{
  substate.push_back(item);
}

State *LeafState::initial()
{
  return this;
}

Hfsm::Hfsm() :
  CompositeState()
{
}

void Hfsm::initialize()
{
  CompositeState::initialize(this);
  active = initial();
}

void Hfsm::handle(const Event *event)
{
  active = handle(active, event);
}

std::pair<const Transition *, State *> Hfsm::firstTransitionTopDown(State *state, const Event *event, const State *active)
{
  const auto transition = state->transitionFor(event, active);
  if (transition)
  {
    return {transition, state};
  }

  if (state == state->parent())
  {
    return {};
  }

  return firstTransitionTopDown(state->parent(), event, active);

  //  const auto transition = firstTransitionTopDown(state->parent(), event, active);
  //  if (transition)
  //  {
  //    return transition;
  //  }

  //  return state->transitionFor(event, active);
}

State *Hfsm::handle(State *active, const Event *event)
{
  const auto result = firstTransitionTopDown(active, event, active);
  if (result.first)
  {
    const auto transition = result.first;
    const auto context = result.second;
    const auto source = active;
    const auto destination = transition->destination().initial();

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

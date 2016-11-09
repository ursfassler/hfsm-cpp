#include "hfsm.hpp"


Hfsm::Hfsm(State *initial) :
  active{initial}
{
  callEntry(root(), active);
}

Hfsm::~Hfsm()
{
  callExit(root(), active);
}

void Hfsm::handle(const Event *event)
{
  active = handle(active, event);
}



static bool isAncestorOf(const State *state, const State *active)
{
  if (active == nullptr)
  {
    return false;
  }

  if (state == active)
  {
    return true;
  }

  return isAncestorOf(state, active->parent());
}

const Transition *transitionFor(const State *state, const State *active)
{
  for (std::size_t i = 0; i < state->transitionCount(); i++)
  {
    const Transition *itr = state->transition(i);
    if (isAncestorOf(&itr->source(), active))
    {
      return itr;
    }
  }

  return nullptr;
}

std::pair<const Transition *, State *> Hfsm::firstTransitionTopDown(State *state, const Event *event, const State *active)
{
  if (state == nullptr)
  {
    return {};
  }

  const auto transition = transitionFor(state, active);
  if (transition)
  {
    if (transition->canHandle(event))
    {
      return {transition, state};
    }
  }

  if (state == state->parent())
  {
    return {};
  }

  return firstTransitionTopDown(state->parent(), event, active);

  // this code is for fallback transition

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

    callExit(context, source);
    transition->execute(event);
    callEntry(context, destination);

    return destination;
  }

  return active;
}

State *Hfsm::root() const
{
  State* walker = active;

  while (walker->parent() != nullptr)
  {
    walker = walker->parent();
  }

  return walker;
}

void Hfsm::callEntry(State *top, State *bottom) const
{
  if (top != bottom)
  {
    callEntry(top, bottom->parent());
    bottom->entry();
  }
}

void Hfsm::callExit(State *top, State *bottom) const
{
  if (top != bottom)
  {
    bottom->exit();
    callExit(top, bottom->parent());
  }
}


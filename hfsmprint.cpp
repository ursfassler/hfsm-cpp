#include "hfsmprint.hpp"

HfsmPrint::HfsmPrint(State *initial)
{
  addAll(initial);
}

void HfsmPrint::printStates(std::ostream &stream)
{
  for (State* state : states)
  {
    stream << dynamic_cast<DebugState*>(state)->name();
    if (state->parent() != nullptr)
    {
      stream << " (";
      stream << dynamic_cast<DebugState*>(state->parent())->name();
      stream << ")";
    }
    stream << std::endl;
  }
}

void HfsmPrint::printTransitions(std::ostream &stream)
{
  for (const Transition* transition : transitions)
  {
    stream << dynamic_cast<DebugState*>(&transition->source())->name();
    stream << " -> ";
    stream << dynamic_cast<DebugState*>(&transition->destination())->name();
    stream << std::endl;
  }
}

void HfsmPrint::addAll(State *state)
{
  for (State* itr = state->initial(); itr; itr = itr->parent())
  {
    add(itr);
  }
}

void HfsmPrint::add(State *state)
{
  if (states.find(state) == states.end())
  {
    states.insert(state);
    for (std::size_t i = 0; i < state->transitionCount(); i++)
    {
      const Transition* transition = state->transition(i);
      transitions.insert(transition);
      addAll(&transition->source());
      addAll(&transition->destination());
    }
  }
}

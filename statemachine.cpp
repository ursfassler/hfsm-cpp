#include "statemachine.hpp"

#include <stdexcept>

Transition::Transition(State *source_, State *destination_, Event *event_) :
  source{source_},
  destination{destination_},
  event{event_}
{
}

bool Transition::canHandle(const Event *event) const
{
  return event == this->event;
}

void Transition::handle(const Event *) const
{
}


















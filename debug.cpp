#include "debug.hpp"


DebugState::DebugState(const std::string &name) :
  stateName{name}
{
}

const std::string &DebugState::name() const
{
  return stateName;
}

void DebugState::entry()
{
  std::cout << "↓" << stateName << std::endl;
}

void DebugState::exit()
{
  std::cout << "↑" << stateName << std::endl;
}

void DebugTransition::execute(const Event *) const
{
  std::cout << source().name() << "→" << destination().name() << std::endl;
}

#pragma once

#include "simple.hpp"

#include <string>
#include <iostream>


class DebugState;
class DebugTransition;


class DebugState :
    public SimpleGenericState<DebugTransition>
{
  public:
    DebugState(const std::string& name);

    const std::string &name() const;
    void entry() override;
    void exit() override;

  private:
    std::string stateName{};

};


class DebugTransition :
    public SimpleGenericTransition<DebugState>
{
  public:
    using SimpleGenericTransition<DebugState>::SimpleGenericTransition;

    void execute(const Event*) const override;

};


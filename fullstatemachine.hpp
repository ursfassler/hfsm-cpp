#pragma once

#include <vector>

class Event
{
};

class Transition;

class State
{
  public:
    virtual ~State() = default;

    virtual void initialize(State* parent) = 0;
    virtual State* initial() = 0;
    virtual State* parent() const = 0;
    virtual bool isAncestorOf(const State* active) = 0;
    virtual const Transition* transitionFor(const Event* event, const State *active) = 0;
    virtual void addTransition(const Transition* item) = 0;

    virtual void entry() = 0;
    virtual void exit() = 0;

};

class Transition
{
  public:
    virtual ~Transition() = default;

    virtual State& source() const = 0;
    virtual State& destination() const = 0;
    virtual bool canHandle(const Event* event) const = 0;
    virtual void handle(const Event* event) const = 0;

};

class AbstractTransition :
    public Transition
{
  public:
    AbstractTransition(State& source, State& context, State& destination);

    State &source() const override;
    State &destination() const override;

    void handle(const Event* event) const override;

  private:
    State& sourceState;
    State& destinationState;

};

class SimpleTransition :
    public AbstractTransition
{
  public:
    SimpleTransition(State &source_, State &context, State &destination_, const Event* event_);

    bool canHandle(const Event* event) const override;

  private:
    const Event* event{};

};


class BaseState :
    public State
{
  public:
    void entry() override;
    void exit() override;

    void initialize(State* parent) override;
    const Transition* transitionFor(const Event* event, const State *active) override;
    bool isAncestorOf(const State* active) override;
    State *parent() const override;
    void addTransition(const Transition* item) override;

  private:
    State* parentState;
    std::vector<const Transition*> transition{};

};

class CompositeState :
    public BaseState
{
  public:
    using BaseState::BaseState;

    void initialize(State* parent) override;
    State* initial() override;
    void addState(State* item);

  private:
    std::vector<State*> substate{};

};


class LeafState :
    public BaseState
{
  public:
    using BaseState::BaseState;

    State *initial() override;

};

State* handle(State* active, const Event *event);


class Hfsm :
    public CompositeState
{
  public:
    Hfsm();

    void initialize();
    void handle(const Event* event);

  private:
    State* active{};



    std::pair<const Transition*, State*> firstTransitionTopDown(State* state, const Event *event, const State* active);

    State* handle(State* active, const Event *event);


};

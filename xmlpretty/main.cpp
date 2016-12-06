#include "../simple.hpp"

#include <string>
#include <sstream>
#include <iostream>
#include <functional>

enum class Token
{
  Open,
  Close,
  End,
  Identifier,
  Whitespace,
};

bool isStartName(char value)
{
  return (value == ':') or
      (value == '_') or
      (('a' <= value) and (value <= 'z')) or
      (('A' <= value) and (value <= 'Z'));
}


class LambdaTransition :
    public Transition<char>
{
  public:
    typedef std::function<void(char)> OnEvent;
    typedef std::function<bool(char)> CanHandleEvent;

    LambdaTransition(State<char>& source, State<char>& destination, CanHandleEvent canHandleEvent_, OnEvent onEvent_) :
      sourceState{source},
      destinationState{destination},
      canHandleEvent{canHandleEvent_},
      onEvent{onEvent_}
    {
    }

    State<char> &source() const override
    {
      return sourceState;
    }

    State<char> &destination() const override
    {
      return destinationState;
    }

    bool canHandle(const char& event) const override
    {
      return isStartName(event);
    }

    void execute(const char& event) const override
    {
      onEvent(event);
    }

  private:
    State<char>& sourceState;
    State<char>& destinationState;
    CanHandleEvent canHandleEvent;
    OnEvent onEvent;

};


class LambdaState :
    public State<char>
{
  public:
    typedef std::function<void()> OnActivityChange;


    LambdaState(OnActivityChange onEntry_, OnActivityChange onExit_) :
      onEntry{onEntry_},
      onExit{onExit_}
    {
    }

    void entry() override
    {
      onEntry();
    }

    void exit() override
    {
      onExit();
    }

    std::size_t transitionCount() const override
    {
      return transitions.size();
    }

    const Transition<char> *transition(std::size_t index) const override
    {
      return transitions[index];
    }

    void setParent(State<char>* value)
    {
      parentState = value;
    }

    State<char> *parent() const override
    {
      return parentState;
    }

    void addTransition(const Transition<char>* item)
    {
      transitions.push_back(item);
    }

    State<char>* initial() override
    {
      return initialState;
    }

    void addState(LambdaState *item)
    {
      if (initialState == this)
      {
        initialState = item;
      }
      item->setParent(this);
    }

  private:
    OnActivityChange onEntry;
    OnActivityChange onExit;
    State<char>* parentState{nullptr};
    State<char>* initialState{this};
    std::vector<const Transition<char>*> transitions{};

};



int main()
{
  std::string id{};

  LambdaState Top{[]{}, []{}};
  LambdaState Symbol{[]{}, []{}};
  LambdaState Identifier{[]{}, [id]{
      std::cout << id << std::endl;
    }};
  Top.addState(&Symbol);
  Top.addState(&Identifier);

  LambdaTransition startName{Symbol, Identifier, isStartName, [&id](char symbol){
      id = symbol;
    }};
  LambdaTransition inName{Identifier, Identifier, isStartName, [&id](char symbol){
      id += symbol;
    }};
  LambdaTransition nameEnd{Identifier, Symbol, [](char){
      return true;
    }, [](char){
    }};
  Top.addTransition(&startName);
  Identifier.addTransition(&inName);
  Top.addTransition(&nameEnd);

  Hfsm<char> tokenizer{&Top};





  const std::string content{"<root><hello  />  <world></world></root>"};
  std::stringstream stream{content};

  while (true)
  {
    const int symbol = stream.get();
    if (!stream.good())
    {
      break;
    }
    tokenizer.handle(symbol);
  }

  std::cout << std::endl;

  return 0;
}

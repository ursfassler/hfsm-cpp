#pragma once

#include "types.hpp"

#include <utility>



template<typename E>
class Hfsm final
{
  public:
    Hfsm(State<E>* initial) :
      active{initial->initial()}
    {
      callEntry(root(), active);
    }

    ~Hfsm()
    {
      callExit(root(), active);
    }

    void handle(const E& event)
    {
      active = handle(active, event);
    }

  private:
    State<E>* active{};

    static bool isAncestorOf(const State<E> *state, const State<E> *active)
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

    static const Transition<E> *transitionFor(const State<E> *state, const State<E> *active, const E& event)
    {
      for (std::size_t i = 0; i < state->transitionCount(); i++)
      {
        const Transition<E> *itr = state->transition(i);
        if (isAncestorOf(&itr->source(), active))
        {
          if (itr->canHandle(event))
          {
            return itr;
          }
        }
      }

      return nullptr;
    }

    std::pair<const Transition<E>*, State<E>*> firstTransitionTopDown(State<E>* state, const E& event, const State<E>* active)
    {
      if (state == nullptr)
      {
        return {};
      }

      const auto transition = transitionFor(state, active, event);
      if (transition)
      {
        return {transition, state};
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

    State<E>* handle(State<E>* active, const E& event)
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

    State<E> *root() const
    {
      State<E>* walker = active;

      while (walker->parent() != nullptr)
      {
        walker = walker->parent();
      }

      return walker;
    }

    void callEntry(State<E>* top, State<E>* bottom) const
    {
      if (top != bottom)
      {
        callEntry(top, bottom->parent());
        bottom->entry();
      }
    }

    void callExit(State<E>* top, State<E>* bottom) const
    {
      if (top != bottom)
      {
        bottom->exit();
        callExit(top, bottom->parent());
      }
    }


};

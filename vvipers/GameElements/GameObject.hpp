#pragma once

namespace VVipers {

/** The main purpose of this class is to make sure all objects inherit from one
 * common virtual class to make typeinfo work properly **/
class Object {
  public:
    // Must have at least one virtual memeber to function
    virtual ~Object() {}
};

class GameObject : public Object {
  public:
    enum ObjectState { Alive, Dying, Dead };

    GameObject() : _state(Alive) {}
    virtual ~GameObject() {}

    ObjectState state() const { return _state; }
    void state(ObjectState state);

  protected:
    virtual void state_changed(ObjectState from, ObjectState into) {}

  private:
    ObjectState _state;
};

}  // namespace VVipers

#ifndef VVIPERS_GAMEOBJECT_HPP
#define VVIPERS_GAMEOBJECT_HPP

namespace VVipers {

/** The main purpose of this calss is to make sure all objects inherit from one
 * common virtual class to make typeinfo work properly **/
class GameObject {
  public:
    enum ObjectState { Alive, Dying, Dead };

    GameObject() : m_state(Alive) {}
    virtual ~GameObject() {}

    ObjectState state() const { return m_state; }
    void state(ObjectState state);

  protected:
    virtual void stateChanged(ObjectState from, ObjectState into) {}

  private:
    ObjectState m_state;
};

}  // namespace VVipers

#endif  // VVIPERS_GAMEOBJECT_HPP

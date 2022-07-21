#ifndef VVIPERS_GAMEOBJECT_HPP
#define VVIPERS_GAMEOBJECT_HPP

namespace VVipers {

/** The main purpose of this calss is to make sure all objects inherit from one
 * common virtual class to make typeinfo work properly **/
class GameObject {
  public:
    virtual ~GameObject() {}
};

}  // namespace VVipers

#endif  // VVIPERS_GAMEOBJECT_HPP

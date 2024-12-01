#include <vvipers/GameElements/GameObject.hpp>

namespace VVipers
{

void GameObject::state( ObjectState state ){
    if( state != _state ){
        ObjectState old = _state;
        _state = state;
        state_changed( old, _state );
    }
}

} // namespace VVipers
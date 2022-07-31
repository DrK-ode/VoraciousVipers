#include <vvipers/Scenes/GameElements/GameObject.hpp>

namespace VVipers
{
    
void GameObject::state( ObjectState state ){
    if( state != m_state ){
        ObjectState old = m_state;
        m_state = state;
        stateChanged( old, m_state );
    }
}

} // namespace VVipers
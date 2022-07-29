#include <vvipers/Services.hpp>

namespace VVipers {

Services* Services::s_instance(nullptr);

Services::Services()
    : m_fontProvider(nullptr),
      m_optionsProvider(nullptr),
      m_textureProvider(nullptr) {
    if (s_instance)
        throw std::runtime_error("Only one instance of GameOptions allowed.");
    s_instance = this;
}

Services* Services::getInstance() {
    if (s_instance)
        return s_instance;
    throw std::runtime_error("Services not instantiated.");
}

}  // namespace VVipers

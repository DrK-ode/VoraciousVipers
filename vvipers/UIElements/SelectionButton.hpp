#pragma once

#include "vvipers/UIElements/MenuButton.hpp"

namespace VVipers {

template<typename T> class SelectionButton : public MenuButton {
  public:
    SelectionButton(const std::string& pre_label, const std::vector<T>& options);
    void on_event(const sf::Event& event) override;
    T selected_option() const {return _options[_selected_option];}
    void set_selected_option(size_t);

  private:
    void update_label();
    const std::string _pre_label;
    std::vector<T> _options;
    size_t _selected_option;
};
}  // namespace VVipers
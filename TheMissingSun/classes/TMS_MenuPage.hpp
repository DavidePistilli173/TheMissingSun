#ifndef TMS_MENUPAGE_HPP
#define TMS_MENUPAGE_HPP

#include <memory>
#include <string>
#include <vector>

#include "TMS_Button.hpp"

/* Single page of the menu. */
class TMS_MenuPage
{
public:
    /* Association of button and destination page. */
    struct Link
    {
        TMS_Button button;
        std::shared_ptr<TMS_MenuPage> link;
    };

    TMS_MenuPage();

    /**************** METHODS ****************/
    /* Setters. */
    /* Change the page id if it has not been set previously. */
    void setId(const unsigned int id)
    {
        if (_id < 0) _id = id;
    }
    void setTitle(const std::string title) { _title = title; }
    /* Getters. */
    int getId() const { return _id; }
    std::string getTitle() const { return _title; }
    /* Return a reference to the button list. */
    std::vector<Link>& getButtons()
    {
        return _buttons;
    }
    /* Add a button to the page. */
    void addButton(TMS_Button& button, std::shared_ptr<TMS_MenuPage> link)
    {
        Link newButton;
        newButton.button = std::move(button);
        newButton.link = link;
        _buttons.push_back(std::move(newButton));
    }

private:
    int _id; // Page id.
    std::string _title; // Page title. 
    std::vector<Link> _buttons; // Buttons on the page.
};

#endif
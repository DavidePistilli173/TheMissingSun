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
    /* Render the current page. */
    void render();
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
    /* Add a button to the page. */
    void addButton(const TMS_Button &button, const std::shared_ptr<TMS_MenuPage> &link)
    {
        _buttons.push_back({ button, link });
    }

private:
    int _id; // Page id.
    std::string _title; // Page title. 
    std::vector<Link> _buttons; // Buttons on the page.
};

#endif
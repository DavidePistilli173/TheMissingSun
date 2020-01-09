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
        /**************** CONSTRUCTORS ****************/
        Link() {}; // Empty constructor.
        Link(TMS_Button&& b, std::shared_ptr<TMS_MenuPage> l) :
            button(std::move(b)),
            link(l)
        {};
        ~Link() = default;
        /* Disable copy. */
        Link(const Link& oldLink) = delete;
        Link& operator= (const Link& oldLink) = delete;
        /* Move constructors. */
        Link(Link&& oldLink) noexcept
        {
            this->button = std::move(oldLink.button);
            this->link = oldLink.link;
        }
        Link& operator= (Link&& oldLink) noexcept
        {
            if (this != &oldLink)
            {
                this->button = std::move(oldLink.button);
                this->link = oldLink.link;
            }
        }

        /**************** VARIABLES ****************/
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
        Link newButton(std::move(button), link);
        _buttons.push_back(std::move(newButton));
    }

private:
    int _id; // Page id.
    std::string _title; // Page title. 
    std::vector<Link> _buttons; // Buttons on the page.
};

#endif
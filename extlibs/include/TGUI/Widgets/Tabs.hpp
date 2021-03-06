/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2017 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef TGUI_TAB_HPP
#define TGUI_TAB_HPP


#include <TGUI/Widget.hpp>
#include <TGUI/Renderers/TabsRenderer.hpp>
#include <TGUI/Text.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Tabs widget
    ///
    /// Signals:
    ///     - TabSelected (Another tab has been selected)
    ///         * Optional parameter sf::String: New text on the newly selected tab
    ///         * Uses Callback member 'text'
    ///
    ///     - Inherited signals from Widget
    ///
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TGUI_API Tabs : public Widget
    {
    public:

        typedef std::shared_ptr<Tabs> Ptr; ///< Shared widget pointer
        typedef std::shared_ptr<const Tabs> ConstPtr; ///< Shared constant widget pointer


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Default constructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Tabs();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Creates a new tabs widget
        ///
        /// @return The new tabs
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static Tabs::Ptr create();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Makes a copy of another tab
        ///
        /// @param tab  The other tab
        ///
        /// @return The new tab
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static Tabs::Ptr copy(Tabs::ConstPtr tab);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the renderer, which gives access to functions that determine how the widget is displayed
        ///
        /// @return Temporary pointer to the renderer
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        TabsRenderer* getRenderer() const
        {
            return aurora::downcast<TabsRenderer*>(m_renderer.get());
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief This function currently does nothing
        ///
        /// @param size  Currently ignored
        ///
        /// It is not yet possible to change the size directly.
        ///
        /// @see setTabHeight
        /// @see setDistanceToSide
        /// @see setMaximumTabWidth
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setSize(const Layout2d& size) override;
        using Transformable::setSize;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the size of the tabs
        ///
        /// @return Size of the tabs
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual sf::Vector2f getSize() const override
        {
            return {m_width, m_tabHeight};
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Adds a new tab
        ///
        /// @param text    The text of the tab that will be drawn on top of it
        /// @param select  Do you want the new tab to be selected immediately?
        ///
        /// @return  The index of the tab in the list
        ///
        /// @warning The index returned by this function may no longer be correct when a tab is removed
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::size_t add(const sf::String& text, bool select = true);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Inserts a new tab somewhere between the other tabs
        ///
        /// @param index   The index where the tab gets inserted. 0 means before the first tab, 1 means behind the first tab
        /// @param text    The text of the tab that will be drawn on top of it
        /// @param select  Do you want the new tab to be selected immediately?
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void insert(std::size_t index, const sf::String& text, bool select = true);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Gets the text of one of the tabs
        ///
        /// @param index  The index of the tab. The first tab has index 0
        ///
        /// @return The text on the tab or an empty string when index was too high
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        sf::String getText(std::size_t index) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the text of one of the tabs
        ///
        /// @param index  The index of the tab to be changed. The first tab has index 0
        /// @param text   The new text of the tab that will be drawn on top of it
        ///
        /// @return True when text was successfully changed, false when index was too high
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool changeText(std::size_t index, const sf::String& text);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Selects the tab with a given text
        ///
        /// @param text  The text of the tab to select
        ///
        /// When the text doen't match any tab then nothing will be changed.
        /// If there are multiple tabs with the same text then the first one will be selected.
        ///
        /// @see select(int)
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void select(const sf::String& text);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Selects the tab with a given index
        ///
        /// @param index  The index of the tab to select
        ///
        /// When the index is too high then nothing will happen.
        ///
        /// @see select(sf::String)
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void select(std::size_t index);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Deselects the selected tab
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void deselect();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Removes a tab with a given text
        ///
        /// @param text  The text on the tab to remove
        ///
        /// When multiple tabs have the same text, only the first will be removed.
        ///
        /// @see remove(unsigned int)
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void remove(const sf::String& text);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Removes a tab with a given index
        ///
        /// @param index  The index of the tab to remove
        ///
        /// When the index is too high then nothing will happen.
        ///
        /// @see remove(sf::String)
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void remove(std::size_t index);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Removes all tabs
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void removeAll();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Gets the text that is drawn on the currently selected tab
        ///
        /// @return The text on the tab.
        ///         When no tab is selected then this function returns an empty string
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        sf::String getSelected() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Gets the index of the currently selected tab
        ///
        /// @return The index of the tab.
        ///         When no tab is selected then this function returns -1
        ///
        /// @warning The index returned by this function may no longer be correct when a tab is removed
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int getSelectedIndex() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the character size of the text
        ///
        /// @param size  The new size of the text
        ///              If the size is 0 (default) then the text will be scaled to fit in the tab.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setTextSize(unsigned int size);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the character size of the text
        ///
        /// @return The text size
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        unsigned int getTextSize() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the height of the tabs
        ///
        /// @param height  Height of the tabs
        ///
        /// By default, it is the height of the tab image that is loaded with the load function.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setTabHeight(float height);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the height of the tabs
        ///
        /// @return Tab height
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        float getTabHeight() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Changes the maximum tab width of the tabs
        ///
        /// @param maximumWidth  Maximum width of a single tab
        ///
        /// If the text on the tab is longer than this width then it will be cropped to fit inside the tab.
        /// By default, the maximum width is 0 which means that there is no limitation.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void setMaximumTabWidth(float maximumWidth);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the maximum tab width of the tabs
        ///
        /// @return Maximum tab width
        ///
        /// If the text on the tab is longer than this width then it will be cropped to fit inside the tab.
        /// By default, the maximum width is 0 which means that there is no limitation.
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        float getMaximumTabWidth() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the amount of tabs
        ///
        /// @return Number of tabs
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::size_t getTabsCount() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual bool mouseOnWidget(sf::Vector2f pos) const override;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @internal
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void leftMousePressed(sf::Vector2f pos) override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Draw the widget to a render target
        ///
        /// @param target Render target to draw to
        /// @param states Current render states
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Recalculates the size of each tab image.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void recalculateTabsWidth();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Function called when one of the properties of the renderer is changed
        ///
        /// @param property  Lowercase name of the property that was changed
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void rendererChanged(const std::string& property) override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Makes a copy of the widget
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual Widget::Ptr clone() const override
        {
            return std::make_shared<Tabs>(*this);
        }


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:
        unsigned int m_requestedTextSize = 0;
        unsigned int m_textSize = 22;

        float              m_maximumTabWidth = 0;
        int                m_selectedTab = -1;

        float              m_width = 0;
        float              m_tabHeight = 0;
        std::vector<float> m_tabWidth;

        std::vector<Text>  m_tabTexts;

        Sprite m_spriteTab;
        Sprite m_spriteSelectedTab;

        // Cached renderer properties
        Borders   m_bordersCached;
        Color     m_borderColorCached;
        Color     m_backgroundColorCached;
        Color     m_selectedBackgroundColorCached;
        Color     m_textColorCached;
        Color     m_selectedTextColorCached;
        float     m_distanceToSideCached = 0;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_TAB_HPP

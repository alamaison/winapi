/**
    @file

    Menu item implementation details.

    @if license

    Copyright (C) 2012  Alexander Lamaison <awl03@doc.ic.ac.uk>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    If you modify this Program, or any covered work, by linking or
    combining it with the OpenSSL project's OpenSSL library (or a
    modified version of that library), containing parts covered by the
    terms of the OpenSSL or SSLeay licenses, the licensors of this
    Program grant you additional permission to convey the resulting work.

    @endif
*/

#ifndef WASHER_GUI_MENU_DETAIL_ITEM_COMMON_HPP
#define WASHER_GUI_MENU_DETAIL_ITEM_COMMON_HPP
#pragma once

#include <washer/gui/menu/button/button.hpp>
#include <washer/gui/menu/detail/item_position.hpp>
#include <washer/gui/menu/item/item_state.hpp> // selectability, check_mark,
                                               // highlighting

#include <boost/shared_ptr.hpp>

#include <Windows.h> // MENUITEMINFO

namespace washer {
namespace gui {
namespace menu {
namespace detail {

class selectable_item_core
{
public:

    explicit selectable_item_core(const item_position& item) : m_item(item) {}

    /**
     * The ID of the item.
     *
     * Used to find an item in a menu even if its position changes.
     */
    UINT id() const
    {
        return m_item.get_menuiteminfo(MIIM_ID).wID;
    }

    void button_state(BOOST_SCOPED_ENUM(selectability) state)
    {
        MENUITEMINFOW info = m_item.get_menuiteminfo(MIIM_STATE);
        detail::adjust_selectability(state, info);
        m_item.set_menuiteminfo(info);
    }

    void check_mark(BOOST_SCOPED_ENUM(check_mark) state)
    {
        MENUITEMINFOW info = m_item.get_menuiteminfo(MIIM_STATE);
        detail::adjust_checkedness(state, info);
        m_item.set_menuiteminfo(info);
    }

    ::washer::gui::menu::button button() const
    {
        return ::washer::gui::menu::button(m_item);
    }

    bool is_selectable() const
    {
        return !(m_item.get_menuiteminfo(MIIM_STATE).fState & MFS_DISABLED);
    }

    bool check_mark_is_visible() const
    {
        return (m_item.get_menuiteminfo(MIIM_STATE).fState & MFS_CHECKED) != 0;
    }

    bool is_highlighted() const
    {
        return (m_item.get_menuiteminfo(MIIM_STATE).fState & MFS_HILITE) != 0;
    }

    MENUITEMINFOW get_menuiteminfo(UINT mask) const
    {
        return m_item.get_menuiteminfo(mask);
    }

private:

    item_position m_item;
};


}}}} // namespace washer::gui::menu::detail

#endif

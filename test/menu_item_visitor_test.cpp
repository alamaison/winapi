/**
    @file

    Tests for menu item visitor.

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

#include "menu_fixtures.hpp"
#include "wchar_output.hpp" // wchar_t test output

#include <washer/gui/menu/item/command_item.hpp>
#include <washer/gui/menu/item/command_item_description.hpp>
#include <washer/gui/menu/item/item.hpp> // test subject
#include <washer/gui/menu/item/separator_item.hpp>
#include <washer/gui/menu/item/separator_item_description.hpp>
#include <washer/gui/menu/item/sub_menu_item.hpp>
#include <washer/gui/menu/item/sub_menu_item_description.hpp>
#include <washer/gui/menu/visitor.hpp> // test subject

#include <boost/detail/scoped_enum_emulation.hpp> // BOOST_SCOPED_ENUM
#include <boost/test/unit_test.hpp>

#include <string>

using namespace washer::gui::menu;
using washer::test::menu_ownership_fixtures;
using washer::test::menu_with_handle_creator_fixtures;

using std::string;

/**
 * These tests only exist to ensure the correct visitor overload gets fired.
 */
BOOST_AUTO_TEST_SUITE( menu_item_visitor_tests )

BOOST_SCOPED_ENUM_START(expected_case)
{
    separator,
    command,
    sub,
    const_separator,
    const_command,
    const_sub
};
BOOST_SCOPED_ENUM_END;

class test_visitor : public menu_visitor<string>
{
public:

    test_visitor(BOOST_SCOPED_ENUM(expected_case) c) : m_case(c) {}

    string operator()(separator_item&)
    {
        BOOST_REQUIRE(m_case == expected_case::separator);
        return "separator";
    }

    string operator()(command_item&)
    {
        BOOST_REQUIRE(m_case == expected_case::command);
        return "command";
    }

    string operator()(sub_menu_item&)
    {
        BOOST_REQUIRE(m_case == expected_case::sub);
        return "sub_menu";
    }

private:

    BOOST_SCOPED_ENUM(expected_case) m_case;
};

class void_test_visitor : public menu_visitor<>
{
public:

    void_test_visitor(BOOST_SCOPED_ENUM(expected_case) c) : m_case(c) {}

    void operator()(separator_item&)
    {
        BOOST_REQUIRE(m_case == expected_case::separator);
    }

    void operator()(command_item&)
    {
        BOOST_REQUIRE(m_case == expected_case::command);
    }

    void operator()(sub_menu_item&)
    {
        BOOST_REQUIRE(m_case == expected_case::sub);
    }

private:

    BOOST_SCOPED_ENUM(expected_case) m_case;
};

class const_void_test_visitor : public menu_visitor<>
{
public:

    const_void_test_visitor(BOOST_SCOPED_ENUM(expected_case) c)
        : m_case(c) {}

    void operator()(const separator_item&)
    {
        BOOST_REQUIRE(m_case == expected_case::const_separator);
    }

    void operator()(const command_item&)
    {
        BOOST_REQUIRE(m_case == expected_case::const_command);
    }

    void operator()(const sub_menu_item&)
    {
        BOOST_REQUIRE(m_case == expected_case::const_sub);
    }

private:

    BOOST_SCOPED_ENUM(expected_case) m_case;
};

class const_and_nonconst_void_test_visitor : public menu_visitor<>
{
public:

    const_and_nonconst_void_test_visitor(
        BOOST_SCOPED_ENUM(expected_case) c) : m_case(c) {}

    void operator()(separator_item&)
    {
        BOOST_REQUIRE(m_case == expected_case::separator);
    }

    void operator()(command_item&)
    {
        BOOST_REQUIRE(m_case == expected_case::command);
    }

    void operator()(sub_menu_item&)
    {
        BOOST_REQUIRE(m_case == expected_case::sub);
    }

    void operator()(const separator_item&)
    {
        BOOST_REQUIRE(m_case == expected_case::const_separator);
    }

    void operator()(const command_item&)
    {
        BOOST_REQUIRE(m_case == expected_case::const_command);
    }

    void operator()(const sub_menu_item&)
    {
        BOOST_REQUIRE(m_case == expected_case::const_sub);
    }

private:

    BOOST_SCOPED_ENUM(expected_case) m_case;
};

class templated_test_visitor : public menu_visitor<string>
{
public:

    template<typename T>
    string operator()(T&)
    {
        return "catch-all";
    }
};

/**
 * Tests that the inheritance hierachy we've set up to separate selectable
 * from non-selectable menu items, actually works.
 */
class semi_inheritance_test_visitor : public menu_visitor<string>
{
public:

    semi_inheritance_test_visitor(BOOST_SCOPED_ENUM(expected_case) c)
        : m_case(c) {}

    string operator()(separator_item&)
    {
        BOOST_REQUIRE(m_case == expected_case::separator);
        return "separator";
    }

    string operator()(selectable_item&)
    {
        BOOST_REQUIRE(
            m_case == expected_case::command || m_case == expected_case::sub);
        return "command-or-sub_menu";
    }

private:

    BOOST_SCOPED_ENUM(expected_case) m_case;
};

BOOST_AUTO_TEST_CASE( visit_separator )
{
    menu m;
    m.insert(separator_item_description());

    BOOST_CHECK_EQUAL(
        m[0].accept(test_visitor(expected_case::separator)),
        "separator");

    m[0].accept(void_test_visitor(expected_case::separator));

    BOOST_CHECK_EQUAL(m[0].accept(templated_test_visitor()), "catch-all");

    BOOST_CHECK_EQUAL(
        m[0].accept(semi_inheritance_test_visitor(expected_case::separator)),
        "separator");
}

BOOST_AUTO_TEST_CASE( visit_separator_const_correctness )
{
    menu m;
    m.insert(separator_item_description());

    const item ci = m[0];

    // Const items should accept a visitor that only gives access to const items
    ci.accept(const_void_test_visitor(expected_case::const_separator));

    // Const items should not accept a non-const visitor so the following
    // should cause a compile error
    //ci.accept(void_test_visitor(expected_case::separator));

    // It should accept a visitor with both const and non-const items but only
    // fire the methods giving access to the const items
    ci.accept(
        const_and_nonconst_void_test_visitor(expected_case::const_separator));

    // The non-const items should be equally happy with a const-only visitor
    m[0].accept(const_void_test_visitor(expected_case::const_separator));
}

BOOST_AUTO_TEST_CASE_TEMPLATE( visit_command, F, menu_with_handle_creator_fixtures )
{
    F::menu_type m = F::create_menu_to_test().menu();
    m.insert(command_item_description(string_button_description(L"Bob"), 42));

    BOOST_CHECK_EQUAL(
        m[0].accept(test_visitor(expected_case::command)),
        "command");

    m[0].accept(void_test_visitor(expected_case::command));

    BOOST_CHECK_EQUAL(m[0].accept(templated_test_visitor()), "catch-all");

    BOOST_CHECK_EQUAL(
        m[0].accept(semi_inheritance_test_visitor(expected_case::command)),
        "command-or-sub_menu");
}

BOOST_AUTO_TEST_CASE_TEMPLATE( visit_command_const_correctness, F, menu_with_handle_creator_fixtures )
{
    F::menu_type m = F::create_menu_to_test().menu();
    m.insert(command_item_description(string_button_description(L"Bob"), 42));

    const item ci = m[0];

    // Const items should accept a visitor that only gives access to const items
    ci.accept(const_void_test_visitor(expected_case::const_command));

    // Const items should not accept a non-const visitor so the following
    // should cause a compile error
    //ci.accept(void_test_visitor(expected_case::command));

    // It should accept a visitor with both const and non-const items but only
    // fire the methods giving access to the const items
    ci.accept(
        const_and_nonconst_void_test_visitor(expected_case::const_command));

    // The non-const items should be equally happy with a const-only visitor
    m[0].accept(const_void_test_visitor(expected_case::const_command));
}

BOOST_AUTO_TEST_CASE_TEMPLATE( visit_sub_menu, F, menu_with_handle_creator_fixtures )
{
    F::menu_type m = F::create_menu_to_test().menu();
    menu s;
    s.insert(command_item_description(string_button_description(L"Pop"), 7));
    m.insert(sub_menu_item_description(string_button_description(L"Bob"), s));

    BOOST_CHECK_EQUAL(
        m[0].accept(test_visitor(expected_case::sub)),
        "sub_menu");

    m[0].accept(void_test_visitor(expected_case::sub));

    BOOST_CHECK_EQUAL(m[0].accept(templated_test_visitor()), "catch-all");

    BOOST_CHECK_EQUAL(
        m[0].accept(semi_inheritance_test_visitor(expected_case::sub)),
        "command-or-sub_menu");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(
    visit_sub_menu_const_correctness, F, menu_with_handle_creator_fixtures )
{
    F::menu_type m = F::create_menu_to_test().menu();
    menu s;
    s.insert(command_item_description(string_button_description(L"Pop"), 7));
    m.insert(sub_menu_item_description(string_button_description(L"Bob"), s));

    const item ci = m[0];

    // Const items should accept a visitor that only gives access to const items
    ci.accept(const_void_test_visitor(expected_case::const_sub));

    // Const items should not accept a non-const visitor so the following
    // should cause a compile error
    //ci.accept(void_test_visitor(expected_case::sub));

    // It should accept a visitor with both const and non-const items but only
    // fire the methods giving access to the const items
    ci.accept(
        const_and_nonconst_void_test_visitor(expected_case::const_sub));

    // The non-const items should be equally happy with a const-only visitor
    m[0].accept(const_void_test_visitor(expected_case::const_sub));
}

BOOST_AUTO_TEST_SUITE_END()

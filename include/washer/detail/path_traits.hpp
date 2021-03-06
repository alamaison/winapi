/**
    @file

    Traits-helper for writing generic path-related code.

    @if license

    Copyright (C) 2010, 2011  Alexander Lamaison <awl03@doc.ic.ac.uk>

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

#ifndef WASHER_DETAIL_PATH_TRAITS_HPP
#define WASHER_DETAIL_PATH_TRAITS_HPP
#pragma once

#include <boost/filesystem/path.hpp> // path_traits, basic_path

namespace washer {
namespace detail {

#if defined(BOOST_FILESYSTEM_VERSION) && BOOST_FILESYSTEM_VERSION < 3

/**
 * Helper to work around lack of genericity in path/wpath selection.
 */
template<typename T>
struct path_traits_chooser;

template<> struct path_traits_chooser<char>
{
    typedef boost::filesystem::path_traits trait;
};

template<> struct path_traits_chooser<wchar_t>
{
    typedef boost::filesystem::wpath_traits trait;
};

#endif

/**
 * Templated typedef shrink path-choosing declaration.
 *
 * e.g use:
 *     typename choose_path<T>::type
 * instead of:
 *    boost::filesystem::basic_path<
 *        std::basic_string<T>,
 *        typename detail::path_traits_chooser<T>::trait>
 */
template<typename T>
struct choose_path
{

#if defined(BOOST_FILESYSTEM_VERSION) && BOOST_FILESYSTEM_VERSION < 3
    typedef boost::filesystem::basic_path<
        std::basic_string<T>, typename path_traits_chooser<T>::trait> type;
#else
    typedef boost::filesystem::path type;
#endif
};

}} // namespace washer::detail

#endif

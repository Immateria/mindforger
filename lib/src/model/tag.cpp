/*
 tag.cpp     MindForger application entry point

 Copyright (C) 2016-2023 Martin Dvorak <martin.dvorak@mindforger.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "tag.h"

using namespace std;

namespace m8r {

Tag::Tag(const string& name, Clazz* isA, const Color& color)
    : Clazz(name, isA), color(color)
{
}

Tag::~Tag()
{
}

/*
 * Tags.
 */

Tags::Tags()
    : tags{}
{
}

Tags::Tags(std::vector<const Tag*> ts)
    : tags{ts}
{

}

Tags::~Tags()
{
}

} // namespace m8r

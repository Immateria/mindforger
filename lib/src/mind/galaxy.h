/*
 galaxy.h     MindForger thinking notebook

 Copyright (C) 2016-2025 Martin Dvorak <martin.dvorak@mindforger.com>

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
#ifndef M8R_GALAXY_H
#define M8R_GALAXY_H

#include <vector>

#include "../model/note.h"

namespace m8r {

class Galaxy
{
private:
    /**
     * @brief Note w/ the highest number of in/out relationships.
     */
    Note* blackHole;
    std::vector<Note*> stars;

public:
    Galaxy();
    Galaxy(const Galaxy&) = delete;
    Galaxy(const Galaxy&&) = delete;
    Galaxy &operator=(const Galaxy&) = delete;
    Galaxy &operator=(const Galaxy&&) = delete;
    ~Galaxy();
};

}
#endif // M8R_GALAXY_H

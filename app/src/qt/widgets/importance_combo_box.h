/*
 importance_combo_box.h     MindForger thinking notebook

 Copyright (C) 2016-2024 Martin Dvorak <martin.dvorak@mindforger.com>

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
#ifndef M8RUI_IMPORTANCE_COMBO_BOX_H
#define M8RUI_IMPORTANCE_COMBO_BOX_H

#include <QtWidgets>

#include "../../lib/src/representations/unicode.h"

namespace m8r {

class ImportanceComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit ImportanceComboBox(QWidget* parent);
    ImportanceComboBox(const ImportanceComboBox&) = delete;
    ImportanceComboBox(const ImportanceComboBox&&) = delete;
    ImportanceComboBox &operator=(const ImportanceComboBox&) = delete;
    ImportanceComboBox &operator=(const ImportanceComboBox&&) = delete;
    ~ImportanceComboBox();

    int8_t getValue() {
        return itemData(currentIndex(), Qt::UserRole).value<int>();
    }
};

}
#endif // M8RUI_IMPORTANCE_COMBO_BOX_H

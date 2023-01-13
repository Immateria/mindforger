/*
 organizers_table_view.h     MindForger thinking notebook

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
#ifndef M8RUI_ORGANIZERS_TABLE_VIEW_H
#define M8RUI_ORGANIZERS_TABLE_VIEW_H

#include <QtWidgets>

#include "../../../lib/src/debug.h"

namespace m8r {

class OrganizersTableView : public QTableView
{
    Q_OBJECT

public:
    explicit OrganizersTableView(QWidget* parent);
    OrganizersTableView(const OrganizersTableView&) = delete;
    OrganizersTableView(const OrganizersTableView&&) = delete;
    OrganizersTableView &operator=(const OrganizersTableView&) = delete;
    OrganizersTableView &operator=(const OrganizersTableView&&) = delete;
    virtual ~OrganizersTableView() override {}

    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

signals:
    void signalShowSelectedOrganizer();
};

}
#endif // M8RUI_ORGANIZERS_TABLE_VIEW_H

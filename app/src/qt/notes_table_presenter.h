/*
 notes_table_presenter.h     MindForger thinking notebook

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
#ifndef M8RUI_NOTES_TABLE_PRESENTER_H
#define M8RUI_NOTES_TABLE_PRESENTER_H

#include <QtWidgets>

#include "notes_table_view.h"
#include "notes_table_model.h"

namespace m8r {

class NotesTablePresenter : public QObject
{
    Q_OBJECT

private:
    NotesTableView* view;
    NotesTableModel* model;

public:
    NotesTablePresenter(NotesTableView* view);
    NotesTablePresenter(const NotesTablePresenter&) = delete;
    NotesTablePresenter(const NotesTablePresenter&&) = delete;
    NotesTablePresenter &operator=(const NotesTablePresenter&) = delete;
    NotesTablePresenter &operator=(const NotesTablePresenter&&) = delete;

    NotesTableModel* getModel() const { return model; }
    NotesTableView* getView() const { return view; }

    void refresh(std::vector<Note*>* notes);
};

}

#endif // M8RUI_NOTES_TABLE_PRESENTER_H

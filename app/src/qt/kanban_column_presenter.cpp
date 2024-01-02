/*
 kanban_column_presenter.cpp     MindForger thinking notebook

 Copyright (C) 2016-2024 Martin Dvorak <martin.dvorak@mindforger.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#include "kanban_column_presenter.h"

namespace m8r {

using namespace std;

KanbanColumnPresenter::KanbanColumnPresenter(
    KanbanColumnView* view,
    OrlojPresenter* orloj,
    QString title
) {
    this->view = view;
    this->model = new KanbanColumnModel(
        title, this, orloj->getMainPresenter()->getHtmlRepresentation()
    );
    this->view->setModel(this->model);

    this->orloj = orloj;

    // ensure HTML cells rendering
    HtmlDelegate* delegate = new HtmlDelegate();
    this->view->setItemDelegate(delegate);

    // hit ENTER to open selected O
    QObject::connect(
        view, SIGNAL(signalShowSelectedNote()),
        this, SLOT(slotShowSelectedNote()));
    QObject::connect(
        view, SIGNAL(signalShowSelectedKanbanNote()),
        this, SLOT(slotShowSelectedNote()));
    QObject::connect(
        view, SIGNAL(signalFocusToNextVisibleQuadrant()),
        this, SLOT(slotFocusToNextVisibleQuadrant()));
    QObject::connect(
        view, SIGNAL(signalFocusToPreviousVisibleQuadrant()),
        this, SLOT(slotFocusToPreviousVisibleQuadrant()));
    QObject::connect(
        view, SIGNAL(signalMoveNoteToNextQuadrant()),
        this, SLOT(slotMoveNoteToNextQuadrant()));
    QObject::connect(
        view, SIGNAL(signalMoveNoteToPreviousQuadrant()),
        this, SLOT(slotMoveNoteToPreviousQuadrant()));
    QObject::connect(
        this->view->horizontalHeader(), SIGNAL(sectionClicked(int)),
        this, SLOT(slotHeaderClicked(int))
    );

    QObject::connect(
        view, SIGNAL(signalOpenOrganizerPreferences()),
        this, SLOT(slotOrganizerEdit())
    );
}

KanbanColumnPresenter::~KanbanColumnPresenter()
{
}

int KanbanColumnPresenter::getCurrentRow() const
{
    QModelIndexList indexes = view->selectionModel()->selection().indexes();
    for(int i=0; i<indexes.count(); i++) {
        return indexes.at(i).row();
    }
    return NO_ROW;
}

Note* KanbanColumnPresenter::getSelectedNote()
{
    int row = getCurrentRow();
    if(row != NO_ROW) {
        QStandardItem* item = model->item(row);
        if(item) {
            // IMPROVE make my role constant
            return item->data(Qt::UserRole + 1).value<Note*>();
        } else {
            orloj->getMainPresenter()->getStatusBar()->showInfo(
                QString(tr("Selected Notebook/Note not found!"))
            );
        }
    } else {
        orloj->getMainPresenter()->getStatusBar()->showInfo(
            QString(tr("No Notebook selected!"))
        );
    }

    return nullptr;
}

void KanbanColumnPresenter::slotShowSelectedNote()
{
    Note* note = this->getSelectedNote();
    if(note != nullptr) {
        note->incReads();
        note->makeDirty();

        orloj->showFacetNoteView(note);
    }
}

void KanbanColumnPresenter::slotShowNote(
    const QItemSelection& selected, const QItemSelection& deselected
) {
    Q_UNUSED(deselected);

    QModelIndexList indices = selected.indexes();
    if(indices.size()) {
        const QModelIndex& index = indices.at(0);
        QStandardItem* item
            = model->itemFromIndex(index);
        // IMPROVE make my role constant
        Note* note = item->data(Qt::UserRole + 1).value<Note*>();

        note->incReads();
        note->makeDirty();

        orloj->showFacetNoteView(note);
    } // else do nothing
}

void KanbanColumnPresenter::slotOrganizerEdit()
{
    MF_DEBUG("Kanban column presenter: O/N table header clicked..." << endl);
    orloj->getMainPresenter()->doActionOrganizerEdit();
}

void KanbanColumnPresenter::slotHeaderClicked(int section)
{
    Q_UNUSED(section);

    this->slotOrganizerEdit();
}

void KanbanColumnPresenter::slotFocusToNextVisibleQuadrant()
{
    MF_DEBUG("Kanban column presenter: SLOT" << endl);
    orloj->getMainPresenter()->doActionOrganizerFocusToNextVisibleQuadrant();
}

void KanbanColumnPresenter::slotFocusToPreviousVisibleQuadrant()
{
    orloj->getMainPresenter()->doActionOrganizerFocusToPreviousVisibleQuadrant();
}

void KanbanColumnPresenter::slotMoveNoteToNextQuadrant()
{
    MF_DEBUG("Kanban column presenter: move N to next SLOT" << endl);
    Note* note = this->getSelectedNote();
    if(note != nullptr) {
        orloj->getMainPresenter()->doActionOrganizerMoveNoteToNextVisibleQuadrant(note);
    }
}

void KanbanColumnPresenter::slotMoveNoteToPreviousQuadrant()
{
    MF_DEBUG("Kanban column presenter: move N to previous SLOT" << endl);
    Note* note = this->getSelectedNote();
    if(note != nullptr) {
        orloj->getMainPresenter()->doActionOrganizerMoveNoteToPreviousVisibleQuadrant(note);
    }
}

void KanbanColumnPresenter::refresh(
    const std::vector<Note*>& ts, bool urgency, bool importance, bool showOutline
) {
    model->removeAllRows();
    if(ts.size()) {
        view->setVisible(true);
        for(auto& t:ts) {
            model->addRow(t, urgency, importance, showOutline);
        }

        this->view->setFocus();
    }
}

void KanbanColumnPresenter::focusAndSelectRow()
{
    this->view->setCurrentIndex(this->model->index(0, 0));
}


} // m8r namespace

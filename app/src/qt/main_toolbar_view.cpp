/*
 main_toolbar_view.cpp     MindForger thinking notebook

 Copyright (C) 2016-2023 Martin Dvorak <martin.dvorak@mindforger.com>

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
#include "main_toolbar_view.h"

namespace m8r {

MainToolbarView::MainToolbarView(MainWindowView* mainWindowView)
    : QToolBar{tr("Main Toolbar"), mainWindowView}, mainWindow{mainWindowView}
{
    // TOOLBAR: L&F driven toolbar icons - dark vs. light

    actionFindFts = addAction(
        QIcon(":/icons/find-fts.svg"),
        "Full-text search");

    cli = new CliAndBreadcrumbsView{this};
    addWidget(cli);
    cli->show();

    addSeparator();

    actionNewOutlineOrNote = addAction(
        QIcon(":/icons/new.svg"),
        tr("New Notebook"));

    addSeparator();

    actionOpenRepository = addAction(
        QIcon(":/icons/open-repository.svg"),
        tr("Open a directory with Markdowns or MindForger repository"));
    actionOpenFile = addAction(
        QIcon(":/icons/open-file.svg"),
        tr("Open Markdown file"));

    addSeparator();

    actionHomeOutline = addAction(
        QIcon(":/icons/home.svg"),
        "Open home Notebook");

    addSeparator();

#ifdef MF_DEPRECATED
    actionViewDashboard = addAction(
        QIcon(":/icons/dashboard.svg"),
        tr("View Dashboard"));
#endif
#ifdef ONE_ORGANIZER
    actionViewEisenhower = addAction(
        QIcon(":/icons/view-eisenhower.svg"),
        tr("View Eisenhower Matrix"));
#else
    actionViewOrganizers = addAction(
        QIcon(":/icons/view-eisenhower.svg"),
        tr("View Eisenhower Matrices"));
#endif
    actionViewOutlines = addAction(
        QIcon(":/icons/view-outlines.svg"),
        tr("View Notebooks"));
    actionViewNavigator = addAction(
        QIcon(":/icons/view-navigator.svg"),
        tr("View Knowledge Graph Navigator"));
    actionViewTags = addAction(
        QIcon(":/icons/view-tags.svg"),
        tr("View Tags"));
    actionViewRecentNotes = addAction(
        QIcon(":/icons/view-recent-notes.svg"),
        tr("View Recent Notes"));

    addSeparator();

    actionThink = addAction(
        QIcon(":/icons/think.svg"),
        "Think / Sleep");

    addSeparator();

    actionScope = addAction(
        QIcon(":/icons/scope.svg"),
        "Scope Mind");
    actionAdapt = addAction(
        QIcon(":/icons/adapt.svg"),
        "Adapt");

    addSeparator();

    actionHelp = addAction(
        QIcon(":/icons/help.svg"),
        "Open Documentation");
}

MainToolbarView::~MainToolbarView()
{
}

} // m8r namespace

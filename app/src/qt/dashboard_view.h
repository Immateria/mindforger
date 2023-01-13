/*
 dashboard_view.h     MindForger thinking notebook

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
#ifndef M8RUI_DASHBOARD_VIEW_H
#define M8RUI_DASHBOARD_VIEW_H

#include <QtWidgets>

#include "organizer_quadrant_view.h"
#include "recent_notes_table_view.h"
#include "navigator/navigator_view.h"
#include "tags_table_view.h"
#include "outlines_table_view.h"

namespace m8r {

/**
 * @brief Dashboard
 */
class DashboardView : public QSplitter
{
    Q_OBJECT

private:    
    // if view is width < threshold columns, then shows simplified view w/o Mind-related columns
    static constexpr int SIMPLIFIED_VIEW_THRESHOLD_WIDTH = 75*2;

    bool isMindForgerRepository;

    QSplitter* left;
    QSplitter* middle;
    QSplitter* right;

    QTextBrowser* welcomeDashboardlet;
    OrganizerQuadrantView* doFirstDashboardlet;
    OrganizerQuadrantView* doSoonDashboardlet;
    NavigatorView* navigatorDashboardlet;
    RecentNotesTableView* recentDashboardlet;
    TagsTableView* tagsDashboardlet;
    OutlinesTableView* outlinesDashboardlet;

public:
    explicit DashboardView(QWidget* parent);
    DashboardView(const DashboardView&) = delete;
    DashboardView(const DashboardView&&) = delete;
    DashboardView &operator=(const DashboardView&) = delete;
    DashboardView &operator=(const DashboardView&&) = delete;
    ~DashboardView();

    void setMindForgerMode(bool isMindForgerRepository);

    QTextBrowser* getWelcomeDashboardlet() { return welcomeDashboardlet; }
    OrganizerQuadrantView* getDoFirstDashboardlet() { return doFirstDashboardlet; }
    OutlinesTableView* getOutlinesDashboardlet() { return outlinesDashboardlet; }
    NavigatorView* getNavigatorDashboardlet() { return navigatorDashboardlet; }
    RecentNotesTableView* getRecentDashboardlet() { return recentDashboardlet; }
    TagsTableView* getTagsDashboardlet() { return tagsDashboardlet; }

protected:
    void resizeEvent(QResizeEvent* event) override;
};

}
#endif // M8RUI_DASHBOARD_VIEW_H

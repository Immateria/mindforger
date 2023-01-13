/*
 async_task_notifications_distributor.h     MindForger thinking notebook

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
#ifndef M8RUI_ASYNC_TASK_NOTIFICATIONS_DISTRIBUTOR_H
#define M8RUI_ASYNC_TASK_NOTIFICATIONS_DISTRIBUTOR_H

#include <vector>
#include <future>

#include "../../lib/src/debug.h"
#include "../../lib/src/model/note.h"
#include "../../lib/src/mind/associated_notes.h"

#include "../main_window_presenter.h"

namespace m8r {

class MainWindowPresenter;

/**
 * @brief Worker thread that distributes lib/backend async task/thread results to Qt GUI.
 *
 * IMPORTANT: you must work with Qt GUI only from GUI thread, that is main Qt thread.
 *
 * Therefore this distributor notifies GUI components using *signals* so that they
 * can be repainted() by the main Qt thread (signal is added to Qt event queue).
 *
 * Distributor is started from MainWindowPresenter where all MVP components are easily
 * accessible.
 *
 * Summary: distributor gets or pulls tasks, executes them (in its own thread i.e. it
 * doesn't block Qt main thread) and notifies result using signals to Qt frontend (which
 * ensures asynchronous dispatch).
 */
class AsyncTaskNotificationsDistributor : public QThread
{
    Q_OBJECT

public:

    enum TaskType {
        DREAM_TO_THINK
        // IMPROVE NOTE_ASSOCIATIONS
    };

    /**
     * @brief Task to be performed on successful finish of the future.
     */
    // IMPROVE this hardcoded data class is stupid and ugly
    class Task {
        std::shared_future<bool> f;
        TaskType tt;
        Outline* o;
        Note* n;

    public:
        explicit Task(std::shared_future<bool> f, TaskType tt) {
            this->f = f;
            this->tt = tt;
        }

        bool isReady() const {
            return f.wait_for(std::chrono::microseconds(0)) == std::future_status::ready;
        }

        bool isSuccessful() const { return f.get(); }
        void setOutline(Outline* o) { this->o = o; }
        Outline* getOutline() const { return o; }
        void setNote(Note* n) { this->n = n; }
        Note* getNote() const { return n; }
        TaskType getType() const { return tt; }
    };

private:
    MainWindowPresenter* mwp;

    int sleepInterval;

    std::vector<Task*> tasks;
    std::mutex tasksMutex;

public:
    explicit AsyncTaskNotificationsDistributor(MainWindowPresenter* mwp);
    ~AsyncTaskNotificationsDistributor();

    /**
     * @brief Worker thread code.
     */
    void run();

    /*
     * Futures to be notified
     */

    void add(Task* task) {
        std::lock_guard<std::mutex> criticalSection{tasksMutex};
        tasks.push_back(task);
    }

// signals that are sent by distributor to GUI components
signals:
    void statusBarShowStatistics();
    void showStatusBarInfo(QString msg);
    void refreshHeaderLeaderboardByValue(AssociatedNotes* associations);
    void refreshLeaderboardByValue(AssociatedNotes* associations);
    void signalRefreshCurrentNotePreview();

public slots:

    void slotConfigurationUpdated();
};

}
#endif // M8RUI_ASYNC_TASK_NOTIFICATIONS_DISTRIBUTOR_H

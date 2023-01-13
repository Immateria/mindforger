/*
 filesystem_persistence.h     MindForger thinking notebook

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
#ifndef M8R_FILESYSTEM_PERSISTENCE_H
#define M8R_FILESYSTEM_PERSISTENCE_H

#include <string>

#include "persistence.h"
#include "../config/configuration.h"
#include "../model/stencil.h"
#include "../representations/markdown/markdown_outline_representation.h"
#include "../representations/html/html_outline_representation.h"

namespace m8r {

class FilesystemPersistence : public Persistence
{
private:
    MarkdownOutlineRepresentation& mdRepresentation;
    HtmlOutlineRepresentation& htmlRepresentation;

public:

    static std::string getUniqueDirOrFileName(
        const std::string& directory,
        const std::string* text,
        const std::string& extension
    );

public:
    FilesystemPersistence(MarkdownOutlineRepresentation& mdRepresentation, HtmlOutlineRepresentation& htmlRepresentation);
    FilesystemPersistence(const FilesystemPersistence&) = delete;
    FilesystemPersistence(const FilesystemPersistence&&) = delete;
    FilesystemPersistence &operator=(const FilesystemPersistence&) = delete;
    FilesystemPersistence &operator=(const FilesystemPersistence&&) = delete;
    ~FilesystemPersistence();

    /**
     * @brief Create filename that doesn't clash with existing files.
     */
    virtual std::string createFileName(
        const std::string& directory,
        const std::string* text,
        const std::string& extension
    );
    /**
     * @brief Load stencil content
     * @param stencil   concept of the stencil to be set
     */
    virtual void load(Stencil* stencil);
    /**
     * @brief Check whether Outline file is writeable or read-only.
     * @param outline   Outline to be written.
     * @return `false` if read-only, else `true`.
     */
    bool isWriteable(const std::string& outlineKey);
    virtual void save(Outline* outline);
    virtual void saveAsHtml(Outline* o, const std::string& fileName);
};

}
#endif // M8R_FILESYSTEM_PERSISTENCE_H

# src.pro     MindForger thinking notebook
#
# Copyright (C) 2016-2025 Martin Dvorak <martin.dvorak@mindforger.com>
#
# This program is free software ; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation ; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY ; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

TARGET = mindforger-lib-unit-tests
TEMPLATE = app

DEFINES = DO_MF_DEBUG

CONFIG += console
CONFIG -= app_bundle

win32|macx {
    # Qt Network as CURL replacement on Win - add Qt to libmindforger
    CONFIG += qt
    QT += network
} else {
    CONFIG -= qt
}

message("= MindForger library test QMake configuration ==========================")
message("Qt version: $$QT_VERSION")


INCLUDEPATH += $$PWD/../../../lib/src
DEPENDPATH += $$PWD/../../../lib/src


# -L where to look for library, -l link the library
win32 {
    CONFIG(release, debug|release): LIBS += -L$$PWD/../../release -lmindforger
    else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../debug -lmindforger
} else {
    LIBS += -L$$OUT_PWD/../../../lib -lmindforger -lcurl
}

!mfnomd2html {
  win32 {
    DEFINES += MF_MD_2_HTML_CMARK
    CONFIG(release, debug|release) {
        LIBS += -L$$PWD/../../../deps/cmark-gfm/build/src/Release -lcmark-gfm_static
        LIBS += -L$$PWD/../../../deps/cmark-gfm/build/extensions/Release -lcmark-gfm-extensions_static
    } else:CONFIG(debug, debug|release) {
        LIBS += -L$$PWD/../../../deps/cmark-gfm/build/src/Debug -lcmark-gfm_static
        LIBS += -L$$PWD/../../../deps/cmark-gfm/build/extensions/Debug -lcmark-gfm-extensions_static
    }
  } else {
    # cmark-gfm
    DEFINES += MF_MD_2_HTML_CMARK
    INCLUDEPATH += $$PWD/../../../deps/cmark-gfm/src
    INCLUDEPATH += $$PWD/../../../deps/cmark-gfm/extensions
    INCLUDEPATH += $$PWD/../../../deps/cmark-gfm/build/src
    INCLUDEPATH += $$PWD/../../../deps/cmark-gfm/build/extensions
    LIBS += -L$$PWD/../../../deps/cmark-gfm/build/extensions -lcmark-gfm-extensions
    LIBS += -L$$PWD/../../../deps/cmark-gfm/build/src -lcmark-gfm
  }
} else {
  DEFINES += MF_NO_MD_2_HTML
}


# zlib
win32 {
    INCLUDEPATH += $$PWD/../../../deps/zlib-win/include
    DEPENDPATH += $$PWD/../../../deps/zlib-win/include

    CONFIG(release, debug|release): LIBS += -L$$PWD/../../../deps/zlib-win/lib/ -lzlibwapi
    else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../deps/zlib-win/lib/ -lzlibwapi
} else {
    LIBS += -lz
}

#
win32 {
    LIBS += -lRpcrt4 -lOle32 -lShell32
}

# GTest unit test framework library dependencies
win32 {
    INCLUDEPATH += "c:/Program Files/googletest-distribution/include"
    CONFIG(release, debug|release): LIBS += -L"c:/Program Files/googletest-distribution/lib" -lgtest
    else:CONFIG(debug, debug|release): LIBS += -L"c:/Program Files/googletest-distribution/lib" -lgtestd
} else {
    LIBS += -lgtest
    LIBS += -lpthread
}

# compiler options
win32{
    QMAKE_CXXFLAGS += /MP

    # DISABLED ccache as it causes compilation error:
    #   "C1090: PDB API call failed, error code '23'" when used
    # when used w/ MS VS compiler:
    # !mfnoccache { QMAKE_CXX = ccache $$QMAKE_CXX }
} else {
    # linux and macos
    mfnoccache {
      QMAKE_CXX = g++
    } else:!mfnocxx {
      QMAKE_CXX = ccache g++
    }
    QMAKE_CXXFLAGS += -pedantic -std=c++11
    # ensure valgrind line numbers
    mfunits {
      QMAKE_CXX += -g
    }
}

SOURCES += \
    ./test_utils.cpp \
    ./indexer/repository_indexer_test.cpp \
    ./config/configuration_test.cpp \
    ./markdown/markdown_test.cpp \
    ./html/html_test.cpp \
    ./json/json_test.cpp \
    ../benchmark/markdown_benchmark.cpp \
    ../benchmark/html_benchmark.cpp \
    ../benchmark/trie_benchmark.cpp \
    ../benchmark/ai_benchmark.cpp \
    ./ai/nlp_test.cpp \
    ./ai/autolinking_test.cpp \
    ./ai/autolinking_cmark_test.cpp \
    ./gear/datetime_test.cpp \
    ./gear/string_utils_test.cpp \
    ./gear/file_utils_test.cpp \
    ./gear/trie_test.cpp \
    ./mind/fts_test.cpp \
    ./mind/memory_test.cpp \
    ./mind/mind_test.cpp \
    ./mind/note_test.cpp \
    ./mindforger_lib_unit_tests.cpp \
    ./mind/organizer_test.cpp \
    ./mind/outline_test.cpp \
    ./mind/filesystem_information_test.cpp

HEADERS += \
    ./test_gear.h

# ########################################
# Diagnostics
# ########################################

message(DEFINES of app.pro build: $$DEFINES)
message(QMAKE_EXTRA_TARGETS of app.pro build: $$QMAKE_EXTRA_TARGETS)
message(QT of app.pro build: $$QT)
message(PATH is: $$(PATH))

# eof

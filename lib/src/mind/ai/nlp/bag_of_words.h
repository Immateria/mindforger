/*
 bag_of_words.h     MindForger thinking notebook

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
#ifndef M8R_BAG_OF_WORDS_H
#define M8R_BAG_OF_WORDS_H

#include <map>

#ifdef DO_MF_DEBUG
#include <iostream>
#endif

#include "word_frequency_list.h"
#include "../../../model/note.h"

namespace m8r {

/**
 * @brief BoW.
 *
 * Classic BoW definition:
 *
 * ----------------------------------
 * | Document | Word1 | ... | WordN |
 * ----------------------------------
 * | N1       |     1 | ... |     1 |
 * | O2       |     3 | ... |     0 |
 * | N3       |     0 | ... |     1 |
 * ----------------------------------
 *
 */
class BagOfWords
{
private:
    /**
     * @brief Document (O/N) to words (w/ frequencies).
     */
    std::map<Thing*,WordFrequencyList*> bow;

public:
    explicit BagOfWords();
    BagOfWords(const BagOfWords&) = delete;
    BagOfWords(const BagOfWords&&) = delete;
    BagOfWords &operator=(const BagOfWords&) = delete;
    BagOfWords &operator=(const BagOfWords&&) = delete;
    ~BagOfWords() {
        clear();
    }

    size_t size() const { return bow.size(); }
    void clear() {
        for(auto p:bow) {
            delete p.second;
        }
        bow.clear();
    }

    void add(Thing* t, WordFrequencyList* wfl) {
        bow[t] = wfl;
    }

    WordFrequencyList* get(Thing* t) {
        return bow[t];
    }

    void reorderDocVectorsByWeight();

#ifdef DO_MF_DEBUG
    void print() const {
        MF_DEBUG("BoW[" << bow.size() << "]:" << std::endl);
        for(auto& e:bow) {
            MF_DEBUG("  '" << e.first->getName() << "' > ");
            e.second->printFlat();
            MF_DEBUG(std::endl);
        }
    }
#endif
};

}
#endif // M8R_BAG_OF_WORDS_H

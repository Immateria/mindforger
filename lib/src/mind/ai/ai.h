/*
 ai.h     MindForger thinking notebook

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
#ifndef M8R_AI_H
#define M8R_AI_H

#include <vector>
#include <future>
#include <mutex>

#include "../mind.h"
#include "../../model/outline.h"
#include "../memory.h"
#include "./aa_model.h"
#include "./ai_aa_weighted_fts.h"
#include "./ai_aa_bow.h"
#ifdef MF_NER
    #include "./nlp/named_entity_recognition.h"
#endif

namespace m8r {

/**
 * @brief Mind's AI.
 *
 * IMPORTANT: AI cannot be called directory - its methods to be always invoked
 *            by Mind. Method visibility was not restricted in order to ensure
 *            AI testability.
 *
 * AI lifecycle:
 *
 *  SLEEPING            ... initial AI state
 *     |
 *   amnesia(), sleep()
 *     |
 *     V
 *  SLEEPING
 *     |
 *   learnMemory()
 *     |
 *     V
 *  DREAMING
 *     |                ... automatically switches to THINKING once Memory is learned
 *     V
 *  THINKING
 *     |
 *   amnesia(), sleep() ... if no computation is in progress
 *     |
 *     V
 *  SLEEPING
 *
 */
class Ai
{
private:

    /*
     * Associations
     */

    // Associations assessment implemenations: AA @ weighted FTS, AA @ BoW
    AiAssociationsAssessment* aa;

#ifdef MF_NER
    /*
     * Named-entity recognition (NER)
     */

    NamedEntityRecognition ner;
#endif

    /*
     * Neural network models
     */

    // TODO AssociationAssessmentModel* aaModel;

public:
    explicit Ai(Memory& memory, Mind& mind);
    Ai(const Ai&) = delete;
    Ai(const Ai&&) = delete;
    Ai &operator=(const Ai&) = delete;
    Ai &operator=(const Ai&&) = delete;
    ~Ai();

    /**
     * @brief Learn what's in memory to get ready for thinking.
     *
     * Can be LONG running on huge repositories.
     * Synchronized by caller ~ Mind.
     */
    std::shared_future<bool> dream()
    {
        return aa->dream();
    }

    /**
     * @brief Get best Note associations.
     *
     * Can be LONG running on big repositories.
     * Synchronized by caller ~ Mind.
     */
    std::shared_future<bool> getAssociatedNotes(const Note* note, std::vector<std::pair<Note*,float>>& associations) {
        return aa->getAssociatedNotes(note, associations);
    }

    std::shared_future<bool> getAssociatedNotes(Outline* outline, std::vector<std::pair<Note*,float>>& associations) {
        return aa->getAssociatedNotes(outline, associations);
    }

    std::shared_future<bool> getAssociatedNotes(const std::string& words, std::vector<std::pair<Note*,float>>& associations, const Note* self=nullptr) {
        return aa->getAssociatedNotes(words, associations, self);
    }

#ifdef MF_NER
    bool isNerInitialized() const { return ner.isInitialized(); }

    /**
     * @brief Recognize person names in O.
     */
    void recognizePersons(const Outline* outline, int entityFilter, std::vector<NerNamedEntity>& result) {
        ner.recognizePersons(outline, entityFilter, result);
    }
#endif

    /**
     * @brief Clear, but don't deallocate.
     *
     * Synchronized by caller ~ Mind.
     */
    bool sleep() {
        return aa->sleep();
    }

    /**
     * @brief Forget everything.
     *
     * Synchronized by caller ~ Mind.
     */
    bool amnesia() {
        return aa->amnesia();
    }

private:

    /**
     * @brief Train associations assessment neural network once memory is learned.
     */
    void trainAaNn();

public:
#ifdef DO_MF_DEBUG
    static void print(const Note* n, std::vector<std::pair<Note*,float>>& leaderboard) {
        std::cout << "Note '" << n->getName() << "' AA leaderboard("<< leaderboard.size() <<"):" << std::endl;
        int i=1;
        for(auto& nn:leaderboard) {
            std::cout << "  #" << i++ << " '" << nn.first->getName() << "' ~ " << nn.second << std::endl;
        }
    }
#endif
};

}
#endif // M8R_AI_H

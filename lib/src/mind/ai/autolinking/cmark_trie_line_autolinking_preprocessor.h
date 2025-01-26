/*
 cmark_trie_line_autolinking_preprocessor.h     MindForger thinking notebook

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
#ifndef M8R_CMARK_TRIE_LINE_AUTOLINKING_PREPROCESSOR_H
#define M8R_CMARK_TRIE_LINE_AUTOLINKING_PREPROCESSOR_H

#ifdef MF_MD_2_HTML_CMARK
  #include <cmark-gfm.h>
#endif

#include "../autolinking_preprocessor.h"

/*
 * DEPRECATED
 *
 * This line-based autolinker has been deprecated by block-based autlinker.
 */

namespace m8r {

/**
 * @brief cmark-gfm AST and Trie autolinking pre-processor.
 *
 * Autolinking implementation which aims to be both precise (cmark-gfm AST)
 * and fast (Trie).
 *
 * Ideal autolinking implementation has two goals:
 *
 *  - correctness
 *  - performance
 *
 * It aims to be FAST, therefore it's based on Aho-Corasick search
 * structure when autolinking lines.
 *
 * It aims to AVOID BREAKING rendered HTML by partial parsing of MD
 * constructions and autolinking only safe segments of line's AST.
 *
 * Algorithm description:
 *
 *  - O or N description is autolinked
 *  - O or N or T links are injected
 *
 * Preparation:
 *
 *  1. Aho-Corasick search structure is (re)build:
 *    - Aho-Corasick structure is Trie w/ internal links
 *    - the structure can be CACHED and rebuilt only when name or description
 *      of O/N is changed OR tag is added/removed
 *
 * Method:
 *
 *  1. Description is processed LINE by LINE
 *    a) if LINE is in code or math BLOCK, then it's skipped
 *  2. LINE is partially parsed for INLINED code, math, link, ... constructions
 *     to get line AST with TEXT SECTIONS which can be safely autolinked
 *  3. Text sections are searched for O/N/T names presense using Aho-Corasick
 *     search structure:
 *    a) Aho-Corasics structure searches for all O/N/T names at once in single pass
 *    b) Aho-Corasics structure has LINEAR COMPLEXITY
 *    c) Aho-Corasics structure gives ALL MATCHES found in the text
 *    d) every MATCH to have:
 *      - begin offset
 *      - end offset
 *      - length
 *      - type O/N/T
 *    e) in case of overlapping/included MATCHES:
 *      - longest match always wins
 *  4. When there is O/N/T name clash (e.g. `ML` can be Outline, Note and Tag, actually
 *     there can be even more Notes with this name), then these matches are remembered
 *     and when user CLICKs the link in the description, then DIALOG with all options
 *     is opened (think Wikipedia cross-road page) and user can choose which link
 *     to use.
 */
class CmarkTrieLineAutolinkingPreprocessor : public AutolinkingPreprocessor
{
protected:
    // allowed text MD snippets words trailing chars (\\... added newly)
    static const std::string TRAILING_CHARS;

public:
    explicit CmarkTrieLineAutolinkingPreprocessor(Mind& mind);
    CmarkTrieLineAutolinkingPreprocessor(const CmarkTrieLineAutolinkingPreprocessor&) = delete;
    CmarkTrieLineAutolinkingPreprocessor(const CmarkTrieLineAutolinkingPreprocessor&&) = delete;
    CmarkTrieLineAutolinkingPreprocessor &operator=(const CmarkTrieLineAutolinkingPreprocessor&) = delete;
    CmarkTrieLineAutolinkingPreprocessor &operator=(const CmarkTrieLineAutolinkingPreprocessor&&) = delete;
    ~CmarkTrieLineAutolinkingPreprocessor();

    /**
     * @brief Autolink Markdown.
     *
     * Provide previous Thing's name to update indices.
     */
    virtual void process(const std::vector<std::string*>& md, std::string& amd) override;

private:
    virtual void processLineByLine(const std::vector<std::string*>& md, std::string& amd);

    void processProtectedBlock(std::vector<std::string*>& block, std::string& amd);
    void processAndAutolinkBlock(std::vector<std::string*>& block, std::string& amd);

    /**
     * @brief Parse MD line to AST to get MD snippets which are safe for links injection.
     */
    void parseMarkdownLine(const std::string* md, std::string* amd);

    /**
     * @brief Inject Os and Ns links to given Markdown snippet.
     */
    void injectThingsLinks(cmark_node* node);

    cmark_node* addAstLinkNode(cmark_node* origNode, cmark_node* node, std::string& pre);
    cmark_node* addAstTxtNode(cmark_node* origNode, cmark_node* node, std::string& at);
};

}
#endif // M8R_CMARK_TRIE_LINE_AUTOLINKING_PREPROCESSOR_H

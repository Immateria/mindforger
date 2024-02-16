/*
 ai.cpp     MindForger thinking notebook

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
#include "ai.h"

namespace m8r {

using namespace std;

Ai::Ai(Memory& memory, Mind& mind)
{
    switch(Configuration::getInstance().getAaAlgorithm()) {
    case Configuration::AssociationAssessmentAlgorithm::BOW:
        aa = new AiAaBoW{memory,mind};
        break;
    case Configuration::AssociationAssessmentAlgorithm::WEIGHTED_FTS:
        aa = new AiAaWeightedFts{memory,mind};
        break;
    default:
        aa = nullptr;
    }
}

Ai::~Ai()
{
    if(aa) delete aa;
}

void Ai::trainAaNn()
{
    MF_DEBUG("AI: training AA NN..." << endl);
    // TODO train NN using a reasonable number of features: createAaFeature()
    MF_DEBUG("AI: AA NN trained!" << endl);
}

} // m8r namespace

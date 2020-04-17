//
// Created by losos on 04.04.2020.
//

#ifndef HOMILYDATABASE_SIGLUMPAIR_H
#define HOMILYDATABASE_SIGLUMPAIR_H

#include <utility>

#include "SimpleSiglum.h"

class SiglumPair {
private:
	SimpleSiglum m_siglumBeginning, m_siglumEnding;
public:
	SiglumPair(SimpleSiglum siglumBeginning, SimpleSiglum siglumEnding)
			: m_siglumBeginning(std::move(siglumBeginning)), m_siglumEnding(std::move(siglumEnding)) {}

	[[nodiscard]] const SimpleSiglum& getSiglumBeginning() const { return m_siglumBeginning; }

	[[nodiscard]] const SimpleSiglum& getSiglumEnding() const { return m_siglumEnding; }

	void setSiglumBeginning(const SimpleSiglum& siglumBeginning) { m_siglumBeginning = siglumBeginning; }

	void setSiglumEnding(const SimpleSiglum& siglumEnding) { m_siglumEnding = siglumEnding; }

	/**TODO
	 *  - make verif. func.*/
};

#endif //HOMILYDATABASE_SIGLUMPAIR_H

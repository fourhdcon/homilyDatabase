//
// Created by losos on 04.04.2020.
//

#ifndef HOMILYDATABASE_SINGLEHOMILY_H
#define HOMILYDATABASE_SINGLEHOMILY_H

#include <utility>

#include "SiglumPair.h"

class SingleHomily {
private:
	SiglumPair m_siglumPair;
	std::string m_author, m_link;
public:
	SingleHomily(SiglumPair siglumPair, std::string author, std::string link)
			: m_siglumPair(std::move(siglumPair)), m_author(std::move(author)), m_link(std::move(link)) {}

	[[nodiscard]] const SiglumPair& getSiglumPair() const { return m_siglumPair; }

	[[nodiscard]] const std::string& getAuthor() const { return m_author; }

	[[nodiscard]] const std::string& getMLink() const { return m_link; }

	void setSiglumPair(const SiglumPair& siglumPair) { m_siglumPair = siglumPair; }

	void setMAuthor(const std::string& author) { m_author = author; }

	void setMLink(const std::string& link) { m_link = link; }

	/**TODO
	 *  - make verif. func. for link and author*/
};

#endif //HOMILYDATABASE_SINGLEHOMILY_H
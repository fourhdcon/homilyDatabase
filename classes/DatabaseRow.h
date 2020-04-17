//
// Created by losos on 05.04.2020.
//

#ifndef HOMILYDATABASE_DATABASEROW_H
#define HOMILYDATABASE_DATABASEROW_H

#include <string>

class DatabaseRow {
private:
	const int m_id;
	const std::string m_author, m_link, m_siglumBeginning, m_siglumEnding;

public:
	DatabaseRow(const int id,
				std::string author,
				std::string link,
				std::string siglumBeginning,
				std::string siglumEnding)
			: m_id(id),
			  m_author(std::move(author)),
			  m_link(std::move(link)),
			  m_siglumBeginning(std::move(siglumBeginning)),
			  m_siglumEnding(std::move(siglumEnding)) {}

	[[nodiscard]] int getId() const { return m_id; }

	[[nodiscard]] const std::string& getAuthor() const { return m_author; }

	[[nodiscard]] const std::string& getLink() const { return m_link; }

	[[nodiscard]] const std::string& getsiglumBeginning() const { return m_siglumBeginning; }

	[[nodiscard]] const std::string& getsiglumEnding() const { return m_siglumEnding; }

	[[nodiscard]] std::string getString() const {
		return std::to_string(m_id) SPLITTERSTRING
			   m_author SPLITTERSTRING
			   m_link SPLITTERSTRING
			   m_siglumBeginning SPLITTERSTRING
			   m_siglumEnding;
	}

	[[nodiscard]] std::string getStringHTML() const {
		return "Id: " + std::to_string(m_id) + "<br>" +
			   "Author: " + m_author + "<br>" +
			   "Link: " + m_link + "<br>" +
			   "siglumBeginning: " + m_siglumBeginning + "<br>" +
			   "siglumEnding: " + m_siglumEnding;
	}

	friend std::ostream& operator<<(std::ostream& os, const DatabaseRow& databaseResultRow) {
		os << std::to_string(databaseResultRow.getId()) SPLITTER
		   databaseResultRow.getAuthor() SPLITTER
		   databaseResultRow.getLink() SPLITTER
		   databaseResultRow.getsiglumBeginning() SPLITTER
		   databaseResultRow.getsiglumEnding();
		return os;
	}
};

#endif //HOMILYDATABASE_DATABASEROW_H

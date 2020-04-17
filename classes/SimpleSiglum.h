//
// Created by losos on 04.04.2020.
//

#ifndef HOMILYDATABASE_SIMPLESIGLUM_H
#define HOMILYDATABASE_SIMPLESIGLUM_H

#include <string>

class SimpleSiglum {
private:
	std::string m_book;
	__uint16_t m_chapter, m_verse;
public:
	SimpleSiglum(std::string book, __uint16_t chapter, __uint16_t verse, __uint16_t verseEnding)
			: m_book(std::move(book)),
			  m_chapter(chapter),
			  m_verse(verse) {};

	[[nodiscard]] const std::string& getBook() const { return m_book; }

	[[nodiscard]] __uint16_t getChapter() const { return m_chapter; }

	[[nodiscard]] __uint16_t getVerse() const { return m_verse; }

	void setBook(const std::string& book) { m_book = book; }

	void setChapter(__uint16_t chapter) { m_chapter = chapter; }

	void setVerse(__uint16_t verse) { m_verse = verse; }

	/**TODO
	 *  - make verif. func.*/
};

#endif //HOMILYDATABASE_SIMPLESIGLUM_H
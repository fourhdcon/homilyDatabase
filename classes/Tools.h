//
// Created by losos on 16.04.2020.
//

#ifndef HOMILYDATABASE_TOOLS_H
#define HOMILYDATABASE_TOOLS_H

#include <memory>
#include <vector>
#include <string>
#include <utility>
#include <functional>
#include <optional>
#include <restbed>
#include <Poco/URI.h>
#include "FileHandling.h"
#include "Database.h"

extern "C" size_t decode_html_entities_utf8(char* dest, const char* src);

class Tools {
	typedef std::optional<std::pair<std::string, std::string>> optionalPairofStrings;
public:
	static bool checkCookieAuth(const std::shared_ptr<restbed::Session>& session);

	[[nodiscard]] static std::vector<std::string> splitStringByAmpersand(const std::string& stringToSplit);

	[[nodiscard]] static std::vector<std::string> splitStringByString(const std::string& stringToSplit,
																	  const std::string& delimiter);

	[[nodiscard]] static bool findAuthEq1(const std::vector<std::string>& baseVector);

	[[nodiscard]] static std::string replacePlusesWithSpaces(std::string sourceString);

	[[nodiscard]] static optionalPairofStrings splitStringByEqual(const std::string& stringToSplit);

	static void findAndReplace(std::string& base,
							   const std::string& toReplace,
							   const std::function<std::string(const std::string&)>& function);

	static void findAndReplace(std::string& base,
							   const std::string& toReplace,
							   const std::string& replaceWith);

	[[nodiscard]] static std::string findAndReplaceReturn(std::string base,
														  const std::string& toReplace,
														  const std::string& replaceWith);

	static void closingSessionHandler(const std::shared_ptr<restbed::Session>& session,
									  std::string&& response,
									  bool usingButton = true,
									  const std::optional<std::pair<std::string, std::string>>& additionalHeader = std::nullopt);
};

#endif //HOMILYDATABASE_TOOLS_H

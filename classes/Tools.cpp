//
// Created by losos on 16.04.2020.
//

#include "Tools.h"

[[nodiscard]]  std::vector<std::string> Tools::splitStringByAmpersand(std::string stringToSplit) {
	std::vector<std::string> result;
	std::string segment;
	std::string temp;
	for (const auto c:stringToSplit) {
		if (c == '&') {
			result.push_back(temp);
			temp = "";
		} else
			temp += c;
	}
	if (stringToSplit.back() != '&')
		result.push_back(temp);
	return result;
}

[[nodiscard]] std::vector<std::string> Tools::splitStringByString(const std::string& stringToSplit,
																  const std::string& delimiter = "; ") {
	std::vector<std::string> result;

	size_t last = 0;
	size_t next{};
	while ((next = stringToSplit.find(delimiter, last)) != std::string::npos) {
		result.emplace_back(stringToSplit.substr(last, next - last));
		last = next + delimiter.size();
	}
	result.emplace_back(stringToSplit.substr(last));

	return result;
}

[[nodiscard]] bool Tools::findAuthEq1(const std::vector<std::string>& baseVector) {
	for (const auto& it : baseVector)
		if (it == "auth=1")
			return true;

	return false;
}

[[nodiscard]]  std::string Tools::replacePlusesWithSpaces(std::string sourceString) {
	for (auto& c : sourceString)
		if (c == '+')
			c = ' ';
	return sourceString;
}

typedef std::optional<std::pair<std::string, std::string>> optionalPairofStrings;

[[nodiscard]] optionalPairofStrings Tools::splitStringByEqual(std::string stringToSplit) {
	for (size_t i = 0; i < stringToSplit.length(); ++i) {
		if (stringToSplit.at(i) == '=') {
			std::string decodedToHTML;
			Poco::URI::decode(stringToSplit.substr(i + 1, stringToSplit.length() - 1), decodedToHTML);
			auto* decodedToUTF8 = new char[decodedToHTML.length()];
			decode_html_entities_utf8(decodedToUTF8, decodedToHTML.c_str());

			return std::make_optional(
					std::make_pair<std::string, std::string>(stringToSplit.substr(0, i),
															 replacePlusesWithSpaces(decodedToUTF8)));
		}
	}
	return optionalPairofStrings();
}

void Tools::findAndReplace(std::string& base,
						   const std::string& toReplace,
						   const std::function<std::string(const std::string&)>& function) {
	auto indexIfFound = base.find(toReplace);
	if (indexIfFound != std::string::npos) {
		std::string replaceWith = function("homilies");
		base.replace(indexIfFound, toReplace.length(), replaceWith);
	}
}

void Tools::findAndReplace(std::string& base,
						   const std::string& toReplace,
						   const std::string& replaceWith) {
	auto indexIfFound = base.find(toReplace);
	if (indexIfFound != std::string::npos)
		base.replace(indexIfFound, toReplace.length(), replaceWith);
}

[[nodiscard]] std::string Tools::findAndReplaceReturn(std::string base,
													  const std::string& toReplace,
													  const std::string& replaceWith) {
	auto indexIfFound = base.find(toReplace);
	if (indexIfFound != std::string::npos)
		base.replace(indexIfFound, toReplace.length(), replaceWith);
	return base;
}

bool Tools::checkCookieAuth(const std::shared_ptr<restbed::Session>& session) {
	const auto request = session->get_request();
	std::string cookies = request->get_header("Cookie", "");
	if (cookies.empty())
		return false;
	else {
		auto vectorCookies = splitStringByString(cookies);
		if (!findAuthEq1(vectorCookies))
			return false;
	}
	return true;
}

void Tools::closingSessionHandler(const std::shared_ptr<restbed::Session>& session,
								  std::string&& response,
								  const bool usingButton,
								  const std::optional<std::pair<std::string, std::string>>& additionalHeader) {
	if (usingButton) {
		std::string comebackButton = "<br><a href=\"/\"><button type=\"button\">Wróć do strony głównej</button></a>";
		response += comebackButton;
	}
	std::multimap<std::string, std::string> headers{{"Content-Type",   "text/html; charset=utf-8"},
													{"Content-Length", std::to_string(response.size())}};
	if (additionalHeader.has_value())
		headers.insert(additionalHeader.value());
	session->close(restbed::OK, response, headers);
}
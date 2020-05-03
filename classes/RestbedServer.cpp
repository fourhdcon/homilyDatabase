//
// Created by losos on 16.04.2020.
//

#include "RestbedServer.h"
#include "Tools.h"
#include <cctype>
#include <optional>
#include <utility>

void RestbedServer::defaultHandler(const std::shared_ptr<restbed::Session>& session) {
	std::string response = FileHandling::readFromFile("index.html");
	Tools::findAndReplace(response, "[[SELECT_ALL]]", Database::selectAll);
	{
		const std::string loingForm = "<form action=\"auth\" method=\"get\">"
									  "<input type=\"text\" name=\"username\" required placeholder=\"username\"><br>"
									  "<input type=\"password\" name=\"password\" required placeholder=\"password\"><br>"
									  "<input type=\"submit\" value=\"Sign in\">"
									  "</form><br><br>";
		Tools::findAndReplace(response, "[[LOGINING]]", Tools::checkCookieAuth(session) ? "" : loingForm);
	}

	Tools::closingSessionHandler(session, std::move(response), false);
}

void RestbedServer::displaySingle(const std::shared_ptr<restbed::Session>& session) {
	const auto request = session->get_request();

	if (!request->has_query_parameter("id"))
		Tools::closingSessionHandler(session, "No id parameter given");

	const unsigned int id = static_cast<unsigned int>(std::stoi(request->get_query_parameter("id")));

	std::string response;

	try {
		DatabaseRow homily = Database::selectSingleHomily(id);
		const auto& homilyLink = homily.getLink();
		response = FileHandling::readFromFile("displaySingle.html");
		Tools::findAndReplace(response, "[[HOMILY_TITLE]]", homily.getAuthor());
		Tools::findAndReplace(response, "[[HOMILY_AUTHOR]]", homily.getAuthor());
		Tools::findAndReplace(response, "[[HOMILY_LINK]]", homily.getLink());
		Tools::findAndReplace(response, "[[YOUTUBE_LINK]]",
							  Tools::findAndReplaceReturn(homilyLink, "watch?v=", "embed/"));
		Tools::findAndReplace(response, "[[HOMILY_SIGLUM_BEGINNING]]", homily.getsiglumBeginning());
		Tools::findAndReplace(response, "[[HOMILY_SIGLUM_ENDING]]", homily.getsiglumEnding());

		Tools::closingSessionHandler(session, std::move(response), false);
	}
	catch (const std::exception& exception) {
		Tools::closingSessionHandler(session, exception.what());
	}
}

void RestbedServer::addingHandler(const std::shared_ptr<restbed::Session>& session) {
	if (!Tools::checkCookieAuth(session))
		Tools::closingSessionHandler(session, "You are not authorized");

	Tools::closingSessionHandler(session, FileHandling::readFromFile("addForm.html"), false);
}

void RestbedServer::addingResultHandler(const std::shared_ptr<restbed::Session>& session) {
	if (!Tools::checkCookieAuth(session))
		Tools::closingSessionHandler(session, "You are not authorized");

	const auto& request = session->get_request();
	std::size_t content_length = static_cast<size_t>(request->get_header("Content-Length", 0));
	std::string requestBody;

	session->fetch(content_length,
				   [&requestBody]([[maybe_unused]] const std::shared_ptr<restbed::Session>& session,
								  const restbed::Bytes& line) {
					   char* cstr = new char[line.size() + 1];
					   strncpy(cstr, reinterpret_cast<const char*>(line.data()), line.size());
					   cstr[line.size()] = 0;
					   requestBody += cstr;
					   delete[] cstr;
				   });

	auto paramteres = Tools::splitStringByAmpersand(requestBody);

	std::string author, link, siglumBeginning, siglumEnding;

	for (const auto& it : paramteres) {
		std::optional<std::pair<std::string, std::string>> splittedOptional = Tools::splitStringByEqual(it);
		if(!splittedOptional.has_value())
			Tools::closingSessionHandler(session, "No equal sign in given parameters");
		auto splitted = splittedOptional.value();
		if (splitted.first == "author")
			author = splitted.second;
		else if (splitted.first == "link")
			link = splitted.second;
		else if (splitted.first == "siglumBeginning")
			siglumBeginning = splitted.second;
		else if (splitted.first == "siglumEnding")
			siglumEnding = splitted.second;
	}

	DatabaseRow databaseRow(0, author, link, siglumBeginning, siglumEnding);

	std::string response;
	if (Database::insertNewHomily(std::move(databaseRow))) {
		response = "Inserting was successful";
	} else {
		response = "Inserting encountered an error";
	}

	Tools::closingSessionHandler(session, std::move(response));
}

void RestbedServer::deletingHandler(const std::shared_ptr<restbed::Session>& session) {
	if (!Tools::checkCookieAuth(session))
		Tools::closingSessionHandler(session, "You are not authorized");

	Tools::closingSessionHandler(session, FileHandling::readFromFile("deleteForm.html"), false);
}

void RestbedServer::deletingResultHandler(const std::shared_ptr<restbed::Session>& session) {
	if (!Tools::checkCookieAuth(session))
		Tools::closingSessionHandler(session, "You are not authorized");

	const auto request = session->get_request();

	if (!request->has_query_parameter("id"))
		Tools::closingSessionHandler(session, "No id parameter given");

	const unsigned int id = static_cast<unsigned int>(std::stoi(request->get_query_parameter("id")));

	std::string response;

	if (Database::deleteOneHomily(id)) {
		response = "Deleting was successful";
	} else {
		response = "Deleting encountered an error";
	}

	Tools::closingSessionHandler(session, std::move(response));
}

std::shared_ptr<restbed::Resource> RestbedServer::setupResource(const std::string& route,
																const std::string& method,
																const std::function<void(const std::shared_ptr<restbed::Session>)>& callback) {
	auto resource = std::make_shared<restbed::Resource>();
	resource->set_path(route);
	resource->set_method_handler(method, callback);
	return resource;
}

void RestbedServer::authenticationHandler(const std::shared_ptr<restbed::Session>& session) {
	const auto request = session->get_request();

	if (!request->has_query_parameter("username") || !request->has_query_parameter("password")) {
		Tools::closingSessionHandler(session, "No username or password given");
	}

	const std::string username = request->get_query_parameter("username");
	const std::string password = request->get_query_parameter("password");

	bool doesUsernameContainsNonalpha
			= std::find_if(username.begin(), username.end(),
						   [](char c) { return !std::isalpha(c); }) != username.end();
	bool doesPasswordContainsNonalpha
			= std::find_if(password.begin(), password.end(),
						   [](char c) { return !std::isalpha(c); }) != password.end();

	if (doesPasswordContainsNonalpha || doesUsernameContainsNonalpha)
		Tools::closingSessionHandler(session, "Not allowed characters in username or password");

	if (username == "root" && password == "kucyk")
		Tools::closingSessionHandler(session, "Login was successful", true, std::make_pair("Set-Cookie", "auth=1"));
	else
		Tools::closingSessionHandler(session, "Invalid username or password");
}


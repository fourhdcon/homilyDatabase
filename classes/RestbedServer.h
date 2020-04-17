//
// Created by losos on 15.04.2020.
//

#ifndef HOMILYDATABASE_RESTBEDSERVER_H
#define HOMILYDATABASE_RESTBEDSERVER_H

#include <restbed>
#include <Poco/URI.h>
#include "FileHandling.h"
#include "Database.h"

class RestbedServer {
public:
	RestbedServer() {
		defaultResource = setupResource("/", "GET", defaultHandler);
		singleHomilyResource = setupResource("/displaySingle", "GET", displaySingle);
		addingPageResource = setupResource("/add", "GET", addingHandler);
		addingResultPageResource = setupResource("/add/result", "POST", addingResultHandler);
		deletingPageResource = setupResource("/delete", "GET", deletingHandler);
		deletingResultPageResource = setupResource("/delete/result", "GET", deletingResultHandler);
		authenticationResource = setupResource("/auth", "GET", authenticationHandler);

		settings = std::make_shared<restbed::Settings>();
		settings->set_port(8080);
		settings->set_worker_limit(4);
		settings->set_default_header("Connection", "close");

		service.publish(defaultResource);
		service.publish(singleHomilyResource);
		service.publish(addingPageResource);
		service.publish(addingResultPageResource);
		service.publish(deletingPageResource);
		service.publish(deletingResultPageResource);
		service.publish(authenticationResource);
		service.start(settings);
	}

private:
	std::shared_ptr<restbed::Resource> defaultResource;
	std::shared_ptr<restbed::Resource> singleHomilyResource;
	std::shared_ptr<restbed::Resource> addingPageResource;
	std::shared_ptr<restbed::Resource> addingResultPageResource;
	std::shared_ptr<restbed::Resource> deletingPageResource;
	std::shared_ptr<restbed::Resource> deletingResultPageResource;
	std::shared_ptr<restbed::Resource> authenticationResource;

	std::shared_ptr<restbed::Settings> settings;
	restbed::Service service;

	/**
	 * HANDLING METHODS
	 * */

	static std::shared_ptr<restbed::Resource> setupResource(const std::string& route, const std::string& method,
															const std::function<void(const std::shared_ptr<restbed::Session>)>& callback);

	static void defaultHandler(const std::shared_ptr<restbed::Session>& session);

	static void displaySingle(const std::shared_ptr<restbed::Session>& session);

	static void addingHandler(const std::shared_ptr<restbed::Session>& session);

	static void addingResultHandler(const std::shared_ptr<restbed::Session>& session);

	static void deletingHandler(const std::shared_ptr<restbed::Session>& session);

	static void deletingResultHandler(const std::shared_ptr<restbed::Session>& session);

	static void authenticationHandler(const std::shared_ptr<restbed::Session>& session);
};

#endif //HOMILYDATABASE_RESTBEDSERVER_H

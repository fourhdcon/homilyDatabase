//
// Created by losos on 12.04.2020.
//

#ifndef HOMILYDATABASE_FILEHANDLING_H
#define HOMILYDATABASE_FILEHANDLING_H

#include <string>
#include <fstream>

class FileHandling {
public:
	[[nodiscard]] static std::string readFromFile(const std::string& src) {
		std::ifstream file(src);
		std::string fileBody;
		for (std::string line; getline(file, line);)
			fileBody += line;
		return fileBody;
	}
};

#endif //HOMILYDATABASE_FILEHANDLING_H

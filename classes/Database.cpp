//
// Created by losos on 06.04.2020.
//

#include "Database.h"
#include <SQLiteCpp/Database.h>
#include "DatabaseRow.h"
#include <iostream>

[[nodiscard]] std::string Database::selectAll(const std::string& tableName) {
	Database::checkIfTableExists(tableName);

	SQLite::Statement query(Database::m_homilyDatabase, "SELECT * FROM " + tableName);

	std::string result;
	while (query.executeStep()) {
		result += getDatabaseRowFromQuery(query).getStringHTML() + "<br>";
	}

	return result;
}

[[nodiscard]] DatabaseRow Database::selectSingleHomily(const unsigned int id, const std::string& tableName) {
	checkIfTableExists(tableName);

	SQLite::Statement query(Database::m_homilyDatabase, "SELECT * FROM " + tableName + " WHERE id=" + std::to_string(id));

	if (query.executeStep()) {
		return getDatabaseRowFromQuery(query);
	}

	throw std::runtime_error("No homily with that id");
}

[[nodiscard]] bool Database::insertNewHomily(DatabaseRow&& databaseRow, const std::string& tableName) {
	SQLite::Statement query(Database::m_homilyDatabase, "INSERT INTO " + tableName + " VALUES(null, \"" +
														databaseRow.getAuthor() + "\", \"" +
														databaseRow.getLink() + "\", \"" +
														databaseRow.getsiglumBeginning() + "\", \"" +
														databaseRow.getsiglumEnding() + "\")");

	return query.exec();
}

[[nodiscard]] bool Database::deleteOneHomily(unsigned int id, const std::string& tableName) {
	SQLite::Statement query(Database::m_homilyDatabase, "DELETE FROM " + tableName + " WHERE id=" + std::to_string(id));
	return query.exec();
}


[[nodiscard]] DatabaseRow Database::getDatabaseRowFromQuery(SQLite::Statement& query) {
	DatabaseRow databaseRow(query.getColumn(0),
							query.getColumn(1),
							query.getColumn(2),
							query.getColumn(3),
							query.getColumn(4));

	return databaseRow;
}

void Database::checkIfTableExists(const std::string& tableName) {
	const bool doesTableExist = Database::m_homilyDatabase.tableExists(tableName);
	if (!doesTableExist)
		throw std::runtime_error("No table named " + tableName);
}
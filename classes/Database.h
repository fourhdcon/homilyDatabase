//
// Created by losos on 06.04.2020.
//

#ifndef HOMILYDATABASE_DATABASE_H
#define HOMILYDATABASE_DATABASE_H

#include <SQLiteCpp/Database.h>
#include "DatabaseRow.h"
#include <iostream>

#ifdef Debug
#define LOG(x) std::cout << x;
#endif
#ifdef Release
#define LOG(x)
#endif

class Database {
public:
	static SQLite::Database m_homilyDatabase;

	Database() = default;

	/**
	 * @brief Gets all homilies data from database
	 *
	 * @exception	std::runtime_error		No table in database named as given
	 * @param		[in] tableName			String variable that contains name of table from which data will be downloaded
	 * @return								String with data format as in DatabaseRow::getString()
	 */
	[[nodiscard]] static std::string selectAll(const std::string& tableName = "homilies");

	/**
	 * @brief Gets homily data with specified id from database
	 *
	 * @exception	std::runtime_error		No table in database named as given
	 * @param 		[in] id					Integer variable that contains id of homily to be downloaded
	 * @param 		[in] tableName			String variable that contains name of table from which data will be downloaded
	 * @return 								DatabaseRow (/classes) element
	 * */
	[[nodiscard]] static DatabaseRow selectSingleHomily(unsigned int id, const std::string& tableName = "homilies");

	/**
	 * @brief Inserts data from given databaseRow into table named as given in tableName
	 *
	 * @param		[in] databaseRow		DatabaseRow (/classes) variable that contains data of homily to insert into database
	 * @param		[in] tableName			String variable that contains name of table to which data will be inserted
	 * @return								Bool: true if inserting was complete successful, false if not
	 * */
	[[nodiscard]] static bool insertNewHomily(DatabaseRow&& databaseRow, const std::string& tableName = "homilies");

	/**
	 * @brief Inserts data from given databaseRow into table named as given in tableName
	 *
	 * @param		[in] id					Integer variable that contains id of homily which is supposed to be deleted
	 * @param		[in] tableName			String variable that contains name of table to which data will be inserted
	 * @return								Bool: true if deleting was complete successful, false if not
	 * */
	[[nodiscard]] static bool deleteOneHomily(unsigned int id, const std::string& tableName = "homilies");

private:
	/**
	 * @brief Converts query data type to DatabaseRow type
	 *
	 * @param 		[in] query				SQLite::Statement variable that contains query data from execution
	 * @return 								DatabaseRow (/classes) element
	 */
	[[nodiscard]] static DatabaseRow getDatabaseRowFromQuery(SQLite::Statement& query);

	/**
	 * @brief Checks if table exists not in bool-method but in exception-method
	 *
	 * @exception	std::runtime_error		No table in database named as given
	 * @param 		[in] tableName			String variable that contains name of table to be checked and is supposed to be in
	 * databse
	 */
	static void checkIfTableExists(const std::string& tableName);
};

#endif //HOMILYDATABASE_DATABASE_H

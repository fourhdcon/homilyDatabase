#include <SQLiteCpp/SQLiteCpp.h>
#include "classes/Database.h"
#include "classes/RestbedServer.h"

#ifdef Debug
#define LOG(x) std::cout << x;
#endif
#ifdef Release
#define LOG(x)
#endif

//init database
SQLite::Database Database::m_homilyDatabase = SQLite::Database("homilyDatabase.db", SQLite::OPEN_READWRITE);

int main() {
	//start web server
	RestbedServer restbedServer;
	return 0;
}

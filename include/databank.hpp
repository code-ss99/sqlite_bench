#pragma once

#include <sqlite3.h>
#include <string>
#include <memory>

namespace DataRecorder::Data
{
    class DataBank{
        public:

            DataBank(const std::string& dbpath, const int flags);

            DataBank(const std::string& dbpath);

            int ExecuteSql(const std::string& query);

            //int ExecuteStep(std::shared_ptr<sqlite3_stmt> sqlStmt);

            int ExecuteStep(const std::string& query);

            bool TableExist(const std::string& tableName);

            DataBank(DataBank const& db) = delete;

            DataBank& operator =(DataBank const& db) = delete;

            ~DataBank() = default;

            struct Deleter
            {
                void operator()(sqlite3* sqlitePtr);
            };

        private:

            std::unique_ptr<sqlite3, Deleter> sqlite;
    };
} 
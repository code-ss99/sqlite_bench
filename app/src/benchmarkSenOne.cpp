#include <benchmarkSenOne.hpp>

BenchMarkSenOne::BenchMarkSenOne(const std::string& dbPath) : BenchMark(dbPath)
{

}


void BenchMarkSenOne::CreateTable()
{
    std::string sql = "CREATE TABLE IF NOT EXISTS RECORD("
    "datapoint_key	INT NOT NULL, "
    "timestamp	INT NOT NULL,"
    "value BLOB,"
    "PRIMARY KEY(datapoint_key,timestamp));";

    handle.ExecuteSql(sql);

    // create index
    sql = "CREATE INDEX record_inx ON RECORD ( "
                "datapoint_key	ASC, "
                "timestamp	ASC);";

    handle.ExecuteSql(sql);

    // create table metadata
    sql = "CREATE TABLE IF NOT EXISTS METADATA("
        "datapoint_key	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
        "datapoint_name TEXT,"
        "sw_id	INT NOT NULL,"
        "ecu_uuid INT NOT NULL,"
        "ts	INT,"
        "FOREIGN KEY(datapoint_key, ts) REFERENCES RECORD(datapoint_key,timestamp));";
    
    handle.ExecuteSql(sql);
}

void BenchMarkSenOne::WriteSingleDataP(){}

void BenchMarkSenOne::WriteBulkData(){}

void BenchMarkSenOne::QueryWithTime(){}

void BenchMarkSenOne::QueryDataWithinTime(){}

void BenchMarkSenOne::DeletDataOlderThan(){}

#include <databank.hpp>
#include <boost/format.hpp>
#include <iostream> 
#include <chrono>
#include <ctime> 
#include <thread>

void CreateTableSenOne(DataRecorder::Data::DataBank& db);

void CreateTableSenTwo(DataRecorder::Data::DataBank& db);

void WriteSingleDataPointSenOne(DataRecorder::Data::DataBank& db);

void WriteSingleDataPointSenTwo(DataRecorder::Data::DataBank& db);

void BulkWriteDataPointSenOne(DataRecorder::Data::DataBank& db);

void BulkWriteDataPointSenTwo(DataRecorder::Data::DataBank& db);

void QueryDataWithinTime(DataRecorder::Data::DataBank& db, int begin, int end);

void DeleteRowOlderThan(DataRecorder::Data::DataBank& db, int time);



void CalculateTime();

int main(int argc, char **argv) {
try
{
    std::cout << "begin bench 1" << std::endl;
    DataRecorder::Data::DataBank db1("/home/sspa/Desktop/dev/sqlite_bench/db/sehmafirst.db");

    CreateTableSenOne(db1);

    //write data point
    WriteSingleDataPointSenOne(db1);

    //write bulk 
    BulkWriteDataPointSenOne(db1);
    
    std::cout << "end bench 2" << std::endl;


    std::cout << "begin bench2" << std::endl;
    DataRecorder::Data::DataBank db2("/home/sspa/Desktop/dev/sqlite_bench/db/sehmasecond.db");
    
    CreateTableSenTwo(db2);

    WriteSingleDataPointSenTwo(db2);

    BulkWriteDataPointSenTwo(db2);
    
    std::cout << "end bench 2" << std::endl;
}
catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
}

    return (0);
}

void CreateTableSenOne(DataRecorder::Data::DataBank& db)
{
    std::string sql = "CREATE TABLE IF NOT EXISTS RECORD("
        "datapoint_key	INT NOT NULL, "
        "timestamp	INT NOT NULL,"
        "value BLOB,"
        "PRIMARY KEY(datapoint_key,timestamp));";


    db.ExecuteSql(sql);

    // create index
    sql = "CREATE INDEX record_inx ON RECORD ( "
                "datapoint_key	ASC, "
                "timestamp	ASC);";

    db.ExecuteSql(sql);

    // create table metadata
    sql = "CREATE TABLE IF NOT EXISTS METADATA("
        "datapoint_key	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
        "datapoint_name TEXT,"
        "sw_id	INT NOT NULL,"
        "ecu_uuid INT NOT NULL,"
        "ts	INT,"
        "FOREIGN KEY(datapoint_key, ts) REFERENCES RECORD(datapoint_key,timestamp));";
    
    db.ExecuteSql(sql);
}

void CreateTableSenTwo(DataRecorder::Data::DataBank& db)
{
    // create table metadata
    std::string sql = "CREATE TABLE IF NOT EXISTS METADATA("
        "datapoint_key	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, "
        "datapoint_name TEXT,"
        "sw_id	INT NOT NULL,"
        "ecu_uuid INT NOT NULL);";
    
    db.ExecuteSql(sql);

}

void WriteSingleDataPointSenOne(DataRecorder::Data::DataBank& db)
{
    //first write record than meta data
    std::string insert = "INSERT INTO RECORD"
                        "(datapoint_key, timestamp, value)"
                        "VALUES (1,1697702445,19);";

    db.ExecuteSql(insert);

    insert = "INSERT INTO METADATA"
            "(datapoint_key, datapoint_name, sw_id, ecu_uuid, ts)"
            "VALUES (1, 'flow_temp_circult_1', 10, 100, 1697702445);";

    db.ExecuteSql(insert);
}


void WriteSingleDataPointSenTwo(DataRecorder::Data::DataBank& db)
{
    //each datapoint key has a table
    std::string tableName = "RECORD_1";
    std::string sql = "CREATE TABLE IF NOT EXISTS " + tableName + "("
                        "timestamp	INT NOT NULL,"
                        "value BLOB,"
                        "PRIMARY KEY(timestamp));";
    

    db.ExecuteSql(sql);
    std::string insert = "INSERT INTO " + tableName + 
                        "(timestamp, value)"
                        "VALUES (1697702445,19);";

    db.ExecuteSql(insert);

    insert = "INSERT INTO METADATA"
            "(datapoint_key, datapoint_name, sw_id, ecu_uuid)"
            "VALUES (1, 'flow_temp_circult_1', 10, 100);";

    db.ExecuteSql(insert);
}

void BulkWriteDataPointSenOne(DataRecorder::Data::DataBank& db)
{
    std::string bulk = "INSERT INTO RECORD "
                        "(datapoint_key, timestamp, value) "
                        "VALUES";

    // write record
    int datapoint_key = 1;
    int timestamp = 1697785707;
    int data = 10;

    for(int i = 0; i < 10; i++){
        int cur = timestamp;
        for(int j = 0; j < 100; j++){
            cur++;
            data++;
            bulk += " (" + std::to_string(datapoint_key) + "," + std::to_string(cur) + "," + std::to_string(data) + "),";
        }

        datapoint_key++;
    }
    
    bulk.pop_back();

    bulk += ";";
    db.ExecuteSql(bulk);


    // write data
    bulk = "INSERT INTO METADATA "
                        "(datapoint_key, datapoint_name, sw_id, ecu_uuid, ts) "
                        "VALUES";

    int swid = 10;
    int uuid = 1000;
    datapoint_key = 2;
    timestamp = 1697785708;
    std::string dataPointName = "flow_temp_circult_";
    for(int i = 0; i < 10; i++){
        swid += datapoint_key;
        uuid += datapoint_key;
        bulk += " (" + std::to_string(datapoint_key) + "," + "'" + dataPointName + std::to_string(datapoint_key) + "'" + "," + std::to_string(swid) + 
                "," + std::to_string(uuid) + "," + std::to_string(timestamp) + "),";

        datapoint_key++;
    }

    bulk.pop_back();

    bulk += ";";
    db.ExecuteSql(bulk);
}

void BulkWriteDataPointSenTwo(DataRecorder::Data::DataBank& db)
{
    // for each data a table 
    int datapoint_key = 2;
    std::string tableName = "RECORD_";
    const std::string createTable = "CREATE TABLE IF NOT EXISTS  %1% ("
                        "timestamp INT NOT NULL,"
                        "value BLOB,"
                        "PRIMARY KEY(timestamp));";

    //write timestamp to datapoint table
    const std::string insertTimestamp = "INSERT INTO %1% " 
                                        "(timestamp, value) "
                                        "VALUES ";
    
    int timeStamp = time(nullptr);
    int value = 10;
    for(int i = 0; i < 10; i++){
        std::string curTable = tableName + std::to_string(datapoint_key);
        std::string sql = (boost::format(createTable) % curTable).str();
        db.ExecuteSql(sql);

        //insert row
        std::string bulk = (boost::format(insertTimestamp) %curTable).str();
        int cur = timeStamp;
        for(int j = 0; j < 100; j++){
            bulk += " (" + std::to_string(cur) + "," + std::to_string(value) + "),";;
            cur++;
        }

        bulk.pop_back();
        bulk += ";";
        db.ExecuteSql(bulk);

        datapoint_key++;
    }

    // write data
    std::string metaBulk = "INSERT INTO METADATA "
                            "(datapoint_key, datapoint_name, sw_id, ecu_uuid) "
                            "VALUES";

    int swid = 10;
    int uuid = 1000;
    datapoint_key = 2;
    std::string dataPointName = "flow_temp_circult_";
    for(int i = 0; i < 10; i++){
        swid += datapoint_key;
        uuid += datapoint_key;
        metaBulk += " (" + std::to_string(datapoint_key) + "," + "'" + dataPointName + std::to_string(datapoint_key) + "'" + "," + std::to_string(swid) + 
                "," + std::to_string(uuid) + "),";

        datapoint_key++;
    }

    metaBulk.pop_back();

    metaBulk += ";";
    db.ExecuteSql(metaBulk);

}
void CalculateTime()
{
    auto start = std::chrono::system_clock::now();

    auto end = std::chrono::system_clock::now();
 
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
 
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s"
              << std::endl;
}
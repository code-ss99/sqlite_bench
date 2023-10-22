#include <benchmark.hpp>

class BenchMarkSenOne : public BenchMark{
    public:

        BenchMarkSenOne(const std::string& dbPath);

        void CreateTable();

        void WriteSingleDataP();

        void WriteBulkData();

        void QueryWithTime();

        void QueryDataWithinTime();

        void DeletDataOlderThan();
};

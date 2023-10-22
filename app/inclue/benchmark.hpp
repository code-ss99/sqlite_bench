#pragma once

#include <databank.hpp>
#include <iostream> 
#include <chrono>

namespace APP
{
    class BenchMark{
        public:

            BenchMark(DataRecorder::Data::DataBank& db);

            BenchMark(BenchMark const& db) = delete;

            BenchMark& operator =(BenchMark const& db) = delete;

            ~BenchMark() = default;

            virtual void CreateTable();

            virtual void WriteSingleDataP();

            virtual void WriteBulkData();

            virtual void QueryWithTime();

            virtual void QueryDataWithinTime();

            virtual void DeletDataOlderThan();

        private:


    };
} 
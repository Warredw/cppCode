#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <memory>
#include "TimeTableManager.h"
#include <chrono>





int main() {

    auto startTime = std::chrono::high_resolution_clock::now();

   
    std::shared_ptr<TeamManager> teamManager = std::make_shared<TeamManager>();
    TimeTableManager tableManager(teamManager);

    std::string hapSetArray[] = {"22222221", "31222122", "31222212", "31222221", "32122212", 
                                  "32122221", "32212221", "31223121", "31231221", 
                                  "31312212", "31312221", "32122131", "32123121", 
                                  "32131221", "32213121", "31313121", "41213121"};

    int size = sizeof(hapSetArray) / sizeof(hapSetArray[0]); 
    std::map<std::string, int> totalSecondHalfSchedules; 
    std::map<std::string, int> hapSetCounts;
    std::map<std::string, std::vector<int>> varianceCalculator;

    for (int i = 0; i < 1; i++) { // Use the full size here
        HapSet hapSet(hapSetArray[i], "mirrored"); 
        std::vector<TimeTable> firstHalfTables = tableManager.makeNumberTablesFirsthalf(hapSet, 2000);

        for (const auto &firstHalfTable : firstHalfTables) {
            std::vector<TimeTable> secondHalfTables = tableManager.makeNumberTablesSecondHalf(hapSet, firstHalfTable, 100000);
            totalSecondHalfSchedules[hapSetArray[i]] += static_cast<int>(secondHalfTables.size());
            varianceCalculator[hapSetArray[i]].push_back(secondHalfTables.size());
        }
        hapSetCounts[hapSetArray[i]] += static_cast<int>(firstHalfTables.size());
    }

    std::map<std::string, double> averageSchedule; 
    std::map<std::string, double> stddevSchedule; 
    std::map<std::string, std::pair<double, double>> confidenceIntervals;

    for (const auto &hapSet : totalSecondHalfSchedules) {
        if (hapSetCounts[hapSet.first] > 0) {
            averageSchedule[hapSet.first] = static_cast<double>(hapSet.second) / hapSetCounts[hapSet.first]; 
            
            // Calculate standard deviation
            double sum = 0.0;
            for (const auto &value : varianceCalculator[hapSet.first]) {
                sum += (value - averageSchedule[hapSet.first]) * (value - averageSchedule[hapSet.first]);
            }
            double stddev = std::sqrt(sum / hapSetCounts[hapSet.first]);
            stddevSchedule[hapSet.first] = stddev;

            // Calculate confidence interval
            double z = 1.96; // Z-score for 95% confidence
            double marginOfError = z * (stddev / std::sqrt(hapSetCounts[hapSet.first]));
            confidenceIntervals[hapSet.first] = {averageSchedule[hapSet.first] - marginOfError, averageSchedule[hapSet.first] + marginOfError};
        }
    }

    // Print results
    std::cout << "Average number of schedules for the second half: " << std::endl;
    for (const auto &entry : averageSchedule) {
        const auto &ci = confidenceIntervals[entry.first];
        std::cout << "HapSet: " << entry.first 
                  << ", Average schedules: " << entry.second 
                  << ", 95% Confidence Interval: [" 
                  << ci.first << ", " << ci.second << "]" << std::endl;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    // Calculate elapsed time
    std::chrono::duration<double> elapsed = endTime - startTime;

    // Print execution time
    std::cout << "Execution time: " << elapsed.count() << " seconds." << std::endl;

    return 0; 
}

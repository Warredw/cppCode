#ifndef CPPCODE_TIMETABLEMANAGER_H
#define CPPCODE_TIMETABLEMANAGER_H
#include <memory>
#include "TeamManager.h"
#include "TimeTable.h"
#include "HapSet.h"
#include <random>


class TimeTableManager {

private:
    std::shared_ptr<TeamManager> teamManager;


public:
    explicit TimeTableManager(std::shared_ptr<TeamManager> teamManager);

    TimeTable makeTableFirstHalf(const HapSet& hapSet) const;
    TimeTable makeTableSecondHalf(const HapSet& hapSet, const TimeTable& tableFirstHalf) const;
    vector<TimeTable> makeNumberTablesFirsthalf(const HapSet& hapSet, int number);
    vector<TimeTable> makeNumberTablesSecondHalf(const HapSet& hapSet, const TimeTable& tableFirstHalf, int number) const;

    void addPlayOnceConstraint(IloEnv& env, IloModel& model , const vector<vector<vector<IloNumVar>>>&x) const;
    void addHAPConstraint(IloModel& model, IloEnv& env, const HapSet& hapSet, const vector<vector<vector<IloNumVar>>>& x, bool firstHalf) const; 
    void addOppositeVenueConstraint(IloModel& model, IloEnv& env, const TimeTable& tableFirsthalf, const vector<vector<vector<IloNumVar>>>& x ) const;
    TimeTable extractTableFromSolution(IloCplex& cplex, vector<vector<vector<IloNumVar>>>& x, std::shared_ptr<TeamManager> teamManager) const;
    vector<vector<vector<IloNumVar>>> initializeDecisionVariables(IloEnv& env) const;
};

#endif //CPPCODE_TIMETABLEMANAGER_H

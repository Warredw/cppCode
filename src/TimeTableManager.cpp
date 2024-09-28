#include "TimeTableManager.h"

#include <utility>


TimeTableManager::TimeTableManager(std::shared_ptr<TeamManager> teamManager) : teamManager(std::move(teamManager)) {
}

TimeTable TimeTableManager::makeTableFirstHalf(const HapSet &hapSet) const {

    IloEnv env;
    IloModel model(env);

    // 3d array for decision variables
    auto x = initializeDecisionVariables(env);
    addPlayOnceConstraint(env, model, x); 
    addHAPConstraint(model, env, hapSet, x, true);

    IloCplex cplex(model);
    if (cplex.solve()) {
        std::cout << "Solution found!" << std::endl;
    } else {
        std::cout << "No solution found." << std::endl;
    }
    TimeTable table = extractTableFromSolution(cplex, x, teamManager);
    env.end();
    return table;
}


vector<TimeTable> TimeTableManager::makeNumberTablesFirsthalf(const HapSet& hapSet, int number) {

    IloEnv env;
    IloModel model(env);
    vector<TimeTable> allTimeTablesSecondHalf;
    int numberSchedulesPossible;

    // 3D-array for decision variables (decision variable is whether a teams plays at home against another team in a given round)
    auto x = initializeDecisionVariables(env);

    // Constraint that team plays every other team exactly once
    addPlayOnceConstraint(env, model, x);
    addHAPConstraint(model, env, hapSet, x, true);

  const int numberOfRuns = number; // Number of runs to generate diverse solutions

    for (int i = 0; i < numberOfRuns; ++i) {
        IloCplex cplex(model);
        cplex.setParam(IloCplex::Param::RandomSeed, static_cast<int>(std::time(nullptr) + i)); // Set different random seeds
        cplex.setParam(IloCplex::Param::MIP::Limits::Populate, number);  // Set the limit for the number of solutions

        if (cplex.solve()) {
            // Extract and store the solution
            TimeTable table = extractTableFromSolution(cplex, x, teamManager);
            allTimeTablesSecondHalf.push_back(std::move(table));
        } else {
            std::cout << "No solution found for run " << i + 1 << std::endl;
        }
    }


    /*
    if (cplex.populate()) {
        int numFeasibleSolutions = static_cast<int>(cplex.getSolnPoolNsolns()); // Get the number of feasible solutions
        numberSchedulesPossible = numFeasibleSolutions;
        std::cout << "Number of feasible solutions found: " << numFeasibleSolutions << std::endl;
        allTimeTablesSecondHalf.reserve(numFeasibleSolutions);
    
        
        
        for (int sol = 0; sol < numFeasibleSolutions; sol++) {
            TimeTable table = extractTableFromSolution(cplex, x, teamManager);
            allTimeTablesSecondHalf.push_back(std::move(table));     
        }

    } else {

        std::cout << "No solution found." << std::endl;
    }

    */
    env.end();
    return allTimeTablesSecondHalf;
}


TimeTable TimeTableManager::makeTableSecondHalf(const HapSet& hapSet, const TimeTable& tableFirstHalf) const {
    IloEnv env;
    IloModel model(env);

    auto x = initializeDecisionVariables(env);
    addPlayOnceConstraint(env, model, x);
    addHAPConstraint(model, env, hapSet, x, false);
    addOppositeVenueConstraint(model, env, tableFirstHalf, x);

    IloCplex cplex(model);
    if (cplex.solve()) {
        std::cout << "Solution found!" << std::endl;

        // Extract the table from the solution
        return extractTableFromSolution(cplex, x, teamManager);
    } else {
        std::cout << "No solution found." << std::endl;
        return TimeTable(teamManager); // Return an empty TimeTable or handle as needed
    }
}


vector<TimeTable> TimeTableManager::makeNumberTablesSecondHalf(const HapSet &hapSet, const TimeTable &tableFirstHalf, int number) const {
    IloEnv env;
    IloModel model(env);
    vector<TimeTable> allTimeTablesSecondHalf;

    auto x = initializeDecisionVariables(env);
    addPlayOnceConstraint(env, model, x);
    addHAPConstraint(model, env, hapSet, x, false);
    addOppositeVenueConstraint(model, env, tableFirstHalf, x);

    IloCplex cplex(model);
    cplex.setParam(IloCplex::Param::MIP::Limits::Populate, number);

    if (cplex.populate()) {
        int numFeasibleSolutions = static_cast<int>(cplex.getSolnPoolNsolns());
        std::cout << "Number of feasible solutions found: " << numFeasibleSolutions << std::endl;
        allTimeTablesSecondHalf.reserve(numFeasibleSolutions);

        for (int sol = 0; sol < numFeasibleSolutions; sol++) {
            // Extract the solution into a new table
            TimeTable table = extractTableFromSolution(cplex, x, teamManager);
            allTimeTablesSecondHalf.push_back(std::move(table));
        }
    } else {
        std::cout << "No solution found." << std::endl;
    }
    env.end();
    return allTimeTablesSecondHalf; 
}





vector<vector<vector<IloNumVar>>> TimeTableManager::initializeDecisionVariables(IloEnv& env) const {
    vector<vector<vector<IloNumVar>>> x(teamManager->getNumberTeams());
    for (int i = 0; i < teamManager->getNumberTeams(); i++) {
        x[i].resize(teamManager->getNumberTeams());
        for (int j = 0; j < teamManager->getNumberTeams(); j++) {
            x[i][j].resize(teamManager->getNumberRounds());
            for (int z = 0; z < teamManager->getNumberRounds(); z++) {
                x[i][j][z] = IloNumVar(env, 0, 1, ILOBOOL);
            }
        }
    }
    return x;
}

void TimeTableManager::addPlayOnceConstraint(IloEnv& env, IloModel& model , const vector<vector<vector<IloNumVar>>>&x) const {
    for (int i = 0; i < teamManager->getNumberTeams(); i++) {
        for (int j = i + 1; j < teamManager->getNumberTeams(); j++) {
            IloExpr expr(env);
            for (int s = 0; s < teamManager->getNumberRounds(); s++) {
                expr += x[i][j][s] + x[j][i][s]; 
            }
            model.add(IloRange(env, 1, expr, 1));
            expr.end();
        }
    }
}

void TimeTableManager::addHAPConstraint(IloModel& model, IloEnv& env, const HapSet& hapSet, const vector<vector<vector<IloNumVar>>>& x, bool firstHalf) const {
    const auto& hap = firstHalf ? hapSet.getHapSetFirstHalf() : hapSet.getHapSetSecondHalf();

    for (int i = 0; i < teamManager->getNumberTeams(); i++) {
        for (int s = 0; s < teamManager->getNumberRounds(); s++) {
            IloExpr homeExpr(env), awayExpr(env);
            for (int j = 0; j < teamManager->getNumberTeams(); j++) {
                if (i != j) {
                    homeExpr += x[i][j][s];
                    awayExpr += x[j][i][s];
                }
            }
            model.add(IloRange(env, hap[i][s], homeExpr, hap[i][s]));
            model.add(IloRange(env, 1 - hap[i][s], awayExpr, 1 - hap[i][s]));
            homeExpr.end();
            awayExpr.end();
        }
    }
}

TimeTable TimeTableManager::extractTableFromSolution(IloCplex& cplex, vector<vector<vector<IloNumVar>>>& x,
                                                     std::shared_ptr<TeamManager> teamManager) const {
    TimeTable table(teamManager);
    table.resizeRounds(teamManager->getNumberRounds());
    table.resizeHomeAwayGames(teamManager);

    for (int s = 0; s < teamManager->getNumberRounds(); s++) {
        for (int i = 0; i < teamManager->getNumberTeams(); i++) {
            for (int j = 0; j < teamManager->getNumberTeams(); j++) {
                if (i != j && cplex.getValue(x[i][j][s]) == 1) {
                    Match match(teamManager->getTeams()[i], teamManager->getTeams()[j]);
                    table.addMatchToRound(s, match);
                    table.setHomeAwayGamesValue(i, j, s, 1);
                }
            }
        }
    }
    return table;
}

void TimeTableManager::addOppositeVenueConstraint(IloModel& model, IloEnv& env, const TimeTable& tableFirstHalf,
                                                  const vector<vector<vector<IloNumVar>>>& x) const {
    for (int i = 0; i < teamManager->getNumberTeams(); i++) {
        for (int j = 0; j < teamManager->getNumberTeams(); j++) {
            if (i != j) {
                IloExpr expr1(env), expr2(env);
                int homeTeam = 0;

                for (int s = 0; s < teamManager->getNumberRounds(); s++) {
                    homeTeam += tableFirstHalf.getHomeAwayGamesValue(i, j, s);
                    expr1 += x[i][j][s];
                    expr2 += x[j][i][s];
                }

                if (homeTeam == 1) {
                    model.add(expr1 == 0);
                    model.add(expr2 == 1);
                } else {
                    model.add(expr1 == 1);
                    model.add(expr2 == 0);
                }

                expr1.end();
                expr2.end();
            }
        }
    }
}

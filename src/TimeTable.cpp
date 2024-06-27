#include "TimeTable.h"

TimeTable::TimeTable() : hapSet("31222221"){
    teams = new Team[hapSet.nrTeams];
    setTeams();
    makeScheduleFirstHalf();
    makeAllSchedulesSecondHalf();
}

TimeTable::~TimeTable() {
    delete[] teams;
}

void TimeTable::setTeams() const {
    teams[0] = Team("Union");
    teams[1] = Team("Anderlecht");
    teams[2] = Team  ("Antwerp");
    teams[3] = Team("Club Brugge");
    teams[4] = Team("Cercle Brugge");
    teams[5] = Team("Genk");
    teams[6] = Team("Gent");
    teams[7] = Team("Mechelen");
    teams[8] = Team("STVV");
    teams[9] = Team("Standard");
    teams[10] = Team("Westerlo");
    teams[11] = Team("OHL");
    teams[12] = Team("Charleroi");
    teams[13] = Team("Eupen");
    teams[14] = Team("Kortrijk");
    teams[15] = Team("Molenbeek");
}


void TimeTable::makeScheduleFirstHalf() {
    IloEnv env;
    IloModel model(env);

    // 3D-array for decision variables (decision variable is whether a teams plays at home against another team in a given round)
    vector<vector<vector<IloNumVar>>> x(hapSet.nrTeams);
    for (int i = 0; i < hapSet.nrTeams; i++) {
        x[i].resize(hapSet.nrTeams);
        for (int j = 0; j < hapSet.nrTeams; j++) {
            x[i][j].resize(hapSet.nrTeams - 1);
            for (int z = 0; z < hapSet.nrTeams - 1; z++) {
                x[i][j][z] = IloNumVar(env, 0, 1, ILOBOOL); // Decision variable can be either 1 or 0
            }
        }
    }

    // Constraint that team plays every other team exactly once
    for (int i = 0; i < hapSet.nrTeams; i++) {
        for (int j = i+1; j < hapSet.nrTeams; j++) {
                IloExpr expr(env);
                for (int s = 0; s < hapSet.nrTeams - 1; s++) {
                    expr += x[i][j][s] + x[j][i][s];
                }
                model.add(IloRange(env, 1, expr, 1));
                expr.end();
            }
        }

    // Constraint that each team plays according to its HAP
    for (int i = 0; i < hapSet.nrTeams; i++) {
        for (int s = 0; s < hapSet.nrTeams - 1; s++) {
            IloExpr homeExpr(env);
            IloExpr awayExpr(env);
            for (int j = 0; j < hapSet.nrTeams; j++) {
                if (i != j) {
                    homeExpr += x[i][j][s];
                    awayExpr += x[j][i][s];
                }
            }
            model.add(IloRange(env, hapSet.hapSetFirstHalf[i][s], homeExpr, this->hapSet.hapSetFirstHalf[i][s]));
            model.add(IloRange(env, 1 - hapSet.hapSetFirstHalf[i][s], awayExpr, 1 - hapSet.hapSetFirstHalf[i][s]));
            homeExpr.end();
            awayExpr.end();
        }
    }
    IloCplex cplex(model);
    if (cplex.solve()) {
        std::cout << "Solution found!" << std::endl;
    } else {
        std::cout << "No solution found." << std::endl;
    }

    // Resize timeTableFirstHalf
    timeTableFirstHalf.schedule.resize(hapSet.nrTeams-1);

    // Resize homeAwayGames
    homeAwayGames.resize(hapSet.nrTeams);
    for(int i = 0; i < hapSet.nrTeams; i++) {
        homeAwayGames[i].resize(hapSet.nrTeams);
        for(int j = 0; j < hapSet.nrTeams; j++) {
            homeAwayGames[i][j].resize(hapSet.nrTeams-1);
        }
    }

    // Store the results in timeTableFirstHalf and homeAwayGames
    for(int s = 0; s < hapSet.nrTeams-1; s++) {
        for(int i = 0; i < hapSet.nrTeams; i++) {
            for(int j = 0; j < hapSet.nrTeams; j++) {
                if(i != j) {
                    if(cplex.getValue(x[i][j][s]) == 1) {
                        // Initialize Match
                        Match match(teams[i], teams[j]);
                        // add match to schedule homeAwayGames
                        timeTableFirstHalf.schedule[s].round.push_back(match);
                        homeAwayGames[i][j][s] = 1;
                    }
                }
            }
        }
    }

    // Save the schedule in a CSV file in a cross table format
    std::ofstream file("C:/Users/dewae/Documents/thesis/cppCode/scheduleFirstHalf.csv");

    // Write the header row
    file << "\"Team\"";
    for (int s = 0; s < hapSet.nrTeams - 1; s++) {
        file << ",\"Round " << s + 1 << "\"";
    }
    file << "\n";

    // Write the schedule data in a cross table format
    for (int i = 0; i < hapSet.nrTeams; i++) {
        file << "\"" << teams[i].name << "\"";
        for (int s = 0; s < hapSet.nrTeams - 1; s++) {
            string cell_content;
            bool matchFound = false;
            for (const auto& match : timeTableFirstHalf.schedule[s].round) {
                if (match.homeTeam == &teams[i]) {
                    cell_content = "Home vs " + match.awayTeam->name;
                    matchFound = true;
                    break;
                } else if (match.awayTeam == &teams[i]) {
                    cell_content = "Away vs " + match.homeTeam->name;
                    matchFound = true;
                    break;
                }
            }
            file << ",\"" << (matchFound ? cell_content : "") << "\"";
        }
        file << "\n";
    }

    file.close();
    env.end();
}


void TimeTable::makeAllSchedulesSecondHalf() {
    IloEnv env;
    IloModel model(env);

    // 3D-array for decision variables (decision variable is whether a teams plays at home against another team in a given round)
    vector<vector<vector<IloNumVar>>> x(hapSet.nrTeams);
    for (int i = 0; i < hapSet.nrTeams; i++) {
        x[i].resize(hapSet.nrTeams);
        for (int j = 0; j < hapSet.nrTeams; j++) {
            x[i][j].resize(hapSet.nrTeams - 1);
            for (int z = 0; z < hapSet.nrTeams - 1; z++) {
                x[i][j][z] = IloNumVar(env, 0, 1, ILOBOOL); // Decision variable can be 1 or 0
            }
        }
    }

    // Constraint that team plays every other team exactly once
    for (int i = 0; i < hapSet.nrTeams; i++) {
        for (int j = i+1; j < hapSet.nrTeams; j++) {
            IloExpr expr(env);
            for (int s = 0; s < hapSet.nrTeams - 1; s++) {
                expr += x[i][j][s] + x[j][i][s];
            }
            model.add(IloRange(env, 1, expr, 1));
            expr.end();
        }
    }

    // Constraint that each team plays according to its HAP
    for (int i = 0; i < hapSet.nrTeams; i++) {
        for (int s = 0; s < hapSet.nrTeams - 1; s++) {
            IloExpr homeExpr(env);
            IloExpr awayExpr(env);
            for (int j = 0; j < hapSet.nrTeams; j++) {
                if (i != j) {
                    homeExpr += x[i][j][s];
                    awayExpr += x[j][i][s];
                }
            }
            model.add(IloRange(env, hapSet.hapSetSecondHalf[i][s], homeExpr, this->hapSet.hapSetSecondHalf[i][s]));
            model.add(IloRange(env, 1 - hapSet.hapSetSecondHalf[i][s], awayExpr, 1 - hapSet.hapSetSecondHalf[i][s]));
            homeExpr.end();
            awayExpr.end();
        }
    }

    // Constraint that each match is played in the opposite venue compared to the first half
    for(int i = 0; i < hapSet.nrTeams; i++) {
        for(int j = 0; j < hapSet.nrTeams; j++) {
            if (i != j) {
                IloExpr expr1(env);
                IloExpr expr2(env);
                int homeTeam = 0;
                for (int s = 0; s < hapSet.nrTeams - 1; s++) {
                    homeTeam += homeAwayGames[i][j][s]; // homeTeam will be 1 if team i played at home against team j and 0 otherwise
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


    IloCplex cplex(model);
    cplex.setParam(IloCplex::Param::MIP::Tolerances::MIPGap, 0); // Set MIP gap tolerance to zero to ensure finding all feasible solutions

    if (cplex.populate()) {
        int numFeasibleSolutions = static_cast<int>(cplex.getSolnPoolNsolns()); // Get the number of feasible solutions
        numberSchedulesPossible = numFeasibleSolutions;
        std::cout << "Number of feasible solutions found: " << numFeasibleSolutions << std::endl;

        allTimeTablesSecondHalf.resize(numFeasibleSolutions);

        for (int sol = 0; sol < numFeasibleSolutions; sol++) {

            allTimeTablesSecondHalf[sol].schedule.resize(hapSet.nrTeams-1);
            // Extract the solution and store it
            for(int s = 0; s < hapSet.nrTeams-1; s++) {
                for(int i = 0; i < hapSet.nrTeams; i++) {
                    for(int j = 0; j < hapSet.nrTeams; j++) {
                        if(i != j) {
                            if(cplex.getValue(x[i][j][s], sol) == 1) {
                                Match match(teams[i], teams[j]);
                                allTimeTablesSecondHalf[sol].schedule[s].round.push_back(match);
                            }
                        }
                    }
                }
            }
        }
        // Save all schedules in CSV files
        for (int sol = 0; sol < numFeasibleSolutions; sol++) {
            std::ofstream file("C:/Users/dewae/Documents/thesis/cppCode/scheduleSecondHalf_" + std::to_string(sol) + ".csv");

            // Write the header row
            file << "\"Team\"";
            for (int s = 0; s < hapSet.nrTeams - 1; s++) {
                file << ",\"Round " << s + 1 << "\"";
            }
            file << "\n";

            // Write the schedule data in a cross table format
            for (int i = 0; i < hapSet.nrTeams; i++) {
                file << "\"" << teams[i].name << "\"";
                for (int s = 0; s < hapSet.nrTeams - 1; s++) {
                    string cell_content;
                    bool matchFound = false;
                    for (const auto& match : allTimeTablesSecondHalf[sol].schedule[s].round) {
                        if (match.homeTeam == &teams[i]) {
                            cell_content = "Home vs " + match.awayTeam->name;
                            matchFound = true;
                            break;
                        } else if (match.awayTeam == &teams[i]) {
                            cell_content = "Away vs " + match.homeTeam->name;
                            matchFound = true;
                            break;
                        }
                    }
                    file << ",\"" << (matchFound ? cell_content : "") << "\"";
                }
                file << "\n";
            }

            file.close();
        }
    } else {
        std::cout << "No solution found." << std::endl;
    }
    env.end();
}

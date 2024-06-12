#include "TimeTable.h"

TimeTable::TimeTable() : hapSet("22222221"){
    teams = new Team[hapSet.nrTeams];
    setTeams();
    makeScheduleFirstHalf();
    makeScheduleSecondHalf();
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
        for (int j = 0; j < hapSet.nrTeams; j++) {
            if (i != j) {
                IloExpr expr(env);
                for (int s = 0; s < hapSet.nrTeams - 1; s++) {
                    expr += x[i][j][s] + x[j][i][s];
                }
                model.add(IloRange(env, 1, expr, 1));
                expr.end();
            }
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

    // store the schedule inside timeTableFirstHalf
    timeTableFirstHalf.resize(hapSet.nrTeams-1);
    for(int s = 0; s < hapSet.nrTeams-1; s++) {
        for(int i = 0; i < hapSet.nrTeams; i++) {
            for(int j = 0; j < hapSet.nrTeams; j++) {
                if(i != j) {
                    if(cplex.getValue(x[i][j][s]) == 1) {
                        // Initialize Match
                        Match match(teams[i], teams[j]);
                        // add match to schedule
                        timeTableFirstHalf[s].push_back(match);
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
            for (const auto& match : timeTableFirstHalf[s]) {
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



void TimeTable::makeScheduleSecondHalf() {
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
        for (int j = 0; j < hapSet.nrTeams; j++) {
            if (i != j) {
                IloExpr expr(env);
                for (int s = 0; s < hapSet.nrTeams - 1; s++) {
                    expr += x[i][j][s] + x[j][i][s];
                }
                model.add(IloRange(env, 1, expr, 1));
                expr.end();
            }
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

    // Constraint that ensures teams play in opposite venues compared to the first half
    for(int s = 0; s < hapSet.nrTeams-1; s++) {
        for(int i = 0; i < hapSet.nrTeams; i++) {
            // look for the opponent in this round and store whether the team played at home or away against this opponent
            bool isHomeTeam;
            Team opponent;
            int opponentIndex;
            // go over all the matches per round to find the opponent
            for(const auto &match: timeTableFirstHalf[s]) {
                if(match.homeTeam == &teams[i]) {
                    opponent = *match.awayTeam;
                    isHomeTeam = true;

                    //find opponent index
                    for(int w = 0; w < hapSet.nrTeams; w++) {
                        if(teams[w] == opponent) opponentIndex = w;
                    }
                    break;
                }
                else if (match.awayTeam == &teams[i]) {
                    opponent = *match.homeTeam;
                    isHomeTeam = false;

                    //find opponent index
                    for(int w = 0; w < hapSet.nrTeams; w++) {
                        if(teams[w] == opponent) opponentIndex = w;
                    }
                    break;
                }
            }

            IloExpr homeExpr(env);
            IloExpr awayExpr(env);
            // Ensure that the team plays in the opposite venue
            for(int c = 0; c < hapSet.nrTeams - 1; c++) {
                homeExpr += x[i][opponentIndex][c];
                awayExpr += x[opponentIndex][i][c];
            }
            if(isHomeTeam) {
                model.add(homeExpr == 0);
                model.add(awayExpr == 1);
            }
            else{
                model.add(homeExpr == 1);
                model.add(awayExpr == 0);
            }
            homeExpr.end();
            awayExpr.end();
        }
    }

    IloCplex cplex(model);
    cplex.setParam(IloCplex::Param::MIP::Tolerances::MIPGap, 0); // Set MIP gap tolerance to zero to ensure finding all feasible solutions

    if (cplex.populate()) {
        int numFeasibleSolutions = static_cast<int>(cplex.getSolnPoolNsolns()); // Get the number of feasible solutions
        std::cout << "Number of feasible solutions found: " << numFeasibleSolutions << std::endl;
    } else {
        std::cout << "No solution found." << std::endl;
    }

    // store the schedule inside timeTableSecondHalf
    timeTableSecondHalf.resize(hapSet.nrTeams-1);
    for(int s = 0; s < hapSet.nrTeams-1; s++) {
        for(int i = 0; i < hapSet.nrTeams; i++) {
            for(int j = 0; j < hapSet.nrTeams; j++) {
                if(i != j) {
                    if(cplex.getValue(x[i][j][s]) == 1) {
                        // Initialize Match
                        Match match(teams[i], teams[j]);
                        // add match to schedule
                        timeTableSecondHalf[s].push_back(match);
                    }
                }
            }
        }
    }

    // Save the schedule in a CSV file in a cross table format
    std::ofstream file("C:/Users/dewae/Documents/thesis/cppCode/scheduleSecondHalf.csv");

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
            for (const auto& match : timeTableSecondHalf[s]) {
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







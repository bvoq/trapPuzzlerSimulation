#include <deque>
#include <iostream>
#include <queue>
#include <random>
#include <set>
#include <vector>
#include <ctime>
#include <cassert>
#include <mutex>
#include <thread>
#include <climits>

#define synchronized(m) \
for(std::unique_lock<std::recursive_mutex> lk(m); lk; lk.unlock())

using namespace std;

int minPlayers = 3;
int maxPlayers = 3;
int threadCount = 46;
int blocksPushForInstantWin = 3; //how much the field has to be widened for win position.
int limitBound = 100000; //much better
int enough = 27; //print anyways


//Currently ignored
int GLOBAL_DEPTH = 100;


std::recursive_mutex m_mutex;
int globalCounter = 4;


deque<deque<int> > initField(int h, int w) {
    deque<deque<int> > field(h, deque<int> (w,0));

    vector< vector<pair<int, int> > > stonesBlack = {
        {{0,0},{0,1}}, {{0,0}}, {{0,0}}, {{0,0},{1,0}}, {{0,0},{0,1},{1,0},{1,1}}
    };
    vector< vector<pair<int,int> > > stonesRed = stonesBlack;
    vector<vector<pair<int,int> > > stonesYellow = stonesBlack;
    int yellowBlocksToBePlaced = minPlayers+(rand()%(maxPlayers-minPlayers+1));
    int darkBlocksToBePlaced = 15 + (rand() % 15);
    int redBlocksToBePlaced = 10 + (rand()%10);

    while(yellowBlocksToBePlaced != 0) {
        //offsetY = h/2; offsetX = w/2;
        int offsetY = (rand() % (h-1))+1; int offsetX = (rand() % (w-1))+1;
        int randTile = rand() % stonesYellow.size();
        bool noAdd = false;
        for(int i = 0; i < stonesYellow[randTile].size(); ++i) {
            if(stonesYellow[randTile][i].first + offsetY >= h || stonesYellow[randTile][i].second + offsetX >= w ||
               field[stonesYellow[randTile][i].first + offsetY][stonesYellow[randTile][i].second + offsetX] != 0) {
                noAdd = true;
            }
        }
        if(noAdd == false) {
            for(int i = 0; i < stonesYellow[randTile].size(); ++i) {
                field[stonesYellow[randTile][i].first + offsetY][stonesYellow[randTile][i].second + offsetX] = yellowBlocksToBePlaced;
            }
            yellowBlocksToBePlaced--;
        }
    }
    int breakTries = 0;
    while(darkBlocksToBePlaced != 0 && breakTries < 100) {
        int offsetY = rand() % h; int offsetX = rand() % w;
        int randTile = rand() % stonesBlack.size();
        bool noAdd = false;
        for(int i = 0; i < stonesBlack[randTile].size(); ++i) {
            if(stonesBlack[randTile][i].first + offsetY >= h || stonesBlack[randTile][i].second + offsetX >= w ||
               field[stonesBlack[randTile][i].first + offsetY][stonesBlack[randTile][i].second + offsetX] != 0) {
                noAdd = true;
            }
        }
        if(noAdd == false) {
            for(int i = 0; i < stonesBlack[randTile].size(); ++i) {
                field[stonesBlack[randTile][i].first + offsetY][stonesBlack[randTile][i].second + offsetX] = 2000000+darkBlocksToBePlaced;
            }
            darkBlocksToBePlaced--;
        }
	breakTries++;
    }
	breakTries = 0;
    while(redBlocksToBePlaced !=0 && breakTries < 100) {
        int offsetY = rand() % h; int offsetX = rand() % w;
        int randTile = rand() % stonesRed.size();
        bool noAdd = false;
        for(int i = 0; i < stonesRed[randTile].size(); ++i) {
            if(stonesRed[randTile][i].first + offsetY >= h || stonesRed[randTile][i].second + offsetX >= w ||
               field[stonesRed[randTile][i].first + offsetY][stonesRed[randTile][i].second + offsetX] != 0) {
                noAdd = true;
            }
        }
        if(noAdd == false) {
            for(int i = 0; i < stonesRed[randTile].size(); ++i) {
                field[stonesRed[randTile][i].first + offsetY][stonesRed[randTile][i].second + offsetX] = 1000000+redBlocksToBePlaced;
            }
            redBlocksToBePlaced--;
        }
	breakTries++;
    }
    return field;
}






deque<deque<int> > initSymmetricField(int fh, int fw) {
	assert(fh % 2 == 1 && fh == fw && fw > 2); //remove for efficiency
    deque<deque<int> > field(fh, deque<int> (fw,0));

    vector< vector<pair<int, int> > > stonesBlack = {
        {{0,0},{0,1}}, {{0,0}}, {{0,0}}, {{0,0},{1,0}}, {{0,0},{0,1},{1,0},{1,1}}
    };
    vector< vector<pair<int,int> > > stonesRed = stonesBlack;
    vector<vector<pair<int,int> > > stonesYellow = stonesBlack;
    int yellowBlocksToBePlaced = 1;
	int darkBlocksToBePlaced = 7 + (rand() % 11) - 4;
    int redBlocksToBePlaced = 4 + (rand()%11) - 4;
   	int totBlocks = darkBlocksToBePlaced + redBlocksToBePlaced + 20;

	int w = fh/2, h = fw / 2;

	field[w][h] = 1;
	++w;++h;
   /*
    while(yellowBlocksToBePlaced != 0) {
        int offsetY = rand() % h; int offsetX = rand() % w;
        int randTile = rand() % stonesYellow.size();
        bool noAdd = false;
        for(int i = 0; i < stonesYellow[randTile].size(); ++i) {
            if(stonesYellow[randTile][i].first + offsetY >= h || stonesYellow[randTile][i].second + offsetX >= w ||
               field[stonesYellow[randTile][i].first + offsetY][stonesYellow[randTile][i].second + offsetX] != 0) {
                noAdd = true;
            }
        }
        if(noAdd == false) {
            for(int i = 0; i < stonesYellow[randTile].size(); ++i) {
                field[stonesYellow[randTile][i].first + offsetY][stonesYellow[randTile][i].second + offsetX] = yellowBlocksToBePlaced;
            }
            yellowBlocksToBePlaced--;
        }
    } */

    int breakTries = 0;
    while(darkBlocksToBePlaced != 0 && breakTries < 100) {
        int offsetY = rand() % h; int offsetX = rand() % w;
        int randTile = rand() % stonesBlack.size();
        bool noAdd = false;
        for(int i = 0; i < stonesBlack[randTile].size(); ++i) {
            if(stonesBlack[randTile][i].first + offsetY >= h || stonesBlack[randTile][i].second + offsetX >= w ||
               field[stonesBlack[randTile][i].first + offsetY][stonesBlack[randTile][i].second + offsetX] != 0) {
                noAdd = true;
            }
        }
        if(noAdd == false) {
            for(int i = 0; i < stonesBlack[randTile].size(); ++i) {
                field[stonesBlack[randTile][i].first + offsetY][stonesBlack[randTile][i].second + offsetX] = 2000000+darkBlocksToBePlaced;
            }
            darkBlocksToBePlaced--;
        }
	breakTries++;
    }
	breakTries = 0;
    while(redBlocksToBePlaced !=0 && breakTries < 100) {
        int offsetY = rand() % h; int offsetX = rand() % w;
        int randTile = rand() % stonesRed.size();
        bool noAdd = false;
        for(int i = 0; i < stonesRed[randTile].size(); ++i) {
            if(stonesRed[randTile][i].first + offsetY >= h || stonesRed[randTile][i].second + offsetX >= w ||
               field[stonesRed[randTile][i].first + offsetY][stonesRed[randTile][i].second + offsetX] != 0) {
                noAdd = true;
            }
        }
        if(noAdd == false) {
            for(int i = 0; i < stonesRed[randTile].size(); ++i) {
                field[stonesRed[randTile][i].first + offsetY][stonesRed[randTile][i].second + offsetX] = 1000000+redBlocksToBePlaced;
            }
            redBlocksToBePlaced--;
        }
	breakTries++;
    }
	//joint-symmetric mirroring
	bool joint = false;
	if(joint) {
	for(int i = 0; i <= h + 1; ++i) {
		for(int j = 0; j <= w + 1; ++j) {
			field[fh-1-i][j] = field[i][j];
			field[i][fw-1-j] = field[i][j];
			field[fh-1-i][fw-1-j] = field[i][j];
		}
	}
	}
	//normal symemtric mirroring
	else {
		for(int i = 0; i < fw; ++i) {
			field[h-1][i] = (field[h-1][i] < 10 ? 0 : totBlocks) + field[h-1][i];
		}
		for(int i = 0; i < fh; ++i) {
			field[i][w-1] = (field[i][w-1] < 10 ? 0 : totBlocks) + field[i][w-1];
		}
		totBlocks *= 2;
		for(int i = 0; i < h; ++i) {
			for(int j = 0; j < w; ++j) {
				//field[fh-1-i][j] = (field[i][j] < 10 ? 0 : totBlocks) + field[i][j];
				field[i][fw-1-j] = (field[i][j] < 10 ? 0 : totBlocks) + field[i][j];
				//field[fh-1-i][fw-1-j] = (field[i][j] < 10 ? 0 : totBlocks * 3) + field[i][j];
			}
		}
		for(int i = 0; i < h; ++i) {
			for(int j = 0; j < fw; ++j) {
				//field[fh-1-i][j] = (field[i][j] < 10 ? 0 : totBlocks) + field[i][j];
				field[fh-1-i][j] = (field[i][j] < 10 ? 0 : totBlocks * 4) + field[i][j];
				//field[fh-1-i][fw-1-j] = (field[i][j] < 10 ? 0 : totBlocks * 3) + field[i][j];
			}
		}
	}
    return field;
}






enum cellType {
    AIR=0, PLAYER=1, ENEMY=2, UNMOVABLE_ENEMY=3, SUPERAIR=4
};
enum keyType {
    UP, DOWN, LEFT, RIGHT
};

cellType getCellType(int ID) {
    if(ID == 0) return AIR;
    else if(ID > 0 && ID < 1000000) return PLAYER;
    else if(ID >= 1000000 && ID < 2000000) return ENEMY;
    else if(ID >= 2000000) return UNMOVABLE_ENEMY;
    return UNMOVABLE_ENEMY;
}

int moveTile(int elementId, keyType input, set<int>& checked, deque<deque<int> >& tempGrid, deque<deque<int> > & moveGrid) {
    checked.insert(elementId);
    for(int k = 0; k < tempGrid.size(); ++k) {
        for(int l = 0; l < tempGrid[k].size(); ++l) {
            if(moveGrid[k][l] == elementId) {
                moveGrid[k][l] = 0;
            }
        }
    }
    for(int k = 0; k < tempGrid.size(); ++k) {
        for(int l = 0; l < tempGrid[k].size(); ++l) {
            if(tempGrid[k][l] == elementId) {
                int i, j;
                if(input == UP) {i = k - 1; j = l;}
                else if(input == DOWN) {i = k + 1; j = l;}
                else if(input == LEFT) {i = k; j = l - 1;}
                else if(input == RIGHT) {i = k; j = l + 1;}
                if(i < moveGrid.size() && i >= 0 && j < moveGrid[0].size() && j >= 0){
                    cellType cT = getCellType(moveGrid[i][j]);
                    int cB = 0;
                    if(cT == ENEMY || cT == PLAYER) cB = moveTile(moveGrid[i][j], input, checked, tempGrid, moveGrid);
                    else if(cT == UNMOVABLE_ENEMY) return -1;
                    if(cB == -1) return -1;
                    moveGrid[i][j] = elementId;
                    tempGrid[k][l] = 0;
                }
            }
        }
    }
    return 0;
}

void cropBordersOf(deque<deque<int> > &tempGrid) {
    //This should maybe only happen when the size is less then 12
    bool upOrTop = false;
    /*while(tempGrid.size() < MIN_CELL_SIZE) {
     if(upOrTop) pushBackRowOf(tempGrid);
     else pushFrontRowOf(tempGrid);
     upOrTop = !upOrTop;
     }
     while(tempGrid[0].size() < MIN_CELL_SIZE) {
     if(upOrTop) pushBackColumnOf(tempGrid);
     else pushFrontColumnOf(tempGrid);
     upOrTop = !upOrTop;
     }*/
    bool isEmpty = true;
    bool emptyOverall = false;
    while(emptyOverall == false) {
        isEmpty = true;
        emptyOverall = true;
        //if( tempGrid.size() > MIN_CELL_SIZE) {
        for(int i = 0; i < tempGrid.back().size(); ++i) {
            if(tempGrid.back()[i] != 0) isEmpty = false;
        }
        if(isEmpty) {
            emptyOverall = false;
            tempGrid.pop_back();
        }
        isEmpty = true;
        for(int i = 0; i < tempGrid.front().size(); ++i) {
            if(tempGrid.front()[i] != 0) isEmpty = false;
        }
        if(isEmpty) {
            emptyOverall = false;
            tempGrid.pop_front();
        }
        //}
        //if(tempGrid[0].size() > MIN_CELL_SIZE) {
        isEmpty = true;
        for(int i = 0; i < tempGrid.size(); ++i) {
            if(tempGrid[i].back() != 0) isEmpty = false;
        }
        if(isEmpty) {
            emptyOverall = false;
            for(auto &it : tempGrid) it.pop_back();
        }
        isEmpty = true;
        for(int i = 0; i < tempGrid.size(); ++i) {
            if(tempGrid[i].front() != 0) isEmpty = false;
        }
        if(isEmpty) {
            emptyOverall = false;
            for(auto &it : tempGrid) it.pop_front();
        }
        //}
    }
}


bool move(keyType input, int playerID, deque<deque<int> > & moveGrid) {
    //INSTEAD CHECK THE TOP OF
    //moveGridX = gridX;
    //moveGridY = gridY;
    //moveGrid = grid;
    for(auto & it : moveGrid) {it.push_front(0); it.push_back(0);}
    moveGrid.push_front(deque<int>(moveGrid[0].size(),0));
    moveGrid.push_back(deque<int>(moveGrid[0].size(),0));

    deque<deque<int> > tempGrid = moveGrid;

    set<int> checked;
    set<pair<int, int> > eyesToChange;
    int theReturn = moveTile(playerID, input, checked, tempGrid, moveGrid);

    cropBordersOf(moveGrid);
    if(theReturn != -1) {
        return true;
    }
    else {
        return false;
    }
}

int bfsSolve (deque<deque<int> > input) {
    cropBordersOf(input);
    int playerID = 1;
    queue<pair<int, deque<deque<int> > > > q;
    q.push({1,input});
    set<deque<deque<int> > > alreadyPassed;
	int steps = 0;
    while(q.size() != 0) {
		steps++;
		cout << "it: " << steps << endl;
	if(q.size() > limitBound) return -1;
        pair<int, deque<deque<int> > > current = q.front();
        q.pop();
        if(alreadyPassed.count(current.second) == 0) {
            alreadyPassed.insert(current.second);
            deque<deque<int> > moveGrid = current.second;
            if(move(UP, playerID, moveGrid)) {
                if(current.second.size() > input.size()+blocksPushForInstantWin || current.second[0].size() > input[0].size()+blocksPushForInstantWin) return current.first;
                if(current.first < GLOBAL_DEPTH) {
                    q.push({current.first + 1, moveGrid});
                }
            }
            moveGrid = current.second;
            if(move(DOWN, playerID, moveGrid)) {
                if(current.second.size() > input.size()+blocksPushForInstantWin || current.second[0].size() > input[0].size()+blocksPushForInstantWin) return current.first;
                if(current.first < GLOBAL_DEPTH) {
                    q.push({current.first + 1, moveGrid});
                }
            }
            moveGrid = current.second;
            if(move(LEFT, playerID, moveGrid)) {
                if(current.second.size() > input.size()+blocksPushForInstantWin || current.second[0].size() > input[0].size()+blocksPushForInstantWin) return current.first;
                if(current.first < GLOBAL_DEPTH) {
                    q.push({current.first + 1, moveGrid});
                }
            }
            moveGrid = current.second;
            if(move(RIGHT, playerID, moveGrid)) {
                if(current.second.size() > input.size()+blocksPushForInstantWin || current.second[0].size() > input[0].size()+blocksPushForInstantWin) return current.first;
                if(current.first < GLOBAL_DEPTH) {
                    q.push({current.first + 1, moveGrid});
                }
            }
            moveGrid = current.second;
        }
    }
    return -1;
}

void levelExport(ostream & o, deque<deque<int> > copied) {
    cropBordersOf(copied);
    o << "{";
    for(int i = 0; i < copied.size(); ++i) {
        o << "{";
        for(int j = 0; j < copied[i].size(); ++j) {
            o << copied[i][j];
            if(j != copied[i].size() - 1) o << ",";
        }
        o << "}";
        if(i != copied.size() - 1) o << ",";
    }
    o << "}";
}



//calculates up to max step and then calculates step count * depth as output.
//-1 = unsolvable (trapped)
//-2 = no solution found in maxstep steps, but could still technically have a solution.
pair<long long,int> bfsSolveNeat (deque<deque<int> > input, int maxStep) {
	//levelExport(cout, input);
	//cout << endl << endl;
    cropBordersOf(input);
    //int playerID = 1;
    set<int> playerIDs;
    for(auto a : input){
        for(int b : a){
            if(b < 1000000) playerIDs.insert(b);
        }
    }
    queue<pair<int, deque<deque<int> > > > q;
    q.push({1,input});
    //long long t = time(0);
    set<deque<deque<int> > > alreadyPassed;
    set<deque<deque<int> > > dfsPassed;

	long long steps = 0;
    deque<deque<int> > moveGrid;
    while(q.size() != 0) {
		steps++;
		if(steps > maxStep) return {-2,-2}; //even more difficult then solvable
			pair<int, deque<deque<int> > > current = q.front();
			q.pop();
			for(int playerID : playerIDs){
                if(alreadyPassed.count(current.second) == 0) {
                    alreadyPassed.insert(current.second);
                    moveGrid = current.second;
                    if(move(UP, playerID, moveGrid)) {
                        //if(current.second.size() > input.size()+blocksPushForInstantWin || current.second[0].size() > input[0].size()+blocksPushForInstantWin) return {steps * current.first,current.first};
                        if(current.first < GLOBAL_DEPTH) {
                            q.push({current.first + 1, moveGrid});
                        }
                    }
                    moveGrid = current.second;
                    if(move(DOWN, playerID, moveGrid)) {
                        //if(current.second.size() > input.size()+blocksPushForInstantWin || current.second[0].size() > input[0].size()+blocksPushForInstantWin) return {steps * current.first,current.first};
                        if(current.first < GLOBAL_DEPTH) {
                            q.push({current.first + 1, moveGrid});
                        }
                    }
                    moveGrid = current.second;
                    if(move(LEFT, playerID, moveGrid)) {
                        //if(current.second.size() > input.size()+blocksPushForInstantWin || current.second[0].size() > input[0].size()+blocksPushForInstantWin) return {steps * current.first,current.first};
                        if(current.first < GLOBAL_DEPTH) {
                            q.push({current.first + 1, moveGrid});
                        }
                    }
                    moveGrid = current.second;
                    if(move(RIGHT, playerID, moveGrid)) {
                        //if(current.second.size() > input.size()+blocksPushForInstantWin || current.second[0].size() > input[0].size()+blocksPushForInstantWin) return {steps * current.first,current.first};
                        if(current.first < GLOBAL_DEPTH) {
                            q.push({current.first + 1, moveGrid});
                        }
                    }
                    moveGrid = current.second;
                }
			}

            //temp win check for multiple players
            vector<bool> out(playerIDs.size(), false);
            for(int i = 0; i < moveGrid.size(); ++i){
                for(int j = 0; j < moveGrid[0].size(); ++j){
                    if(moveGrid[i][j]-1 < 1000000){
                        if(!i || !j || i >= moveGrid.size()-1 || j >= moveGrid[0].size()-1) out[moveGrid[i][j]-1] = true;
                    }
                }
            }
            bool success = true;
            for(bool b : out){
                if(!b) success = false;
            }
            if(success){
                return make_pair(steps*current.first, current.first);
            }

            if(playerIDs.size() == 1){
                //dfs to check for cheapie
                queue<deque<deque<int> > > dfsQ;
                dfsQ.push(moveGrid);
                //cout << "dfs no: " << steps << endl;
                while(!dfsQ.empty()){
                    deque<deque<int> > moveGrid = dfsQ.front();
                    dfsQ.pop();
                    vector<set<pair<int, int> > > playerPosSet(playerIDs.size());
                    //add player positions to set
                    for(int i = 0; i < moveGrid.size(); ++i)
                        for(int j = 0; j < moveGrid[i].size(); ++j)
                            if(getCellType(moveGrid[i][j]) == PLAYER) playerPosSet[moveGrid[i][j]-1].insert(make_pair(i, j));

                    //cout << "player set done" << endl;
                    bool possible = false;
                    for(auto playerPos : playerPosSet){
                        bool up = true, down = true, right = true, left = true;
                        for(auto p : playerPos){
                            if(p.first && moveGrid[p.first-1][p.second] >=1000000) up = false;
                            if(p.first < moveGrid.size() -1 && moveGrid[p.first+1][p.second] >=1000000) down = false;
                            if(p.second && moveGrid[p.first][p.second-1] >=1000000) left = false;
                            if(p.second < moveGrid[p.first].size() -1 && moveGrid[p.first][p.second+1] >=1000000) right = false;
                        }
                        if(up){
                            deque<deque<int> > grid = moveGrid;
                            assert(move(UP, 1, grid)); //CHANGE FOR MULTIPLE PLAYERS
                            if(grid.size() != moveGrid.size() || grid[0].size() != moveGrid[0].size()) return {steps * current.first, current.first};
                            if(!dfsPassed.count(grid)){
                                dfsPassed.insert(grid);
                                dfsQ.push(grid);
                            }
                        }
                        if(down){
                            auto grid = moveGrid;
                            assert(move(DOWN, 1, grid)); //CHANGE FOR MULTIPLE PLAYERS
                            if(grid.size() != moveGrid.size() || grid[0].size() != moveGrid[0].size()) return {steps * current.first, current.first};
                            if(!dfsPassed.count(grid)){
                                dfsPassed.insert(grid);
                                dfsQ.push(grid);
                            }
                        }
                        if(left){
                            auto grid = moveGrid;
                            assert(move(LEFT, 1, grid)); //CHANGE FOR MULTIPLE PLAYERS
                            if(grid.size() != moveGrid.size() || grid[0].size() != moveGrid[0].size()) return {steps * current.first, current.first};
                            if(!dfsPassed.count(grid)){
                                dfsPassed.insert(grid);
                                dfsQ.push(grid);
                            }
                        }
                        if(right){
                            auto grid = moveGrid;
                            assert(move(RIGHT, 1, grid)); //CHANGE FOR MULTIPLE PLAYERS
                            if(grid.size() != moveGrid.size() || grid[0].size() != moveGrid[0].size()) return {steps * current.first, current.first};
                            if(!dfsPassed.count(grid)){
                                dfsPassed.insert(grid);
                                dfsQ.push(grid);
                            }
                        }
                    }
                }
			}
        }
    return {-1,-1};
}





long long globalBiggestSolved = 0;
long long globalBiggestSolvedDepth = 0;

void runThread(int threadID, int size, bool symmetric) {
    srand(time(0));
    long long biggestSolved = 0;
    long long biggestSolvedDepth = 0;
    //deque<deque<int> > debugLevel = {{0,0,105,105,0,0,0,0,0,0},{0,0,0,0,0,0,0,102,0,0},{0,0,1002,1002,0,0,0,102,0,0},{0,0,0,0,0,0,101,1001,0,0},{0,0,0,0,0,0,101,0,1003,0},{0,0,0,0,0,0,0,0,1003,0},{0,0,0,0,0,0,0,0,0,0},{103,103,0,0,0,0,0,0,0,1},{1004,0,0,0,0,0,104,0,0,1},{1004,0,0,0,0,0,0,0,0,0}};
    //int hi = bfsSolve(debugLevel);
    //cout << "SOLVED: " << hi << endl;

    int incrSuccess = 10000000 * threadID;

    int trapCount = 0;
    int tooLongCount = 0;

    for(long long i = 0; i < 10000000000000000; ++i) {
        int symFieldSize = size;
        int fieldSize = size;
		deque<deque<int> > field;
		if(!symmetric) field = initField(fieldSize, fieldSize);
		else field = initSymmetricField(symFieldSize, symFieldSize);

        pair<long long, int> solutionInfo = bfsSolveNeat(field, 1000000);
        long long hasSolved = solutionInfo.first;
        int depthSolved = solutionInfo.second ;
        if(trapCount == -1) trapCount++;
        else if(hasSolved == -2) tooLongCount++;
        //cout << hasSolved << endl;
        //cout << i << " " << hasSolved << endl;
        if(/*hasSolved >= enough ||*/ hasSolved >= biggestSolved || depthSolved >= biggestSolvedDepth) {
            if(hasSolved > biggestSolved) biggestSolved = hasSolved;
            if(depthSolved > biggestSolvedDepth) biggestSolvedDepth = depthSolved;

            synchronized(m_mutex) {
                if(hasSolved >= globalBiggestSolved || depthSolved >= globalBiggestSolvedDepth) {
                    globalCounter++;

                    if(hasSolved > globalBiggestSolved) globalBiggestSolved = hasSolved;
                    if(depthSolved > globalBiggestSolvedDepth) globalBiggestSolvedDepth = depthSolved;

                    levelExport(cout, field);
                    cout  << ";" << endl;
                    cout << "//" << hasSolved << " depth: " << depthSolved  << " traps: " << trapCount << " toolong: " << tooLongCount << " no.: " << globalCounter << " thread: " << threadID << endl;
                    cout << endl;
                }
            }
        }
    }
}


int main() {
    //runThread(73);

    thread threads[threadCount];
    cout << "Executing with: " << threadCount << " threads." << endl;
    int size = 10;
    for(int i = 0; i < threadCount; ++i) {
        threads[i] = thread(runThread,i,size,false);
    }
    threads[0].join();
    //arr[0].join(); //wait forever
    return 0;
}

#include <iostream>
#include <cstring>
#include "cleaning_robot.h"
using namespace std;

/*
   *    COMP 2011 2021 Fall Programming Assignment 2
   *    Student Name        : Luk Wang Lok
   *    Student ID          : 20679065
   *    Student ITSC email  : wlluk@connect.ust.hk
   * 
   * You are not allowed to use extra library
*/

// Please do all your work in this file. You just need to submit this file.

bool outOfBounds(int x, int y) {
   return ((x > MAP_WIDTH-1) || (y > MAP_HEIGHT-1) || (x < 0) || (y < 0));
}

bool isBlocked(const char map[MAP_HEIGHT][MAP_WIDTH], int x, int y) {
   return outOfBounds(x, y) ? false : map[y][x] == BLOCKED;
}

bool isCharger(const char map[MAP_HEIGHT][MAP_WIDTH], int x, int y) {
   return outOfBounds(x, y) ? false : map[y][x] == CHARGER;
}

bool isAvailable(const char map[MAP_HEIGHT][MAP_WIDTH], int x, int y) {
   return outOfBounds(x, y) ? false : map[y][x] == AVAILABLE;
}

bool hasVisited(const char map[MAP_HEIGHT][MAP_WIDTH], int x, int y) {
   return outOfBounds(x, y) ? false : map[y][x] == VISITED;
}

bool canVisit(const char map[MAP_HEIGHT][MAP_WIDTH], int x, int y) {
   return outOfBounds(x, y) ? false : map[y][x] == AVAILABLE || map[y][x] == CHARGER || map[y][x] == ROBOT;
}

bool isTarget(int robot_x, int robot_y, int target_x, int target_y) {
   return (robot_x == target_x) && (robot_y == target_y);
}

int checkCell(char map[MAP_HEIGHT][MAP_WIDTH], int x, int y, int full_energy, int &energy) {
   if (isCharger(map, x, y)) {
      energy = full_energy;
   }
   if (!hasVisited(map, x, y)) {
      map[y][x] = VISITED;
      return 1;
   }
   return 0;
}


int findMaximumPlace(int robot_x, int robot_y, int robot_energy, int robot_full_energy, 
                     char result_map[MAP_HEIGHT][MAP_WIDTH], char temp_map[MAP_HEIGHT][MAP_WIDTH]) {
   
   if (robot_x < MAP_WIDTH && robot_x >= 0 && robot_y < MAP_HEIGHT && robot_y >= 0) {

		if (temp_map[1][1] == 0) {
			result_map[robot_y][robot_x] = AVAILABLE;
			temp_map[0][0] = 0;
			temp_map[1][1] = 1;
		}

		if (isCharger(result_map, robot_x, robot_y)) {
			result_map[robot_y][robot_x] = VISITED;
			robot_energy = robot_full_energy;
         temp_map[0][0]++;
		}

		if (isAvailable(result_map, robot_x, robot_y)) {
			result_map[robot_y][robot_x] = VISITED;
			temp_map[0][0]++;
		}

		if (robot_energy > 0 && !isBlocked(result_map, robot_x, robot_y)) {
			findMaximumPlace(robot_x, robot_y-1, robot_energy-1, robot_full_energy, result_map, temp_map);
			findMaximumPlace(robot_x+1, robot_y, robot_energy-1, robot_full_energy, result_map, temp_map);
			findMaximumPlace(robot_x, robot_y+1, robot_energy-1, robot_full_energy, result_map, temp_map);
			findMaximumPlace(robot_x-1, robot_y, robot_energy-1, robot_full_energy, result_map, temp_map);
		}

		return (int)temp_map[0][0];
	}
};

// ** ------- Task 2 ------- ** //

void bfs(char map[MAP_HEIGHT][MAP_WIDTH], int queue[PA2_MAX_PATH][2], int &queueCount, int &walkedCount, int robot_x, int robot_y, 
                        int target_x, int target_y, int energy, int full_energy) {
   // cout << "bfs for (" << robot_x << "," << robot_y << ")" << endl;
   // if (energy > 0) {
      if (canVisit(map, robot_x, robot_y)) {
         // if (isCharger(map, robot_x, robot_y)) {
         //    energy = full_energy;
         // }
         map[robot_y][robot_x] = VISITED;
         walkedCount++;
         if (isTarget(robot_x, robot_y, target_x, target_y)) {
            return;
         } else {

            if (canVisit(map, robot_x-1, robot_y)) {
               queue[queueCount][0] = robot_x-1;
               queue[queueCount][1] = robot_y;
               queueCount++;
               // cout << "(" << robot_x-1 << "," << robot_y << ") enqueued" << endl;
            }
            if (canVisit(map, robot_x, robot_y+1)) {
               queue[queueCount][0] = robot_x;
               queue[queueCount][1] = robot_y+1;
               queueCount++;
               // cout << "(" << robot_x << "," << robot_y+1 << ") enqueued" << endl;
            }
            if (canVisit(map, robot_x+1, robot_y)) {
               queue[queueCount][0] = robot_x+1;
               queue[queueCount][1] = robot_y;
               queueCount++;
               // cout << "(" << robot_x+1 << "," << robot_y << ") enqueued" << endl;
            }
            if (canVisit(map, robot_x, robot_y-1)) {
               queue[queueCount][0] = robot_x;
               queue[queueCount][1] = robot_y-1;
               queueCount++;
               // cout << "(" << robot_x << "," << robot_y-1 << ") enqueued" << endl;
            }

            // if (isCharger(map, robot_x-1, robot_y)) {
            //    queue[queueCount][0] = robot_x-1;
            //    queue[queueCount][1] = robot_y;
            //    queueCount++;
            //    // cout << "(" << robot_x-1 << "," << robot_y << ") enqueued" << endl;
            // }
            // if (isCharger(map, robot_x, robot_y+1)) {
            //    queue[queueCount][0] = robot_x;
            //    queue[queueCount][1] = robot_y+1;
            //    queueCount++;
            //    // cout << "(" << robot_x << "," << robot_y+1 << ") enqueued" << endl;
            // }
            // if (isCharger(map, robot_x+1, robot_y)) {
            //    queue[queueCount][0] = robot_x+1;
            //    queue[queueCount][1] = robot_y;
            //    queueCount++;
            //    // cout << "(" << robot_x+1 << "," << robot_y << ") enqueued" << endl;
            // }
            // if (isCharger(map, robot_x, robot_y-1)) {
            //    queue[queueCount][0] = robot_x;
            //    queue[queueCount][1] = robot_y-1;
            //    queueCount++;
            //    // cout << "(" << robot_x << "," << robot_y-1 << ") enqueued" << endl;
            // }

            // if (canVisit(map, robot_x, robot_y-1)) {
            //    queue[queueCount][0] = robot_x;
            //    queue[queueCount][1] = robot_y-1;
            //    queueCount++;
            //    cout << "(" << robot_x << "," << robot_y-1 << ") enqueued" << endl;
            // }
            // if (canVisit(map, robot_x+1, robot_y)) {
            //    queue[queueCount][0] = robot_x+1;
            //    queue[queueCount][1] = robot_y;
            //    queueCount++;
            //    cout << "(" << robot_x+1 << "," << robot_y << ") enqueued" << endl;
            // }
            // if (canVisit(map, robot_x, robot_y+1)) {
            //    queue[queueCount][0] = robot_x;
            //    queue[queueCount][1] = robot_y+1;
            //    queueCount++;
            //    cout << "(" << robot_x << "," << robot_y+1 << ") enqueued" << endl;
            // }
            // if (canVisit(map, robot_x-1, robot_y)) {
            //    queue[queueCount][0] = robot_x-1;
            //    queue[queueCount][1] = robot_y;
            //    queueCount++;
            //    cout << "(" << robot_x-1 << "," << robot_y << ") enqueued" << endl;
            // }
            bfs(map, queue, queueCount, walkedCount, queue[walkedCount][0], queue[walkedCount][1], target_x, target_y, energy, full_energy);
         }
      } else if (hasVisited(map, robot_x, robot_y)) {
         walkedCount++;
         bfs(map, queue, queueCount, walkedCount, queue[walkedCount][0], queue[walkedCount][1], target_x, target_y, energy, full_energy);
      }
   // }
}

void linkPath(char map[MAP_HEIGHT][MAP_WIDTH], int queue[PA2_MAX_PATH][2], char result_sequence[], int i, int past_x, int past_y, 
                  int energy, int full_energy, int robot_x, int robot_y, int &pathCount) {
   if (isCharger(map, past_x, past_y)) {
      energy = full_energy;
   }
   if (energy < 0) {
      pathCount = PA2_MAX_PATH;
      return;
   } else if (i < 0) {

      if (robot_x == past_x && robot_y == past_y+1) {
         result_sequence[pathCount] = 'U';
      } 
      else if (robot_x == past_x-1 && robot_y == past_y) {
         result_sequence[pathCount] = 'R';
      }
      else if (robot_x == past_x && robot_y == past_y-1) {
         result_sequence[pathCount] = 'D';
      }
      else if (robot_x == past_x+1 && robot_y == past_y) {
         result_sequence[pathCount] = 'L';
      }

      pathCount++;
      return;
   } else {

      // if (queue[i][0] == past_x+1 && queue[i][1] == past_y) {
      //    result_sequence[pathCount] = 'L';
      //    pathCount++;
      //    // cout << "energy level: " << energy << endl;
      //    linkPath(map, queue, result_sequence, i-1, queue[i][0], queue[i][1], energy-1, full_energy, robot_x, robot_y, pathCount);
      // } 
      // else if (queue[i][0] == past_x && queue[i][1] == past_y-1) {
      //    result_sequence[pathCount] = 'D';
      //    pathCount++;
      //    // cout << "energy level: " << energy << endl;
      //    linkPath(map, queue, result_sequence, i-1, queue[i][0], queue[i][1], energy-1, full_energy, robot_x, robot_y, pathCount);
      // }
      // else if (queue[i][0] == past_x-1 && queue[i][1] == past_y) {
      //    result_sequence[pathCount] = 'R';
      //    pathCount++;
      //    // cout << "energy level: " << energy << endl;
      //    linkPath(map, queue, result_sequence, i-1, queue[i][0], queue[i][1], energy-1, full_energy, robot_x, robot_y, pathCount);
      // }
      // else if (queue[i][0] == past_x && queue[i][1] == past_y+1) {
      //    result_sequence[pathCount] = 'U';
      //    pathCount++;
      //    // cout << "energy level: " << energy << endl;
      //    linkPath(map, queue, result_sequence, i-1, queue[i][0], queue[i][1], energy-1, full_energy, robot_x, robot_y, pathCount);
      // }
      
      if (queue[i][0] == past_x && queue[i][1] == past_y+1 && isCharger(map, queue[i][0], queue[i][1])) {
         cout << "(" << queue[i][0] << "," << queue[i][1] << ") goes up to (" << past_x << "," << past_y << ")" << endl;
         result_sequence[pathCount] = 'U';
         pathCount++;
         cout << "energy level: " << energy << endl;
         linkPath(map, queue, result_sequence, i-1, queue[i][0], queue[i][1], energy-1, full_energy, robot_x, robot_y, pathCount);
      } 
      else if (queue[i][0] == past_x-1 && queue[i][1] == past_y && isCharger(map, queue[i][0], queue[i][1])) {
         cout << "(" << queue[i][0] << "," << queue[i][1] << ") goes right to (" << past_x << "," << past_y << ")" << endl;
         result_sequence[pathCount] = 'R';
         pathCount++;
         cout << "energy level: " << energy << endl;
         linkPath(map, queue, result_sequence, i-1, queue[i][0], queue[i][1], energy-1, full_energy, robot_x, robot_y, pathCount);
      }
      else if (queue[i][0] == past_x && queue[i][1] == past_y-1 && isCharger(map, queue[i][0], queue[i][1])) {
         cout << "(" << queue[i][0] << "," << queue[i][1] << ") goes down to (" << past_x << "," << past_y << ")" << endl;
         result_sequence[pathCount] = 'D';
         pathCount++;
         cout << "energy level: " << energy << endl;
         linkPath(map, queue, result_sequence, i-1, queue[i][0], queue[i][1], energy-1, full_energy, robot_x, robot_y, pathCount);
      }
      else if (queue[i][0] == past_x+1 && queue[i][1] == past_y && isCharger(map, queue[i][0], queue[i][1])) {
         cout << "(" << queue[i][0] << "," << queue[i][1] << ") goes left to (" << past_x << "," << past_y << ")" << endl;
         result_sequence[pathCount] = 'L';
         pathCount++;
         cout << "energy level: " << energy << endl;
         linkPath(map, queue, result_sequence, i-1, queue[i][0], queue[i][1], energy-1, full_energy, robot_x, robot_y, pathCount);
      }
      
      else if (queue[i][0] == past_x && queue[i][1] == past_y+1) {
         cout << "(" << queue[i][0] << "," << queue[i][1] << ") goes up to (" << past_x << "," << past_y << ")" << endl;
         result_sequence[pathCount] = 'U';
         pathCount++;
         cout << "energy level: " << energy << endl;
         linkPath(map, queue, result_sequence, i-1, queue[i][0], queue[i][1], energy-1, full_energy, robot_x, robot_y, pathCount);
      } 
      else if (queue[i][0] == past_x-1 && queue[i][1] == past_y) {
         cout << "(" << queue[i][0] << "," << queue[i][1] << ") goes right to (" << past_x << "," << past_y << ")" << endl;
         result_sequence[pathCount] = 'R';
         pathCount++;
         cout << "energy level: " << energy << endl;
         linkPath(map, queue, result_sequence, i-1, queue[i][0], queue[i][1], energy-1, full_energy, robot_x, robot_y, pathCount);
      }
      else if (queue[i][0] == past_x && queue[i][1] == past_y-1) {
         cout << "(" << queue[i][0] << "," << queue[i][1] << ") goes down to (" << past_x << "," << past_y << ")" << endl;
         result_sequence[pathCount] = 'D';
         pathCount++;
         cout << "energy level: " << energy << endl;
         linkPath(map, queue, result_sequence, i-1, queue[i][0], queue[i][1], energy-1, full_energy, robot_x, robot_y, pathCount);
      }
      else if (queue[i][0] == past_x+1 && queue[i][1] == past_y) {
         cout << "(" << queue[i][0] << "," << queue[i][1] << ") goes left to (" << past_x << "," << past_y << ")" << endl;
         result_sequence[pathCount] = 'L';
         pathCount++;
         cout << "energy level: " << energy << endl;
         linkPath(map, queue, result_sequence, i-1, queue[i][0], queue[i][1], energy-1, full_energy, robot_x, robot_y, pathCount);
      }
      else {
         linkPath(map, queue, result_sequence, i-1, past_x, past_y, energy, full_energy, robot_x, robot_y, pathCount);
      }
   }
}

int findShortestDistance(int robot_x, int robot_y, int target_x, int target_y, int robot_energy, 
                         int robot_full_energy, const char map[MAP_HEIGHT][MAP_WIDTH], char temp_map[MAP_HEIGHT][MAP_WIDTH]) {
   int queue[PA2_MAX_PATH][2];
   char result_sequence[PA2_MAX_PATH];
   int queueCount = 0;
   int pathCount = 1;
   int walkedCount = 0;

   copyMap(temp_map, map);
   if (canVisit(temp_map, target_x, target_y)) {
      bfs(temp_map, queue, queueCount, walkedCount, robot_x, robot_y, target_x, target_y, robot_energy, robot_full_energy);
      if (robot_x != target_x || robot_y != target_y) {
         linkPath(temp_map, queue, result_sequence, walkedCount, target_x, target_y, robot_energy, robot_full_energy, robot_x, robot_y, pathCount);
      }
   } else {
      pathCount = PA2_MAX_PATH;
   }
   return pathCount;
}

// ** ------- Task 3 ------- ** //

void reverse_arr(char original[], char target[], int size, int i){
   if(i >= size){
      return;
   }
   else{
      target[i] = original[size-i-1];
      reverse_arr(original, target, size, i+1);
   }
}

int findPathSequence(int robot_x, int robot_y, int target_x, int target_y, int robot_energy, 
                     int robot_full_energy, char result_sequence[], const char map[MAP_HEIGHT][MAP_WIDTH], char temp_map[MAP_HEIGHT][MAP_WIDTH]) {
   int queue[PA2_MAX_PATH][2];
   int queueCount = 0;
   int pathCount = 0;
   int walkedCount = 0;
   char tmp_sequence[PA2_MAX_PATH];

   copyMap(temp_map, map);
   if (canVisit(temp_map, target_x, target_y)) {
      bfs(temp_map, queue, queueCount, walkedCount, robot_x, robot_y, target_x, target_y, robot_energy, robot_full_energy);
      if (robot_x != target_x || robot_y != target_y) {
         linkPath(temp_map, queue, tmp_sequence, walkedCount, target_x, target_y, robot_energy, robot_full_energy, robot_x, robot_y, pathCount);
      }
      printMap(temp_map);
      reverse_arr(tmp_sequence, result_sequence, pathCount, 0);
      result_sequence[pathCount] = 'T';
      result_sequence[pathCount+1] = '\0';
      return pathCount+1;
   } else {
      return PA2_MAX_PATH;
   }
};

// ** ------- Task 4 ------- ** //

bool newCharger(int chargers[][2], int charger_count, int x, int y, int i) {
   if (i >= charger_count) {
      return true;
   } else {
      if (chargers[i][0] == x && chargers[i][1] == y) {
         return false;
      } else {
         return newCharger(chargers, charger_count, x, y, i+1);
      }
   }
};

void findReachableChargers(const char map[MAP_HEIGHT][MAP_WIDTH], int chargers[PA2_MAX_PATH][2],
                   int x, int y, int energy, int &charger_count) {
   
   if ((energy < 0) || outOfBounds(x,y)) {
      return;
   } else {
      if (isCharger(map, x, y)) {
         if (newCharger(chargers, charger_count, x, y, 0)) {
            chargers[charger_count][0] = x;
            chargers[charger_count][1] = y;
            charger_count++;
         }
      }
      if (!isBlocked(map, x, y)) {
         findReachableChargers(map, chargers, x, y-1, energy-1, charger_count);
         findReachableChargers(map, chargers, x+1, y, energy-1, charger_count);
         findReachableChargers(map, chargers, x, y+1, energy-1, charger_count);
         findReachableChargers(map, chargers, x-1, y, energy-1, charger_count);
      }
   }
};

void checkFarthest(int robot_x, int robot_y, int robot_energy, int robot_full_energy, const char map[MAP_HEIGHT][MAP_WIDTH], char temp_map[MAP_HEIGHT][MAP_WIDTH],
                         int chargers[PA2_MAX_PATH][2], int charger_count, int i, int &farthest_x, int &farthest_y, int &farthest_distance) {
   if (i >= charger_count) {
      return;
   } else {
      int distance = findShortestDistance(robot_x, robot_y, chargers[i][0], chargers[i][1], robot_energy, robot_full_energy, map, temp_map);
      if (farthest_distance < distance) {
         farthest_distance = distance;
         farthest_x = chargers[i][0];
         farthest_y = chargers[i][1];
      }
      checkFarthest(robot_x, robot_y, robot_energy, robot_full_energy, map, temp_map, chargers, charger_count, i+1, farthest_x, farthest_y, farthest_distance);
   }
};

int findFarthestPossibleCharger(int robot_x, int robot_y, int robot_original_x, int robot_original_y, 
                                int &target_x, int &target_y, int robot_energy, int robot_full_energy,
                                const char map[MAP_HEIGHT][MAP_WIDTH], char temp_map[MAP_HEIGHT][MAP_WIDTH]) {
   
   int chargers[MAP_HEIGHT*MAP_WIDTH][2];
   int charger_count = 0;
   int charger_x = 0;
   int charger_y = 0;
   int farthest_distance = 0;

   findReachableChargers(map, chargers, robot_x, robot_y, robot_energy, charger_count);
   if (charger_count <= 0) {
      return -1;
   } else {
      checkFarthest(robot_x, robot_y, robot_energy, robot_full_energy, map, temp_map, chargers, charger_count, 0, target_x, target_y, farthest_distance);
      return farthest_distance;
   }
}
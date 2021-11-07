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

int abs(int x) {
   if (x < 0) {
      return x*-1;
   } else {
      return x;
   }
}

void insert(int chargers[][3], int new_data[3], int i, bool inserted, int &charger_count) {
   if ((i < 0) || inserted) {
      return;
   }  else {
      if ((charger_count == 0) || (new_data[2] < chargers[i][2])) {
         cout << "Inserting charger at i=" << i << endl;
         chargers[i][0] = new_data[0];
         chargers[i][1] = new_data[1];
         chargers[i][2] = new_data[2];

         inserted = true;
         charger_count++;
      } else {
         chargers[i+1][0] = chargers[i][0];
         chargers[i+1][1] = chargers[i][1];
         chargers[i+1][2] = chargers[i][2];
      }
      insert(chargers, new_data, i-1, inserted, charger_count);
   }
}

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
   return outOfBounds(x, y) ? false : map[y][x] == AVAILABLE || map[y][x] == CHARGER;
}

int checkCell(char map[MAP_HEIGHT][MAP_WIDTH], int x, int y, int full_energy, int &energy) {
   if (isCharger(map, x, y)) {
      energy = full_energy;
   }
   if (!hasVisited(map, x, y)) {
      map[y][x] = VISITED;
      return 1;
   }
   return 1;
}

void findReachableChargers(const char map[MAP_HEIGHT][MAP_WIDTH], int chargers[PA2_MAX_PATH][2],
                   int x, int y, int energy, int &charger_count) {
   
   if ((energy < 0) || outOfBounds(x,y)) {
      cout << "Charger not found" << endl;
      return;
   } else {
      if (isCharger(map, x, y)) {
         cout << "Found charger at (" << x << "," << y << ")" << endl;
         chargers[charger_count][0] = x;
         chargers[charger_count][1] = y;
         charger_count++;
      }
      if (!isBlocked(map, x, y)) {
         findReachableChargers(map, chargers, x, y-1, energy-1, charger_count);
         findReachableChargers(map, chargers, x+1, y, energy-1, charger_count);
         findReachableChargers(map, chargers, x, y+1, energy-1, charger_count);
         findReachableChargers(map, chargers, x-1, y, energy-1, charger_count);
      }
   }
};

int findMaximumPlace(int robot_x, int robot_y, int robot_energy, int robot_full_energy, 
                     char result_map[MAP_HEIGHT][MAP_WIDTH], char temp_map[MAP_HEIGHT][MAP_WIDTH]) {
   
   int totalCount = 0;
   int upCount = 0;
   int rightCount = 0;
   int downCount = 0;
   int leftCount = 0;
   int increment = 0;

   // TODO : add in copy temp_map to count cases with chargers
   if (robot_energy < 0) {
      return 0;
   }
   if (!isBlocked(result_map, robot_x, robot_y)) {
      increment = checkCell(result_map, robot_x, robot_y, robot_full_energy, robot_energy);

      upCount = findMaximumPlace(robot_x, robot_y-1, robot_energy-1, robot_full_energy, result_map, temp_map);
      rightCount = findMaximumPlace(robot_x+1, robot_y, robot_energy-1, robot_full_energy, result_map, temp_map);
      downCount = findMaximumPlace(robot_x, robot_y+1, robot_energy-1, robot_full_energy, result_map, temp_map);
      rightCount = findMaximumPlace(robot_x-1, robot_y, robot_energy-1, robot_full_energy, result_map, temp_map);

      totalCount = upCount + rightCount + downCount + leftCount + increment;
   }
   return totalCount;
};

/** ------- Task 2 ------- **/

void bfs(char map[MAP_HEIGHT][MAP_WIDTH], int queue[PA2_MAX_PATH][2], int &queueCount, int &pathCount, int robot_x, int robot_y, 
                        int target_x, int target_y, int energy, int full_energy) {
   if (energy < 0) {
      pathCount = PA2_MAX_PATH;
      return;
   } else {
      if (canVisit(map, robot_x, robot_y)) {
         map[robot_y][robot_x] = VISITED;
         pathCount++;
         if (robot_x == target_x && robot_y == target_y) {
            return;
         } else {
            if (canVisit(map, robot_x, robot_y-1)) {
               queue[queueCount][0] = robot_x;
               queue[queueCount][1] = robot_y-1;
               queueCount++;
            }
            if (canVisit(map, robot_x+1, robot_y)) {
               queue[queueCount][0] = robot_x+1;
               queue[queueCount][1] = robot_y;
               queueCount++;
            }
            if (canVisit(map, robot_x, robot_y+1)) {
               queue[queueCount][0] = robot_x;
               queue[queueCount][1] = robot_y+1;
               queueCount++;
            }
            if (canVisit(map, robot_x-1, robot_y)) {
               queue[queueCount][0] = robot_x-1;
               queue[queueCount][1] = robot_y;
               queueCount++;
            }
            bfs(map, queue, queueCount, queue[queueCount-1][0], queue[queueCount-1][1], target_x, target_y, energy-1, full_energy, pathCount);
         }
      }
   }
}

int findShortestDistance(int robot_x, int robot_y, int target_x, int target_y, int robot_energy, 
                         int robot_full_energy, const char map[MAP_HEIGHT][MAP_WIDTH], char temp_map[MAP_HEIGHT][MAP_WIDTH]) {
   
   int pathCount = PA2_MAX_PATH;
   int upCount = 0;
   int rightCount = 0;
   int downCount = 0;
   int leftCount = 0;

   // TODO : add in copy temp_map to count cases with chargers
   copyMap(temp_map, map);
   if ((target_x == robot_x) && (target_y == robot_y)) {
      return 0;
   }
   else if (robot_energy < 0) {
      return PA2_MAX_PATH;
   }
   else {
      temp_map[robot_y][robot_x] = VISITED;

      if (isCharger(map, robot_x, robot_y)) {
         robot_energy = robot_full_energy;
      }
      
      if (canVisit(temp_map, robot_x, robot_y-1)) {
         upCount = findShortestDistance(robot_x, robot_y-1, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map) + 1;
      }
      if (canVisit(temp_map, robot_x+1, robot_y)) {
         rightCount = findShortestDistance(robot_x+1, robot_y, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map) + 1;
      }
      if (canVisit(temp_map, robot_x, robot_y+1)) {
         downCount = findShortestDistance(robot_x, robot_y+1, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map) + 1;
      }
      if (canVisit(temp_map, robot_x-1, robot_y)) {
         leftCount = findShortestDistance(robot_x-1, robot_y, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map) + 1;
      }
      pathCount = min(min(upCount, rightCount), min(downCount, leftCount));
   }
   cout << pathCount << endl;
   return pathCount;
};

// * Dependency: Task 2

int findPathSequence(int robot_x, int robot_y, int target_x, int target_y, int robot_energy, 
                     int robot_full_energy, char result_sequence[], const char map[MAP_HEIGHT][MAP_WIDTH], char temp_map[MAP_HEIGHT][MAP_WIDTH]) {
   int delta_x, delta_y;
   int charger_x, charger_y;
   int charger_route = 0;

   return 0;
};

// * Dependency: findShortestDistance
int findFarthestPossibleCharger(int robot_x, int robot_y, int robot_original_x, int robot_original_y, 
                                int &target_x, int &target_y, int robot_energy, int robot_full_energy,
                                const char map[MAP_HEIGHT][MAP_WIDTH], char temp_map[MAP_HEIGHT][MAP_WIDTH]) {
   
   int chargers[MAP_HEIGHT*MAP_WIDTH][2];
   int charger_count = 0;
   int charger_x = 0;
   int charger_y = 0;

   findReachableChargers(map, chargers, robot_x, robot_y, robot_energy, charger_count);
   if (charger_count <= 0) {
      return -1;
   } else {
      return findShortestDistance(robot_x, robot_y, chargers[charger_count-1][0], chargers[charger_count-1][1], robot_energy, robot_full_energy, map, temp_map);
   }
}
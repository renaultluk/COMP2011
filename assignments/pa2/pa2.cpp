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

bool isRobot(const char map[MAP_HEIGHT][MAP_WIDTH], int x, int y) {
   return outOfBounds(x, y) ? false : map[y][x] == ROBOT;
}

bool hasVisited(const char map[MAP_HEIGHT][MAP_WIDTH], int x, int y) {
   return outOfBounds(x, y) ? false : map[y][x] == VISITED;
}

bool canVisit(const char map[MAP_HEIGHT][MAP_WIDTH], int x, int y) {
   return outOfBounds(x, y) ? false : isAvailable(map, x, y) || isCharger(map, x, y) || isRobot(map, x, y);
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
   
   if (!outOfBounds(robot_x, robot_y)) {

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

void initHeatMap(int heat_map[MAP_HEIGHT][MAP_WIDTH], int i) {
   if (i >= PA2_MAX_PATH) {
      return;
   } else {
      heat_map[i/MAP_WIDTH][i%MAP_WIDTH] = 0;
      initHeatMap(heat_map, i+1);
   }
}

void shift(int queue[PA2_MAX_PATH][3], int size, int i) {
   if (i >= size-1) {
      return;
   } else {
      queue[i][0] = queue[i+1][0];
      queue[i][1] = queue[i+1][1];
      queue[i][2] = queue[i+1][2];
      shift(queue, size, i+1);
   }
}

void pop(int queue[PA2_MAX_PATH][3], int &size) {
   if (size < 1) {
      return;
   } else {
      shift(queue, size, 0);
      size--;
   }
}

void floodFill(char map[MAP_HEIGHT][MAP_WIDTH], int heat_map[MAP_HEIGHT][MAP_WIDTH], int queue[PA2_MAX_PATH][3], int queue_size, int x, int y, int target_x, int target_y, int energy, int full_energy, int i) {
   // cout <<"entered floodFill for (" << x << ", " << y << "), i = " << i << endl;

   if (queue_size > 0) {
      pop(queue, queue_size);
      if (!outOfBounds(x, y)) {
         if (isTarget(x, y, target_x, target_y)) {
            map[y][x] = VISITED;
            heat_map[y][x] = i;
            // cout << "arrived at (" << x << ", " << y << "), i = " << i << endl;
            // cout << "branching off to: " << endl;
            // cout << "U: (" << x << ", " << y-1 << ")" << endl;
            // cout << "R: (" << x+1 << ", " << y << ")" << endl;
            // cout << "D: (" << x << ", " << y+1 << ")" << endl;
            // cout << "L: (" << x-1 << ", " << y << ")" << endl;
            return;
         }

         if (isCharger(map, x, y)) {
            energy = full_energy;
            // cout << "charged at (" << x << ", " << y << ")" << endl;
            map[y][x] = VISITED;
            heat_map[y][x] = i;
            // cout << "branching off to: " << endl;
            // cout << "U: (" << x << ", " << y-1 << ")" << endl;
            // cout << "R: (" << x+1 << ", " << y << ")" << endl;
            // cout << "D: (" << x << ", " << y+1 << ")" << endl;
            // cout << "L: (" << x-1 << ", " << y << ")" << endl;
         }

         if (isAvailable(map, x, y) || isRobot(map, x, y)) {
            map[y][x] = VISITED;
            heat_map[y][x] = i;
            // cout << "branching off to: " << endl;
            // cout << "U: (" << x << ", " << y-1 << ")" << endl;
            // cout << "R: (" << x+1 << ", " << y << ")" << endl;
            // cout << "D: (" << x << ", " << y+1 << ")" << endl;
            // cout << "L: (" << x-1 << ", " << y << ")" << endl;
         }

         if (energy >= 0 && !isBlocked(map, x, y)) {
            if (canVisit(map, x, y-1)) {
               queue[queue_size][0] = x;
               queue[queue_size][1] = y-1;
               queue[queue_size][2] = i+1;
               queue_size++;
            }
            if (canVisit(map, x+1, y)) {
               queue[queue_size][0] = x+1;
               queue[queue_size][1] = y;
               queue[queue_size][2] = i+1;
               queue_size++;
            }
            if (canVisit(map, x, y+1)) {
               queue[queue_size][0] = x;
               queue[queue_size][1] = y+1;
               queue[queue_size][2] = i+1;
               queue_size++;
            }
            if (canVisit(map, x-1, y)) {
               queue[queue_size][0] = x-1;
               queue[queue_size][1] = y;
               queue[queue_size][2] = i+1;
               queue_size++;
            }

            if (queue[0][2] != i) {
               energy--;
            }

            floodFill(map, heat_map, queue, queue_size, queue[0][0], queue[0][1], target_x, target_y, energy, full_energy, queue[0][2]);

            // floodFill(map, heat_map, queue, queue_size, x, y-1, target_x, target_y, energy-1, full_energy, i);
            // floodFill(map, heat_map, queue, queue_size, x+1, y, target_x, target_y, energy-1, full_energy, i);
            // floodFill(map, heat_map, queue, queue_size, x, y+1, target_x, target_y, energy-1, full_energy, i);
            // floodFill(map, heat_map, queue, queue_size, x-1, y, target_x, target_y, energy-1, full_energy, i);
         }
      }
   }
};

int findShortestDistance(int robot_x, int robot_y, int target_x, int target_y, int robot_energy, 
                         int robot_full_energy, const char map[MAP_HEIGHT][MAP_WIDTH], char temp_map[MAP_HEIGHT][MAP_WIDTH]) {
   int heat_map[MAP_HEIGHT][MAP_WIDTH];
   int queue[PA2_MAX_PATH][3];
   int queue_size = 1;

   initHeatMap(heat_map, 0);
   queue[0][0] = robot_x;
   queue[0][1] = robot_y;
   queue[0][2] = 1;
   copyMap(temp_map, map);
   floodFill(temp_map, heat_map, queue, queue_size, robot_x, robot_y, target_x, target_y, robot_energy, robot_full_energy, 1);
   // printMap(temp_map);
   // cout << "heat map index at target: " << heat_map[target_y][target_x] << endl;
   if (heat_map[target_y][target_x] == 0) {
      return PA2_MAX_PATH;
   } else {
      return heat_map[target_y][target_x];
   }
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

void linkPath(int heat_map[MAP_HEIGHT][MAP_WIDTH], char path[PA2_MAX_PATH], int x, int y, int pathCount){
   if(heat_map[y][x] == 1 || pathCount < 0){
      
      return;
   }
   else{
      if(heat_map[y][x+1] == heat_map[y][x]-1){
         path[pathCount] = 'L';
         pathCount--;
         linkPath(heat_map, path, x+1, y, pathCount);
      }
      else if(heat_map[y-1][x] == heat_map[y][x]-1){
         path[pathCount] = 'D';
         pathCount--;
         linkPath(heat_map, path, x, y-1, pathCount);
      }
      else if(heat_map[y][x-1] == heat_map[y][x]-1){
         path[pathCount] = 'R';
         pathCount--;
         linkPath(heat_map, path, x-1, y, pathCount);
      }
      else if(heat_map[y+1][x] == heat_map[y][x]-1){
         path[pathCount] = 'U';
         pathCount--;
         linkPath(heat_map, path, x, y+1, pathCount);
      }
   }
}

int findPathSequence(int robot_x, int robot_y, int target_x, int target_y, int robot_energy, 
                     int robot_full_energy, char result_sequence[], const char map[MAP_HEIGHT][MAP_WIDTH], char temp_map[MAP_HEIGHT][MAP_WIDTH]) {
   int heat_map[MAP_HEIGHT][MAP_WIDTH];
   int queue[PA2_MAX_PATH][3];
   int queue_size = 1;
   int pathCount = 0;

   char path[PA2_MAX_PATH];

   initHeatMap(heat_map, 0);
   queue[0][0] = robot_x;
   queue[0][1] = robot_y;
   queue[0][2] = 1;
   copyMap(temp_map, map);
   floodFill(temp_map, heat_map, queue, queue_size, robot_x, robot_y, target_x, target_y, robot_energy, robot_full_energy, 1);
   // printMap(temp_map);
   if (heat_map[target_y][target_x] == 0) {
      return PA2_MAX_PATH;
   } else {
      pathCount = heat_map[target_y][target_x];
      result_sequence[pathCount] = '\0';
      result_sequence[pathCount-1] = 'T';
      linkPath(heat_map,   result_sequence, target_x, target_y, pathCount-2);
      return heat_map[target_y][target_x];
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
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

int min(int w, int x, int y, int z)
{
	if (x < y && x < w && x < z) {
		return x;
	}
	else if (y < w && y < x && y < z) {
		return y;
	}
	else if (w < x && w < y && w < z) {
		return w;
	}
	else {
		return z;
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

// void findChargers(const char map[MAP_HEIGHT][MAP_WIDTH], char temp_map[MAP_HEIGHT][MAP_WIDTH], int chargers[PA2_MAX_PATH][3], int t,
//                         int robot_x, int robot_y, int robot_energy, int robot_full_energy, int &charger_count) {
//    cout << "Entered findChargers for t=" << t << endl;
//    if (t >= PA2_MAX_PATH) {
//       return;
//    } else {
//       if (map[t/MAP_WIDTH][t%MAP_WIDTH] == CHARGER) {
//          int charger_x = t/MAP_WIDTH;
//          int charger_y = t%MAP_WIDTH;
//          int new_data[3];
//          new_data[0] = charger_y;
//          new_data[1] = charger_x;
//          new_data[2] = findShortestDistance(robot_x, robot_y, charger_x, charger_y, robot_energy, robot_full_energy, map, temp_map);
//          cout << "Found charger at (" << charger_x << "," << charger_y << ")" << endl;

//          // insert(chargers, new_data, charger_count-1, false, charger_count);
//       }
//       findChargers(map, temp_map, chargers, t+1, robot_x, robot_y, robot_energy, robot_full_energy, charger_count);
//    }
// };

// void findClosestCharger(const char map[MAP_HEIGHT][MAP_WIDTH], int x, int y, int energy, int full_energy, 
//                   int &charger_x, int &charger_y) {
//    if (isCharger(map, x, y)) {
//       charger_x = x;
//       charger_y = y;
//       return;
//    } else if (energy < 0) {
//       return;
//    } else {
//       checkCell(map, x, y, full_energy, energy);

//       findClosestCharger(map, x, y-1, energy-1, full_energy, charger_x, charger_y);
//       findClosestCharger(map, x+1, y, energy-1, full_energy, charger_x, charger_y);
//       findClosestCharger(map, x, y+1, energy-1, full_energy, charger_x, charger_y);
//       findClosestCharger(map, x-1, y, energy-1, full_energy, charger_x, charger_y);
//    }
// };

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
      if (!isBlocked) {
         findReachableChargers(map, chargers, x, y-1, energy, charger_count);
         findReachableChargers(map, chargers, x+1, y, energy, charger_count);
         findReachableChargers(map, chargers, x, y+1, energy, charger_count);
         findReachableChargers(map, chargers, x-1, y, energy, charger_count);
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

int findShortestDistance(int robot_x, int robot_y, int target_x, int target_y, int robot_energy, 
                         int robot_full_energy, const char map[MAP_HEIGHT][MAP_WIDTH], char temp_map[MAP_HEIGHT][MAP_WIDTH]) {
   
   int delta_x, delta_y;
   int charger_x = -1;
   int charger_y = -1;
   int charger_route = 0;

   int chargers[PA2_MAX_PATH][2];
   int charger_count = 0;

   int upCount = 0;
   int rightCount = 0;
   int downCount = 0;
   int leftCount = 0;
   int increment;

   // TODO : add in copy temp_map to count cases with chargers
   
   // cout << "Current position: (" << robot_x << "," << robot_y << "), \t energy:" << robot_energy << endl;
   if ((robot_x == target_x) && (robot_y == target_y)) {
      return 0;
   }
   // } else if (robot_energy < 0) {
   //    return PA2_MAX_PATH;
   // } else {
   //    if (map[robot_y][robot_x] == CHARGER) {
   //       robot_energy = robot_full_energy;
   //       //cout << "Charged at (" << robot_x << "," << robot_y << ")" << endl;
   //    }
   //    temp_map[robot_y][robot_x] = VISITED;

   //    // delta_x = target_x - robot_x;
   //    // delta_y = target_y - robot_y;
      
   //    // if (delta_x + delta_y > robot_energy) {   // if the robot cannot reach the target directly
         
   //    //    // TODO : Find closest charger, return MAX_PATH if not found
   //    //    // findChargers(map, temp_map, chargers, 0, robot_x, robot_y, robot_energy, robot_full_energy, charger_count);

   //    //    //cout << "Entered charger checking" << endl;
   //    //    findReachableChargers(map, chargers, robot_x, robot_y, robot_energy, charger_count);
   //    //    //cout << "Exited findReachableChargers" << endl;
   //    //    if (charger_count == 0) {
   //    //       return PA2_MAX_PATH;
   //    //    }
         
   //    //    charger_x = chargers[0][0];
   //    //    charger_y = chargers[0][1];

   //    //    charger_route = findShortestDistance(robot_x, robot_y, charger_x, charger_y, robot_energy, robot_full_energy, map, temp_map);
   //    //    robot_x = charger_x;
   //    //    robot_y = charger_y;
   //    //    delta_x = target_x - robot_x;
   //    //    delta_y = target_y - robot_y;
   //    // }
      
   //    // first pass (most prioritized) for if the movement in the delta direction is available
   //    if (!isBlocked(map, robot_x, robot_y)) {
   //       upCount = findShortestDistance(robot_x, robot_y-1, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map) + 1;
   //       rightCount = findShortestDistance(robot_x+1, robot_y, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map) + 1;
   //       downCount = findShortestDistance(robot_x, robot_y+1, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map) + 1;
   //       leftCount = findShortestDistance(robot_x-1, robot_y, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map) + 1;
   //    }
   //    return min(upCount, rightCount, downCount, leftCount);

   // }
   else if (robot_energy < 0) {
      return 0;
   }
   else if (!isBlocked(map, robot_x, robot_y)) {
      if (isCharger(map, robot_x, robot_y)) {
         robot_energy = robot_full_energy;
      }
      // if (!hasVisited(map, robot_x, robot_y)) {
      //    map[robot_y][robot_x] = VISITED;
      // }

      upCount = findShortestDistance(robot_x, robot_y-1, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map);
      rightCount = findShortestDistance(robot_x+1, robot_y, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map);
      downCount = findShortestDistance(robot_x, robot_y+1, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map);
      rightCount = findShortestDistance(robot_x-1, robot_y, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map);

      return min(upCount, rightCount, downCount, leftCount) + 1;
   } else {
      return findShortestDistance(robot_x, robot_y, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map);
   }
};

// * Dependency: Task 2

int findPathSequence(int robot_x, int robot_y, int target_x, int target_y, int robot_energy, 
                     int robot_full_energy, char result_sequence[], const char map[MAP_HEIGHT][MAP_WIDTH], char temp_map[MAP_HEIGHT][MAP_WIDTH]) {
   int delta_x, delta_y;
   int charger_x, charger_y;
   int charger_route = 0;

   return 0;
};

// * Dependency: findChargers
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

   }


   return 0;
}
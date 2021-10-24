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

void findChargers(const char map[MAP_HEIGHT][MAP_WIDTH], int chargers[][3], int t,
                        int robot_x, int robot_y, int &charger_count) {
   if (t >= MAX_PATH) {
      return;
   } else {
      if (map[t/MAP_WIDTH][t%MAP_WIDTH] == CHARGER) {
         int charger_x = t/MAP_WIDTH;
         int charger_y = t%MAP_WIDTH;
         int new_data[3];
         new_data[0] = charger_y;
         new_data[1] = charger_x;
         new_data[2] = (abs(charger_x-robot_x) + abs(charger_y-robot_y));

         insert(chargers, new_data, charger_count-1, false, charger_count);
      }
      findChargers(map, chargers, t+1, robot_x, robot_y, charger_count);
   }
};

int findMaximumPlace(int robot_x, int robot_y, int robot_energy, int robot_full_energy, 
                     char result_map[MAP_HEIGHT][MAP_WIDTH], char temp_map[MAP_HEIGHT][MAP_WIDTH]) {

    if (robot_energy <= 0) {
        return 0;
    } else {
        result_map[robot_y][robot_x] = VISITED;
        if (temp_map[robot_y][robot_x] == CHARGER) {
            robot_energy = robot_full_energy;
        }

        // first pass (most prioritized) for chargers
        if ((robot_y-1 >= 0) && (result_map[robot_y-1][robot_x] == CHARGER)) {
            return findMaximumPlace(robot_x, robot_y-1, robot_energy-1, robot_full_energy, result_map, temp_map) + 1;
        } else if ((robot_x+1 < MAP_WIDTH) && (result_map[robot_y][robot_x+1] == CHARGER)) {
            return findMaximumPlace(robot_x+1, robot_y, robot_energy-1, robot_full_energy, result_map, temp_map) + 1;
        } else if ((robot_y+1 < MAP_HEIGHT) && (result_map[robot_y+1][robot_x] == CHARGER)) {
            return findMaximumPlace(robot_x, robot_y+1, robot_energy-1, robot_full_energy, result_map, temp_map) + 1;
        } else if ((robot_x-1 >= 0) && (result_map[robot_y][robot_x-1] == CHARGER)) {
            return findMaximumPlace(robot_x-1, robot_y, robot_energy-1, robot_full_energy, result_map, temp_map) + 1;
        } 
        
        // second pass (less prioritized) for blank spaces
        else if ((robot_y-1 >= 0) && (result_map[robot_y-1][robot_x] == AVAILABLE)) {
            return findMaximumPlace(robot_x, robot_y-1, robot_energy-1, robot_full_energy, result_map, temp_map) + 1;
        } else if ((robot_x+1 < MAP_WIDTH) && (result_map[robot_y][robot_x+1] == AVAILABLE)) {
            return findMaximumPlace(robot_x+1, robot_y, robot_energy-1, robot_full_energy, result_map, temp_map) + 1;
        } else if ((robot_y+1 < MAP_HEIGHT) && (result_map[robot_y+1][robot_x] == AVAILABLE)) {
            return findMaximumPlace(robot_x, robot_y+1, robot_energy-1, robot_full_energy, result_map, temp_map) + 1;
        } else if ((robot_x-1 >= 0) && (result_map[robot_y][robot_x-1] == AVAILABLE)) {
            return findMaximumPlace(robot_x-1, robot_y, robot_energy-1, robot_full_energy, result_map, temp_map) + 1;
        } 
        
        // third pass (least prioritized) for visited squares without incrementing the number of spaces visited
        else if ((robot_y-1 >= 0) && (result_map[robot_y-1][robot_x] == VISITED)) {
            return findMaximumPlace(robot_x, robot_y-1, robot_energy-1, robot_full_energy, result_map, temp_map);
        } else if ((robot_x+1 < MAP_WIDTH) && (result_map[robot_y][robot_x+1] == VISITED)) {
            return findMaximumPlace(robot_x+1, robot_y, robot_energy-1, robot_full_energy, result_map, temp_map);
        } else if ((robot_y+1 < MAP_HEIGHT) && (result_map[robot_y+1][robot_x] == VISITED)) {
            return findMaximumPlace(robot_x, robot_y+1, robot_energy-1, robot_full_energy, result_map, temp_map);
        } else if ((robot_x-1 >= 0) && (result_map[robot_y][robot_x-1] == VISITED)) {
            return findMaximumPlace(robot_x-1, robot_y, robot_energy-1, robot_full_energy, result_map, temp_map);
        }
    }
};

int findShortestDistance(int robot_x, int robot_y, int target_x, int target_y, int robot_energy, 
                         int robot_full_energy, const char map[MAP_HEIGHT][MAP_WIDTH], char temp_map[MAP_HEIGHT][MAP_WIDTH]) {
   
   int delta_x, delta_y;
   int charger_x, charger_y;
   int charger_route = 0;

   int chargers[MAX_PATH][3];
   int charger_count = 0;

   if ((robot_x == target_x) && (robot_y == target_y)) {
      return 0;
   } else if (robot_energy <= 0) {
      return MAX_PATH;
   } else {
      if (map[robot_y][robot_x] == CHARGER) {
         robot_energy = robot_full_energy;
      }
      temp_map[robot_y][robot_x] = VISITED;

      delta_x = target_x - robot_x;
      delta_y = target_y - robot_y;
      
      if (delta_x + delta_y > robot_energy) {   // if the robot cannot reach the target directly
         
         // TODO : Find closest charger, return MAX_PATH if not found
         findChargers(map, chargers, 0, robot_x, robot_y, charger_count);
         if (charger_count == 0) {
            return MAX_PATH;
         }
         
         charger_x = chargers[0][1];
         charger_y = chargers[0][0];
         charger_route = findShortestDistance(robot_x, robot_y, charger_x, charger_y, robot_energy, robot_full_energy, map, temp_map);
         robot_x = charger_x;
         robot_y = charger_y;
         delta_x = target_x - robot_x;
         delta_y = target_y - robot_y;
      }
      
      // first pass (most prioritized) for if the movement in the delta direction is available
      if ((delta_y < 0) && (robot_y-1 >= 0) && ((temp_map[robot_y-1][robot_x] == AVAILABLE) || (temp_map[robot_y-1][robot_x] == CHARGER))) {
         return findShortestDistance(robot_x, robot_y-1, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map) + 1 + charger_route;
      } else if ((delta_x > 0) && (robot_x+1 < MAP_WIDTH) && ((temp_map[robot_y][robot_x+1] == AVAILABLE) || (temp_map[robot_y][robot_x+1] == CHARGER))) {
         return findShortestDistance(robot_x+1, robot_y, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map) + 1 + charger_route;
      } else if ((delta_y > 0) && (robot_y+1 < MAP_HEIGHT) && ((temp_map[robot_y+1][robot_x] == AVAILABLE) || (temp_map[robot_y+1][robot_x] == CHARGER))) {
         return findShortestDistance(robot_x, robot_y+1, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map) + 1 + charger_route;
      } else if ((delta_x < 0) && (robot_x-1 >= 0) && ((temp_map[robot_y][robot_x-1] == AVAILABLE) || (temp_map[robot_y][robot_x-1] == CHARGER))) {
         return findShortestDistance(robot_x-1, robot_y, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map) + 1 + charger_route;
      }

      // second pass (less prioritized) for detours if the delta direction is not directly available
      else if ((robot_y-1 >= 0) && ((temp_map[robot_y-1][robot_x] == AVAILABLE) || (temp_map[robot_y-1][robot_x] == CHARGER))) {
         return findShortestDistance(robot_x, robot_y-1, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map) + 1 + charger_route;
      } else if ((robot_x+1 < MAP_WIDTH) && ((temp_map[robot_y][robot_x+1] == AVAILABLE) || (temp_map[robot_y][robot_x+1] == CHARGER))) {
         return findShortestDistance(robot_x+1, robot_y, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map) + 1 + charger_route;
      } else if ((robot_y+1 < MAP_HEIGHT) && ((temp_map[robot_y+1][robot_x] == AVAILABLE) || (temp_map[robot_y+1][robot_x] == CHARGER))) {
         return findShortestDistance(robot_x, robot_y+1, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map) + 1 + charger_route;
      } else if ((robot_x-1 >= 0) && ((temp_map[robot_y][robot_x-1] == AVAILABLE) || (temp_map[robot_y][robot_x-1] == CHARGER))) {
         return findShortestDistance(robot_x-1, robot_y, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map) + 1 + charger_route;
      }

      // third pass (least prioritized) for backtracks if the robot is stuck in dead ends
      else if ((robot_y-1 >= 0) && (temp_map[robot_y-1][robot_x] == VISITED)) {
         return findShortestDistance(robot_x, robot_y-1, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map) + 1 + charger_route;
      } else if ((robot_x+1 < MAP_WIDTH) && (temp_map[robot_y][robot_x+1] == VISITED)) {
         return findShortestDistance(robot_x+1, robot_y, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map) + 1 + charger_route;
      } else if ((robot_y+1 < MAP_HEIGHT) && (temp_map[robot_y+1][robot_x] == VISITED)) {
         return findShortestDistance(robot_x, robot_y+1, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map) + 1 + charger_route;
      } else if ((robot_x-1 >= 0) && (temp_map[robot_y][robot_x-1] == VISITED)) {
         return findShortestDistance(robot_x-1, robot_y, target_x, target_y, robot_energy-1, robot_full_energy, map, temp_map) + 1 + charger_route;
      }

      // TODO : more cases
      else {
         return MAX_PATH;
      }

   }
};

// * Dependency: Task 2

int findPathSequence(int robot_x, int robot_y, int target_x, int target_y, int robot_energy, 
                     int robot_full_energy, char result_sequence[], const char map[MAP_HEIGHT][MAP_WIDTH], char temp_map[MAP_HEIGHT][MAP_WIDTH]);

// * Dependency: findChargers
int findFarthestPossibleCharger(int robot_x, int robot_y, int robot_original_x, int robot_original_y, 
                                int &target_x, int &target_y, int robot_energy, int robot_full_energy,
                                const char map[MAP_HEIGHT][MAP_WIDTH], char temp_map[MAP_HEIGHT][MAP_WIDTH]);
   
   // int chargers[MAP_HEIGHT*MAP_WIDTH][3];
   // int charger_count = 0;
   // findChargers(map, chargers, 0, robot_x, robot_y, charger_count);
   // if ((charger_count == 0) || (findShortestDistance(robot_original_x, robot_original_y,))) {

   // }
/**
 @author    Marco Livesu (marco.livesu@gmail.com)
 @copyright Marco Livesu 2014.
*/

#ifndef OLDLOAD_SAVE_TRIMESH_H
#define OLDLOAD_SAVE_TRIMESH_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>


// OBJ FILES
//
void loadObj(const char          * filename,
              std::vector<double> & xyz,
              std::vector<int>    & tri);

void saveObj(const char          * filename,
             const std::vector<double> & xyz,
             const std::vector<int>    & tri);

void objToOff(const char* f1, const char* f2);

void offToObj(const char* f1, const char* f2);

#endif // OLDLOAD_SAVE_TRIMESH_H

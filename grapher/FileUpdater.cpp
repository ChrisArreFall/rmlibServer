//
// Created by Chris on 10/1/17.
//

#include "FileUpdater.h"
#include "FileUpdater.h"
#include <zconf.h>
#include "FileUpdater.h"

void FileUpdater::updater(double x,double y,const char * FILEP) {
    FILE * fileF = fopen(FILEP, "a+");
    sleep(1);
    fprintf(fileF, "%lf %lf \n", x, y);
    fclose(fileF);
}
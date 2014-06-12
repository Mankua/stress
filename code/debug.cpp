#include "max.h"

#define FNAME "c:\\debug\\stress.txt"

void debug(){
	FILE *file = fopen(FNAME,"w");
	fclose(file);
}

void debug(TSTR str){
	FILE *file = fopen(FNAME,"a+");
	fprintf(file, "%s",str);
	fclose(file);
}

void debug(int kprint){
	FILE *file = fopen(FNAME,"a+");
	fprintf(file, "%d\n",kprint);
	fclose(file);
} 

void debug(unsigned int kprint){
	FILE *file = fopen(FNAME,"a+");
	fprintf(file, "%u\n",kprint);
	fclose(file);
} 

void debug(float kprint){
	FILE *file = fopen(FNAME,"a+");
	fprintf(file, "%f\n",kprint);
	fclose(file);
} 

void debug(Point3 kprint){
	FILE *file = fopen(FNAME,"a+");
	fprintf(file, "%f\t%f\t%f\n",kprint.x,kprint.y,kprint.z);
	fclose(file);
} 

void debug(IPoint3 kprint){
	FILE *file = fopen(FNAME,"a+");
	fprintf(file, "%d\t%d\t%d\n",kprint.x,kprint.y,kprint.z);
	fclose(file);
} 

void debug(Point2 kprint){
	FILE *file = fopen(FNAME,"a+");
	fprintf(file, "%f\t%f\n",kprint.x,kprint.y);
	fclose(file);
} 

void debug(IPoint2 kprint){
	FILE *file = fopen(FNAME,"a+");
	fprintf(file, "%d\t%d\n",kprint.x,kprint.y);
	fclose(file);
} 

void debug(Matrix3 kprint){
	FILE *file = fopen(FNAME,"a+");
	Point3 aux;
	aux = kprint.GetRow(0);
	fprintf(file, "%f\t%f\t%f\n",aux.x,aux.y,aux.z);
	aux = kprint.GetRow(1);
	fprintf(file, "%f\t%f\t%f\n",aux.x,aux.y,aux.z);
	aux = kprint.GetRow(2);
	fprintf(file, "%f\t%f\t%f\n",aux.x,aux.y,aux.z);
	aux = kprint.GetRow(3);
	fprintf(file, "%f\t%f\t%f\n",aux.x,aux.y,aux.z);
	fclose(file);
} 


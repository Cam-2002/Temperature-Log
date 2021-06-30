#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void readWord(char **str, FILE *file){
	int ch, count, count2;
	ch = fgetc(file);
	count = 0;
	while(ch != EOF && ch != 0x0A && ch != 0x20){
		count++;
		ch = fgetc(file);
	}
	fseek(file, -count-1, SEEK_CUR);
	*str = malloc(count);
	count2 = 0;
	while(count2++ < count){
		ch = fgetc(file);
		(*str)[count2-1] = ch;
	}
}


int main(int argc, char *argv[]){
	int opt, delay, zone;
	char *outpath, *buf;
	outpath = "";
	zone = 0;
	delay = 15;
	while((opt = getopt(argc,argv,"o:t:z:")) != -1){
		switch(opt){
			case 'o':
				outpath = malloc(strlen(optarg));
				memcpy(outpath,optarg,strlen(optarg));
				break;
			case 't':
				delay = atoi(optarg);
				break;
			case 'z':
				zone = atoi(optarg);
				break;
			default:
				printf("Usage: %s [-o outputfile] [-t timeinterval] [-z thermalzone]",argv[0]);
				return 0;
				break;
		}
	}
	FILE *tempfile;
	FILE *typefile;
	FILE *outfile;
	buf = malloc(255);
	sprintf(buf,"/sys/class/thermal/thermal_zone%d/temp",zone);
	tempfile = fopen(buf,"r");
	if(tempfile == NULL){
		perror("Error");
		return 1;
	}
	sprintf(buf,"/sys/class/thermal/thermal_zone%d/type",zone);
	typefile = fopen(buf,"r");
	if(typefile == NULL){
		perror("Error");
		return 1;
	}
	int ch, count;
	char *type, *tempstr;
	float temp;
	readWord(&type, typefile);
	if(strlen(outpath)<1){
		outpath = "%d_%s_out.csv";
		sprintf(buf,outpath,zone,type);
		outpath = buf;
	}
	outfile = fopen(outpath,"w");
	if(outfile == NULL){
		perror("Error");
		return 1;
	}
	printf("Starting Temperature Log.\nOut: \"%s\"\nThermal Zone %d: %s.\n%ds Interval.\n\n",outpath,zone,type,delay);
	fprintf(outfile, "time, %s\n", type);
	count = 0;
	delay *= 10;
	while(1){
		fflush(tempfile);
		rewind(tempfile);
		readWord(&tempstr, tempfile);
		fflush(stdout);
		temp = atoi(tempstr)/1000.0;
		printf("[%ds] %s: %.3f\u00B0C     \r",(int)(count/10),type,temp);
		usleep(100000);
		if(count%delay==0){
			fprintf(outfile, "%d, %.3f\n", (int)(count/10), temp);
			fflush(outfile);
		}
		count++;
	}
}


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <time.h>
#include <unistd.h>

int32_t time_interval = 1;
double calculated_weight = 0.0;
double simple_weight = 0.0;
const double scale = 94.31;
const double kalman = 0.1;
const double inv_kal = (1.0 - kalman);

struct weight_values{
	int32_t x0, x1, y0, y1;
} weight, measured_weight;

void calculateWeight(){
	calculated_weight = (weight.x0 + weight.x1 + weight.y0 + weight.y1) / scale;
	simple_weight = (measured_weight.x0 + measured_weight.x1 + measured_weight.y0 + measured_weight.y1) / scale;
}

void printWeight(){
	printf("\e[1;1H\e[2J");
	printf("Weight: %f kg\nSimple: %f kg\n", calculated_weight, simple_weight);
}

void setWeight(u_int16_t co, int32_t val){
	switch(co){
		case ABS_HAT0X:
		measured_weight.x0 = val;
		weight.x0 = kalman * val + (inv_kal) * weight.x0;
		break;
		case ABS_HAT1X:
		measured_weight.x1 = val;
		weight.x1 = kalman * val + (inv_kal) * weight.x1;
		break;
		case ABS_HAT0Y:
		measured_weight.y0 = val;
		weight.y0 = kalman * val + (inv_kal) * weight.y0;
		break;
		case ABS_HAT1Y:
		measured_weight.y1 = val;
		weight.y1 = kalman * val + (inv_kal) * weight.y1;
		break;
	}
}

int main(int argc, char** argv){
	char* input_device=NULL;
	int fd, bytes;
	struct input_event ev;
	time_t current_time, last_time;
	time(&last_time);

	if(argc<2){
		printf("Insufficient arguments\n");
		exit(1);
	}
	
	input_device=argv[1];

	fd=open(input_device, O_RDONLY);

	if(fd<0){
		printf("Failed to open device\n");
		exit(1);
	}

	//get exclusive control
	bytes=ioctl(fd, EVIOCGRAB, 1);
	
	while(true){
		bytes=read(fd, &ev, sizeof(struct input_event));
		if(bytes<0){
			printf("read() error\n");
			close(fd);
			exit(1);
		}
		if(ev.type==EV_ABS){
			//printf("Type: %d; Scancode %d; Value: %i \n", ev.type, ev.code, ev.value);
			setWeight(ev.code, ev.value);
		}
		time(&current_time);
		if(current_time - last_time > time_interval){
			last_time = current_time;
			printWeight();
		}
		calculateWeight();
	}

	return 0;
}

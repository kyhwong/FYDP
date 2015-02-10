#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "emokit/emokit.h"

#define HEADSET_FREQUENCY 128

void decrypt_loop(emokit_device *eeg, int secs, const char* output) {
	int i;
    FILE* fp;
    int frames_needed = 0;
    int count = 0;
    frames_needed = HEADSET_FREQUENCY*secs;
    fp = fopen(output, "w");
    if (!fp) {
        printf("Failed to open '%s' for writing\n", output);
        return;
    }
    fprintf(fp, "gx,gy,F3,FC,P7,T8,F7,F8,T7,P8,AF4,F4,AF3,O2,O1,FC5\n");
    printf("Starting data read\n");
	while (count < frames_needed) {
		if (emokit_read_data(eeg) > 0) {
            count++;
            struct emokit_frame frame;
            frame = emokit_get_next_frame(eeg);
            fprintf(fp, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                    frame.gyroX, frame.gyroY, 
                    frame.F3, frame.FC6, frame.P7,
                    frame.T8, frame.F7, frame.F8,
                    frame.T7, frame.P8, frame.AF4,
                    frame.F4, frame.AF3, frame.O2,
                    frame.O1, frame.FC5);
		}
	}
    fclose(fp);
}

int main(int argc, char **argv) {
	int i;
	unsigned char dev_type;
	pthread_t tid;
	struct sigaction sa;
	emokit_device *eeg;
    char filename[255];
    time_t date;
    int secs = 0;
    
    date = time(NULL);
    
    // Create the emokit device
	eeg = emokit_create();
    
	if (emokit_open(eeg, EMOKIT_VID, EMOKIT_PID, 1) != 0) {
        printf("Unable to create a device, are you root?\n");
		return 1;
	}
    
    printf("How many seconds to record?\n");
    scanf("%d", &secs);
    
    sprintf(&filename[0], "output/output_%ds_%d.csv", secs, (int)date);
    
    printf("Starting decrypt loop\n");
	decrypt_loop(eeg, secs, filename);
    printf("Finished recording\n");
    
	return 0;
}

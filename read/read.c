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

int decrypt_loop(emokit_device *eeg, int secs, const char* output) {
	int i;
    FILE* fp;
    int frames_needed = 0;
    int count = 0;
	int ret = 0;
    frames_needed = HEADSET_FREQUENCY*secs;
    fp = fopen(output, "w");
    if (!fp) {
        printf("Failed to open '%s' for writing\n", output);
        return;
    }
    fprintf(fp, "F3,FC,P7,T8,F7,F8,T7,P8,AF4,F4,AF3,O2,O1,FC5\n");
	while (count < frames_needed) {
		ret = emokit_read_data_timeout(eeg, 100);
		if (ret > 0) {
            count++;
            struct emokit_frame frame;
            frame = emokit_get_next_frame(eeg);
            fprintf(fp, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                    frame.F3, frame.FC6, frame.P7,
                    frame.T8, frame.F7, frame.F8,
                    frame.T7, frame.P8, frame.AF4,
                    frame.F4, frame.AF3, frame.O2,
                    frame.O1, frame.FC5);
			ret = 0;
		} else {
			ret = -4;
			break;
		}
	}
    fclose(fp);
	return ret;
}

int main(int argc, char **argv) {
	int i;
	unsigned char dev_type;
	pthread_t tid;
	struct sigaction sa;
	emokit_device *eeg;
    char filename[255];
    int secs = 0;
    
    if (argc != 3) return -1;
    
    strncpy(filename, argv[1], 254);
    filename[254] = '\0';
    secs = atoi(argv[2]);
    
    // Create the emokit device
	eeg = emokit_create();
    int ret = emokit_open(eeg, EMOKIT_VID, EMOKIT_PID, 1);
	if (ret != 0) {
        // printf("Unable to create a device, are you root?\n");
		return ret;
	}
    
    
    // printf("How many seconds to record?\n");
    // scanf("%d", &secs);
    
    // sprintf(&filename[0], "output/output_%ds_%d.csv", secs, (int)date);
    
    // printf("Starting decrypt loop\n");
	ret = decrypt_loop(eeg, secs, filename);
    // printf("Finished recording\n");
    
	return ret;
}

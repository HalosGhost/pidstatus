/*
 * Copyright 2016 - Jonathan Roemer
 * Licensed under the GPLv3. See LICENSE
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <X11/Xlib.h>

static
Display *dpy;

float
getbattery(void);

char
*getdatetime(void);

void
setstatus(char *str);

float
getbattery(void) {
	FILE *fd;
	int energy_now, energy_full, voltage_now;

	/* make sure we can read energy_now */
	if ( !(fd = fopen("/sys/class/power_supply/BAT0/energy_now", "r")) ) {
		fputs("Error opening energy_now.\n", stderr);
		return EXIT_FAILURE;
	}
	
	/* if not NULL, read in the value */
	if(fscanf(fd, "%d", &energy_now) != 1) {
		fputs("Error in fscanf.\n", stderr);
		fclose(fd);
	}

	/* make sure we can read energy_full */
	if ( !(fd = fopen("/sys/class/power_supply/BAT0/energy_full", "r")) ) {
		fputs("Error opening energy_full.\n", stderr);
		return EXIT_FAILURE;
	}
	
	/* if not NULL, read in the value */
	if(fscanf(fd, "%d", &energy_full) != 1) {
		fputs("Error in fscanf.\n", stderr);
		fclose(fd);
	}

	/* make sure we can read voltage_now */
	if ( !(fd = fopen("/sys/class/power_supply/BAT0/voltage_now", "r")) ) {
		fputs("Error opening voltage_now.\n", stderr);
		return EXIT_FAILURE;
	}

	/* if not NULL, read in the value */
	if(fscanf(fd, "%d", &voltage_now) != 1) {
		fputs("Error in fscanf.\n", stderr);	
		fclose(fd);
	}

	/* return a useful percentage */
	return ((float)energy_now * 1000 / (float)voltage_now) * 100 /
		((float)energy_full * 1000 / (float)voltage_now);
}

char
*getdatetime() {
	char *buf;
	time_t result;
	struct tm *resulttm;

	if( !(buf = malloc(sizeof(char)*65)) ) {
		fputs("Cannot allocate memory for buf.\n", stderr);
		return 0;
	}
	result = time(NULL);
	resulttm = localtime(&result);
	if(!resulttm) {
		fputs("Error getting localtime.\n", stderr);
		return 0;
	}
	if(!strftime(buf, sizeof(char)*65-1, "%Y-%m-%d %H:%M", resulttm)) {
		fputs("strftime is 0.\n", stderr);
		return 0;
	}

	return buf;
}

void
setstatus(char *str) {
	XStoreName(dpy, DefaultRootWindow(dpy), str);
	XSync(dpy, False);
}


int
main(void) {
	char status [200], *datetime;
	float bat0;
	
	if (!(dpy = XOpenDisplay(NULL))) {
		fputs("Cannot open display.\n", stderr);
		return EXIT_FAILURE;
	}
	
	bat0 = getbattery();
	if (!(datetime = getdatetime())) {
		return EXIT_FAILURE;
	}
	
	snprintf(status, 200, "%0.2f | %s", bat0, datetime);

	free(datetime);
	setstatus(status);
	
	XCloseDisplay(dpy);
	return EXIT_SUCCESS;
}

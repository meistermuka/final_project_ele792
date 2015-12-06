/*
 ============================================================================
 Name        : photoapp.c
 Author      : Mukandila Mukandila
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <gphoto2/gphoto2-camera.h>
#include "helpers.h"


int main(void) {

	int ret;
	// Camera specific variables
	char *mode;

	Camera *camera;
	GPContext *context;

	context = create_context();
	gp_camera_new(&camera);

	ret = gp_camera_init(camera, context);
	printf("=====PhotoApp v1.0=====\n");

	if(ret < GP_OK) {
		printf("No camera detected.\n");
		gp_camera_free(camera);
		return -1;
	}

	ret = get_config_value_string(camera, "d182", &mode, context);
	if(ret >= GP_OK)
		printf("Control mode is: %s\n", mode);


	// Code clean up area
	gp_camera_exit(camera, context);
	gp_camera_free(camera);
	gp_context_unref(context);

	return EXIT_SUCCESS;

}

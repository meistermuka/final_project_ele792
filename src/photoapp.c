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
#include <gphoto2/gphoto2-camera.h>
#include "photoapp.h"


int main(void) {

	int ret;
	// Camera specific variables
	//char *owner;

	Camera *camera;
	GPContext *context;
	char *iso;

	context = create_context();
	gp_camera_new(&camera);

	ret = gp_camera_init(camera, context);
	printf("=====PhotoApp v1.0=====\n");


	if(ret < GP_OK) {
		printf("No camera detected.\n");
		gp_camera_free(camera);
		return -1;
	}

	// Code clean up area
	gp_camera_exit(camera, context);
	gp_camera_free(camera);
	gp_context_unref(context);

	return EXIT_SUCCESS;

}

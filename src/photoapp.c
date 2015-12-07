#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <gphoto2/gphoto2-camera.h>
#include "helpers.h"

#define CLEAR_TERM  printf("\033[2J\033[1;1H")


int main(void) {

	int ret, cmd;
	// Camera specific variables
	char *mode;

	Camera *camera;
	GPContext *context;

	context = create_context();
	gp_camera_new(&camera);

	ret = gp_camera_init(camera, context);
	//printf("=====PhotoApp v1.0=====\n");

	if(ret < GP_OK) {
		printf("No camera detected.\n");
		gp_camera_free(camera);
		return -1;
	}

	while(cmd!=0) {

		cmd = menu();

		switch(cmd) {
			case 1:
				printf("%s\n", get_camerainfo(camera, context));
				break;
		}
	}

	// Code clean up area
	gp_camera_exit(camera, context);
	gp_camera_free(camera);
	gp_context_unref(context);

	return EXIT_SUCCESS;

}

int menu(void) {

	CLEAR_TERM;
	int cmd = 0;

	printf("=====| Photoapp v1.0 |=====\n");
	printf("Choose a command:\n\n");
	printf("1) Camera Info\n");
	printf("0) Exit Program\n");
	printf("\n===========================\n");

	scanf("%d", cmd);
	CLEAR_TERM;
	return cmd;

}



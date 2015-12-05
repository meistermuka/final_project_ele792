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
#include <gphoto2/gphoto2-camera.h>
#include "helpers.h"


int main(void) {

	int ret, i, list_count;
	// Camera specific variables
	//char *owner;

	Camera *camera;
	GPContext *context;
	char *iso;
	char *image_name = "/tmp/test_image.jpg";
	char *name, *value;
	char *folder_one = "/store_00010001/DCIM/107D7000";
	char *folder_two = "/store_00010001/DCIM/108D7000";
	char *folder_three = "/store_00020001/DCIM/107D7000";
	char *folder_four = "/store_00020001/DCIM/108D7000";

	CameraFilePath cfilepath;
	CameraList *list;
	CameraFileInfo info;
	CameraFile *cfile;
	int fd;

	context = create_context();
	gp_camera_new(&camera);

	ret = gp_camera_init(camera, context);
	printf("=====PhotoApp v1.0=====\n");

	if(ret < GP_OK) {
		printf("No camera detected.\n");
		gp_camera_free(camera);
		return -1;
	}

	strcpy(cfilepath.folder, folder_four);
	strcpy(cfilepath.name, "TST_001.jpg");

	printf("Capturing image...\n");
	ret = gp_camera_capture(camera, GP_CAPTURE_IMAGE, &cfilepath, context);
	if(ret >= GP_OK)
		printf("Image capture OK!\n");
	else
		printf("Image capture ERROR!\n");

	fd = open("/tmp/TST_001.jpg", O_CREAT | O_WRONLY, 0664);

	ret = gp_file_new_from_fd(&cfile, fd);
	if(ret >= GP_OK)
		printf("New file descriptor OK!\n");
	else
		printf("New file descriptor ERROR!\n");

	printf("Getting file from camera...\n");
	ret = gp_camera_file_get(camera, cfilepath.folder, cfilepath.name, GP_FILE_TYPE_NORMAL, cfile, context);
	if(ret >= GP_OK)
		printf("File copied from camera OK!\n");
	else
		printf("File copied from camera ERROR!\n");

	gp_file_free(cfile);

	/*ret = gp_list_new(&list);
	if(ret < GP_OK)
		printf("No new list.\n");

	ret = gp_camera_folder_list_folders(camera,"/store_00020001/DCIM",list,context);
	if(ret < GP_OK)
		printf("Unable to list files.\n");

	list_count = gp_list_count(list);
	for(i = 0; i < list_count; i++) {

		gp_list_get_name(list, i, &name);
		gp_list_get_value(list, i, &value);

		printf("Name: %s\n", name);
		//printf("Value: %s\n", value);

	}

	gp_list_free(list);*/

	//capture_to_file(camera, context, image_name);


	// Code clean up area
	gp_camera_exit(camera, context);
	gp_camera_free(camera);
	gp_context_unref(context);

	return EXIT_SUCCESS;

}

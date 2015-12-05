/*
 * helpers.c
 *
 *  Created on: Dec 1, 2015
 *      Author: muka
 */
#include <gphoto2/gphoto2-camera.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include "helpers.h"


static void ctx_error_func (GPContext *context, const char *str, void *data) {
        fprintf  (stderr, "\n*** Contexterror ***              \n%s\n",str);
        fflush   (stderr);
}

static void ctx_status_func (GPContext *context, const char *str, void *data) {
        fprintf  (stderr, "%s\n", str);
        fflush   (stderr);
}

// from context.c
GPContext* create_context() {

	GPContext *context;

	context = gp_context_new();
	gp_context_set_error_func (context, ctx_error_func, NULL);
	gp_context_set_status_func (context, ctx_status_func, NULL);

	return context;
}


static int _lookup_widget(CameraWidget*widget, const char *key, CameraWidget **child) {
	int ret;
	ret = gp_widget_get_child_by_name (widget, key, child);
	if (ret < GP_OK)
		ret = gp_widget_get_child_by_label (widget, key, child);
	return ret;
}


int get_config_value_string (Camera *camera, const char *key, char **str, GPContext *context) {

	CameraWidget *widget = NULL;
	CameraWidget *child = NULL;
	CameraWidgetType type;
	int ret;
	char *val;

	ret = gp_camera_get_config (camera, &widget, context);
	if (ret < GP_OK) {
		fprintf (stderr, "camera_get_config failed: %d\n", ret);
		return ret;
	}
	ret = _lookup_widget (widget, key, &child);
	if (ret < GP_OK) {
		fprintf (stderr, "lookup widget failed: %d\n", ret);
		goto out;
	}

	/* This type check is optional, if you know what type the label
	 * has already. If you are not sure, better check. */
	ret = gp_widget_get_type (child, &type);
	if (ret < GP_OK) {
		fprintf (stderr, "widget get type failed: %d\n", ret);
		goto out;
	}
	switch (type) {
		case GP_WIDGET_MENU:
		case GP_WIDGET_RADIO:
		case GP_WIDGET_TEXT:
		break;
	default:
		fprintf (stderr, "widget has bad type %d\n", type);
		ret = GP_ERROR_BAD_PARAMETERS;
		goto out;
	}

	/* This is the actual query call. Note that we just
	 * a pointer reference to the string, not a copy... */
	ret = gp_widget_get_value (child, &val);
	if (ret < GP_OK) {
		fprintf (stderr, "could not query widget value: %d\n", ret);
		goto out;
	}
	/* Create a new copy for our caller. */
	*str = strdup (val);
out:
	gp_widget_free (widget);
	return ret;

}

int set_config_value_string (Camera *camera, const char *key, const char *val, GPContext *context) {
	CameraWidget		*widget = NULL, *child = NULL;
	CameraWidgetType	type;
	int			ret;

	ret = gp_camera_get_config (camera, &widget, context);
	if (ret < GP_OK) {
		fprintf (stderr, "camera_get_config failed: %d\n", ret);
		return ret;
	}
	ret = _lookup_widget (widget, key, &child);
	if (ret < GP_OK) {
		fprintf (stderr, "lookup widget failed: %d\n", ret);
		goto out;
	}

	/* This type check is optional, if you know what type the label
	 * has already. If you are not sure, better check. */
	ret = gp_widget_get_type (child, &type);
	if (ret < GP_OK) {
		fprintf (stderr, "widget get type failed: %d\n", ret);
		goto out;
	}
	switch (type) {
        case GP_WIDGET_MENU:
        case GP_WIDGET_RADIO:
        case GP_WIDGET_TEXT:
		break;
	default:
		fprintf (stderr, "widget has bad type %d\n", type);
		ret = GP_ERROR_BAD_PARAMETERS;
		goto out;
	}

	/* This is the actual set call. Note that we keep
	 * ownership of the string and have to free it if necessary.
	 */
	ret = gp_widget_set_value (child, val);
	if (ret < GP_OK) {
		fprintf (stderr, "could not set widget value: %d\n", ret);
		goto out;
	}
	/* This stores it on the camera again */
	ret = gp_camera_set_config (camera, widget, context);
	if (ret < GP_OK) {
		fprintf (stderr, "camera_set_config failed: %d\n", ret);
		return ret;
	}
out:
	gp_widget_free (widget);
	return ret;
}

void capture_to_file(Camera *camera, GPContext *context, char *fn) {

	int fd, retval;
	CameraFile *file;
	CameraFilePath camera_file_path;

	printf("Capturing.\n");

	/* NOP: This gets overridden in the library to /capt0000.jpg */
	strcpy(camera_file_path.folder, "/");
	strcpy(camera_file_path.name, "foo.jpg");

	retval = gp_camera_capture(camera, GP_CAPTURE_IMAGE, &camera_file_path, context);
	printf("  Retval: %d\n", retval);

	printf("Pathname on the camera: %s/%s\n", camera_file_path.folder, camera_file_path.name);

	fd = open(fn, O_CREAT | O_WRONLY, 0644);
	retval = gp_file_new_from_fd(&file, fd);
	printf("  Retval: %d\n", retval);
	retval = gp_camera_file_get(camera, camera_file_path.folder, camera_file_path.name,
		     GP_FILE_TYPE_NORMAL, file, context);
	printf("  Retval: %d\n", retval);

	printf("Deleting.\n");
	retval = gp_camera_file_delete(camera, camera_file_path.folder, camera_file_path.name,
			context);
	printf("  Retval: %d\n", retval);

	gp_file_free(file);
}

void get_folder_contents(Camera *camera, GPContext *context, char *folder) {

	CameraList *list;

	int i, ret;
	int list_count;
	char *name, *value;

	ret = gp_list_new(&list);
	if(ret < GP_OK)
		printf("No new list.\n");

	ret = gp_camera_folder_list_files(camera,folder,list,context);
	if(ret < GP_OK)
		printf("Unable to list files.\n");

	list_count = gp_list_count(list);
	for(i = 0; i < list_count; i++) {

		gp_list_get_name(list, i, &name);
		gp_list_get_value(list, i, &value);

		printf("Name: %s\n", name);
		//printf("Value: %s\n", value);

	}

	gp_list_free(list);
}


void get_folder_contents();
void get_capture(Camera *camera, GPContext *context) {

	char *folder_one = "/store_00010001/DCIM/107D7000";
	char *folder_two = "/store_00010001/DCIM/108D7000";
	char *folder_three = "/store_00020001/DCIM/107D7000";
	char *folder_four = "/store_00020001/DCIM/108D7000";
	char *image_name = "/tmp/TST_%u.jpg";
	char *feh_command = "feh --draw-exif --scale-down %s";
	char buf[1024];
	char cmdbuf[512];
	time_t result = time(NULL);
	char *name, *value;
	CameraFilePath cfilepath;
	CameraFileInfo info;
	CameraFile *cfile;
	int fd, ret, sysret;

	if(result != -1)
		sprintf(buf, image_name, result); //Sets the image name as /tmp/TST_unixtimestamp.jpg

	sprintf(cmdbuf, feh_command, buf); //Sets FEH command as: feh --draw-exif --scale-down /tmp/TST_unixtimestamp.jpg

	strcpy(cfilepath.folder, folder_four);
	strcpy(cfilepath.name, "TST_001.jpg");

	printf("Capturing image...\n");
	ret = gp_camera_capture(camera, GP_CAPTURE_IMAGE, &cfilepath, context);
	if(ret >= GP_OK)
		printf("Image capture OK!\n");
	else
		printf("Image capture ERROR!\n");

	fd = open(buf, O_CREAT | O_WRONLY, 0664);

	ret = gp_file_new_from_fd(&cfile, fd);
	if(ret >= GP_OK)
		printf("New file descriptor OK!\n");
	else
		printf("New file descriptor ERROR!\n");

	printf("Getting file from camera...\n");
	ret = gp_camera_file_get(camera, cfilepath.folder, cfilepath.name, GP_FILE_TYPE_NORMAL, cfile, context);
	if(ret >= GP_OK) {

		printf("File copied from camera OK!\n");
		printf("File located here: %s\n", buf);
		sysret = system(cmdbuf);

		if(sysret >= 0)
			printf("feh executed OK!\n");

	} else
		printf("File copied from camera ERROR!\n");

	gp_file_free(cfile);

}


char* get_iso(Camera *camera, GPContext *context) {

	int ret;
	char *iso;

	ret = get_config_value_string(camera, "iso", &iso, context);
	if(ret >= GP_OK)
		return iso;
	else {
		free(iso);
		return NULL;
	}


}

int set_iso(Camera *camera, GPContext *context, const char *iso) {

	int ret;

	ret = set_config_value_string(camera, "iso", iso, context);
	if(ret >= GP_OK)
		return GP_OK;
	else
		return ret;

}

char* get_aperture(Camera *camera, GPContext *context) {

	int ret;
	char *aperture;

	ret = get_config_value_string(camera, "aperture", &aperture, context);
	if(ret >= GP_OK)
		return aperture;
	else {
		free(aperture);
		return NULL;
	}
}

int set_aperture(Camera *camera, GPContext *context, const char *aperture) {

	int ret;

	ret = set_config_value_string(camera, "f-stop", aperture, context);
	if(ret >= GP_OK)
		return GP_OK;
	else
		return ret;
}

char* get_shutterspeed(Camera *camera, GPContext *context) {

	int ret;
	char *shutterspeed;

	ret = get_config_value_string(camera, "shutterspeed", &shutterspeed, context);
	if(ret >= GP_OK)
		return shutterspeed;
	else {
		free(shutterspeed);
		return NULL;
	}


}

int set_shutterspeed(Camera *camera, GPContext *context, const char *shutterspeed) {

	int ret;

	ret = set_config_value_string(camera, "shutterspeed", shutterspeed, context);
	if(ret >= GP_OK)
		return GP_OK;
	else
		return ret;

}

char* get_battery(Camera *camera, GPContext *context) {

	char *battery_level;
	int ret;

	ret = get_config_value_string(camera, "batterylevel", &battery_level, context);
	if(ret >= GP_OK)
		return battery_level;
	else {
		free(battery_level);
		return NULL;
	}
}

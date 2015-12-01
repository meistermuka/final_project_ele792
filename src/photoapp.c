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

int main(void) {

	int ret;
	// Camera specific variables
	//char *owner;
	char *iso_val;
	char *shutter_val;
	char *aperture_val;
	char *battery_level;

	Camera *camera;
	CameraText text;
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



	ret = get_config_value_string(camera, "iso", &iso_val, context);
	if(ret >= GP_OK) {
		printf("ISO: %s\n", iso_val);
		free(iso_val);
	}

	ret = get_config_value_string(camera, "shutterspeed", &shutter_val, context);
	if(ret >= GP_OK) {
		printf("Shutter: %s\n", shutter_val);
		free(shutter_val);
	}

	ret = get_config_value_string(camera, "f-number", &aperture_val, context);
	if(ret >= GP_OK) {
		printf("Aperture: %s\n", aperture_val);
		free(aperture_val);
	}


	ret = get_config_value_string(camera, "batterylevel", &battery_level, context);
	if(ret >= GP_OK) {
		printf("Battery: %s\n", battery_level);
		free(battery_level);
	}


	// Code clean up area
	gp_camera_exit(camera, context);
	gp_camera_free(camera);
	gp_context_unref(context);

	return EXIT_SUCCESS;

}

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

int get_config_value_string(Camera *camera, const char *key, char **str, GPContext *context) {

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
int set_config_value_string(Camera *camera, const char *key, const char *val, GPContext *context) {
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
void get_capture(Camera *camera, GPContext *context) {

	char *image_name = "/tmp/TST_%u.jpg";
	char *feh_command = "feh --draw-exif --scale-down %s &";
	char buf[1024];
	char cmdbuf[512];
	time_t result = time(NULL);
	CameraFilePath cfilepath;
	CameraFile *cfile;
	int fd, ret, sysret;

	if(result != -1)
		sprintf(buf, image_name, result); //Sets the image name as /tmp/TST_unixtimestamp.jpg

	sprintf(cmdbuf, feh_command, buf); //Sets FEH command as: feh --draw-exif --scale-down /tmp/TST_unixtimestamp.jpg

	strcpy(cfilepath.folder, FOLDER_FOUR);
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

char* get_exposure_comp(Camera *camera, GPContext *context) {

	int ret;
	char *exposurecompensation;

	ret = get_config_value_string(camera, "exposurecompensation", exposurecompensation, context);
	if(ret >= GP_OK)
		return exposurecompensation;
	else {
		free(exposurecompensation);
		return NULL;
	}
}
int set_exposure_comp(Camera *camera, GPContext *context, const char *exposurecomp) {

	int ret;

	ret = set_config_value_string(camera, "exposurecompensation", exposurecomp, context);
	if(ret >= GP_OK)
		return GP_OK;
	else
		return ret;
}
char* get_whitebalance(Camera *camera, GPContext *context) {

	int ret;
	char *whitebalance;

	ret = get_config_value_string(camera, "whitebalance", &whitebalance, context);
	if(ret >= GP_OK)
		return whitebalance;
	else {
		free(whitebalance);
		return NULL;
	}
}
int set_whitebalance(Camera *camera, GPContext *context, const char *whitebalance) {

	int ret;

	ret = set_config_value_string(camera, "whitebalance", whitebalance, context);
	if(ret >= GP_OK)
		return GP_OK;
	else
		return ret;
}
char* get_meteringmode_focus(Camera *camera, GPContext *context) {

	int ret;
	char *meteringmode;

	ret = get_config_value_string(camera, "focusmetermode", &meteringmode, context);
	if(ret >= GP_OK)
		return meteringmode;
	else
		return ret;
}

int set_meteringmode_focus(Camera *camera, GPContext *context, const char *meteringmodefocus) {

	int ret;

	ret = set_config_value_string(camera, "focusmetermode", meteringmodefocus, context);
	if(ret >= GP_OK)
		return GP_OK;
	else
		return ret;
}

char* get_meteringmode_exposure(Camera *camera, GPContext *context) {

	int ret;
	char *meteringmodeexposure;

	ret = get_config_value_string(camera, "exposuremetermode", &meteringmodeexposure, context);
	if(ret >= GP_OK)
		return meteringmodeexposure;
	else {
		free(meteringmodeexposure);
		return NULL;
	}
}

int set_meteringmode_exposure(Camera *camera, GPContext *context, const char *meteringmodeexposure) {

	int ret;

	ret = set_config_value_string(camera, "exposuremetermode", meteringmodeexposure, context);
	if(ret >= GP_OK)
		return GP_OK;
	else
		return ret;
}

char* get_focusmode(Camera *camera, GPContext *context) {

	int ret;
	char *focusmode;

	ret = get_config_value_string(camera, "focusmode", &focusmode, context);
	if(ret >= GP_OK)
		return focusmode;
	else {
		free(focusmode);
		return NULL;
	}
}

int set_focusmode(Camera *camera, GPContext *context, const char *focusmode) {

	int ret;

	ret = set_config_value_string(camera, "focusmode2", focusmode, context);
	if(ret >= GP_OK)
		return GP_OK;
	else
		return ret;
}

char* get_imagequality(Camera *camera, GPContext *context) {

	int ret;
	char *imagequality;

	ret = get_config_value_string(camera, "imagequality", &imagequality, context);
	if(ret >= GP_OK)
		return imagequality;
	else {
		free(imagequality);
		return NULL;
	}
}

int set_imagequality(Camera *camera, GPContext *context, const char *imagequality) {
	/*
	Choice: 0 JPEG Basic
	Choice: 1 JPEG Normal
	Choice: 2 JPEG Fine
	Choice: 3 NEF+Basic
	Choice: 4 NEF+Normal
	Choice: 5 NEF+Fine
	Choice: 6 TIFF (RGB)
	 */
	int ret;

	ret = set_config_value_string(camera, "imagequality", imagequality, context);
	if(ret >= GP_OK)
		return GP_OK;
	else
		return ret;

}


// TODO: correct get_lensinfo error on focallength
char* get_lensinfo(Camera *camera, GPContext *context) {

	int ret;
	char *lensinfo = (char *) malloc(1024);
	char *minfocallength;
	char *maxfocallength;
	char *focallength;

	strcpy(lensinfo, "Lens Info\n");

	ret = get_config_value_string(camera, "focallength", &focallength, context);
	if(ret >= GP_OK) {
		strcat(lensinfo, "Focal Length: ");
		strcat(lensinfo, focallength);
		strcat(lensinfo, "\n");
	} else
		printf("ERROR: %d on line (%u)\n", ret, __LINE__);

	ret = get_config_value_string(camera, "minfocallength", &minfocallength, context);
	if(ret >= GP_OK) {
		strcat(lensinfo, "Min Focal Length: ");
		strcat(lensinfo, minfocallength);
		strcat(lensinfo,"\n");
	} else
		printf("ERROR: %d on line (%u)\n", ret, __LINE__);

	ret = get_config_value_string(camera, "maxfocallength", &maxfocallength, context);
	if(ret >= GP_OK) {
		strcat(lensinfo, "Max Focal Length: ");
		strcat(lensinfo, maxfocallength);
		strcat(lensinfo, "\n");
	} else
		printf("ERROR: %d on line (%u)\n", ret, __LINE__);

	free(minfocallength);
	free(maxfocallength);
	free(focallength);

	return lensinfo;

}

char* get_flashinfo(Camera *camera, GPContext *context) {

	int ret;
	char *flashmode_value = (char *) malloc(1024);
	char *flashmode;
	char *nikonflashmode;
	char *flashmodemanualpower;
	char *flashsyncspeed;
	char *flashshutterspeed;
	char *flashexposurecompensation;
	char *flashopen;
	char *externalflash;

	strcpy(flashmode_value, "Flash Info\n");

	ret = get_config_value_string(camera, "flashmode", &flashmode, context);
	if(ret >= GP_OK) {
		strcat(flashmode_value, "Flash Mode: ");
		strcat(flashmode_value, flashmode);
		strcat(flashmode_value, "\n");
	} else
		printf("ERROR: %d on line (%u)\n", ret, __LINE__);

	ret = get_config_value_string(camera, "nikonflashmode", &nikonflashmode, context);
	if(ret >= GP_OK) {
		strcat(flashmode_value, "Nikon Flash Mode: ");
		strcat(flashmode_value, nikonflashmode);
		strcat(flashmode_value, "\n");
	} else
		printf("ERROR: %d on line (%u)\n", ret, __LINE__);

	ret = get_config_value_string(camera, "flashmodemanualpower", &flashmodemanualpower, context);
	if(ret >= GP_OK) {
		strcat(flashmode_value, "Manual Power: ");
		strcat(flashmode_value, flashmodemanualpower);
		strcat(flashmode_value, "\n");
	} else
		printf("ERROR: %d on line (%u)\n", ret, __LINE__);

	ret = get_config_value_string(camera, "flashsyncspeed", &flashsyncspeed, context);
	if(ret >= GP_OK) {
		strcat(flashmode_value, "Sync Speed: ");
		strcat(flashmode_value, flashsyncspeed);
		strcat(flashmode_value, "\n");
	} else
		printf("ERROR: %d on line (%u)\n", ret, __LINE__);

	ret = get_config_value_string(camera, "flashshutterspeed", &flashshutterspeed, context);
	if(ret >= GP_OK) {
		strcat(flashmode_value, "Shutter Speed: ");
		strcat(flashmode_value, flashshutterspeed);
		strcat(flashmode_value, "\n");
	} else
		printf("ERROR: %d on line (%u)\n", ret, __LINE__);

	ret = get_config_value_string(camera, "flashexposurecompensation", &flashexposurecompensation, context);
	if(ret >= GP_OK) {
		strcat(flashmode_value, "Exposure Compensation: ");
		strcat(flashmode_value, flashexposurecompensation);
		strcat(flashmode_value, "\n");
	} else
		printf("ERROR: %d on line (%u)\n", ret, __LINE__);

	ret = get_config_value_string(camera, "flashopen", &flashopen, context);
	if(ret >= GP_OK) {

	} else
		printf("ERROR: %d on line (%u)\n", ret, __LINE__);

	ret = get_config_value_string(camera, "externalflash", &externalflash, context);
	if(ret >= GP_OK) {

	} else
		printf("ERROR: %d on line (%u)\n", ret, __LINE__);

	free(flashsyncspeed);
	free(flashshutterspeed);
	free(flashexposurecompensation);
	free(flashopen);
	free(externalflash);
	free(flashmode);
	free(nikonflashmode);
	free(flashmodemanualpower);

	return flashmode_value;

}

char* get_camerainfo(Camera *camera, GPContext *context) {

	int ret;
	char *camerainfo = (char *) malloc(1024);
	char *cameramodel;
	char *manufacturer;
	char *serialnumber;
	char *deviceversion;

	strcpy(camerainfo, "Camera Info\n");

	ret = get_config_value_string(camera, "cameramodel", &cameramodel, context);
	if(ret >= GP_OK) {
		strcat(camerainfo, "Model: ");
		strcat(camerainfo, cameramodel);
		strcat(camerainfo, "\n");
	} else
		printf("ERROR: %d on line (%u)\n", ret, __LINE__);

	free(cameramodel);

	return camerainfo;
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

	ret = get_config_value_string(camera, "shutterspeed2", &shutterspeed, context);
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

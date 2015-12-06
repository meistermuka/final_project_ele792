/*
 * helpers.h
 *
 *  Created on: Dec 1, 2015
 *      Author: muka
 */

#ifndef HELPERS_H_
#define HELPERS_H_

#define FOLDER_ONE 		"/store_00010001/DCIM/107D7000"
#define FOLDER_TWO		"/store_00010001/DCIM/108D7000"
#define FOLDER_THREE	"/store_00020001/DCIM/107D7000"
#define FOLDER_FOUR		"/store_00020001/DCIM/108D7000"

extern GPContext* create_context(void);
extern int get_config_value_string (Camera *, const char *, char **, GPContext *);
extern int set_config_value_string (Camera *, const char *, const char *, GPContext *);
static int _lookup_widget(CameraWidget*, const char *, CameraWidget **);

void capture_to_file(Camera *, GPContext *, char *);

void get_capture(Camera *, GPContext *);

void get_folder_contents(Camera *, GPContext *, char *);

char* get_iso(Camera *, GPContext *);
int set_iso(Camera *, GPContext *, const char *);

char* get_aperture(Camera *, GPContext *);
int set_aperture(Camera *, GPContext *, const char *);

char* get_shutterspeed(Camera *, GPContext *);
int set_shutterspeed(Camera *, GPContext *, const char *);

char* get_battery(Camera *, GPContext *);

#endif /* HELPERS_H_ */

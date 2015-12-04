/*
 * helpers.h
 *
 *  Created on: Dec 1, 2015
 *      Author: muka
 */

#ifndef HELPERS_H_
#define HELPERS_H_

extern GPContext* create_context(void);
extern int get_config_value_string (Camera *, const char *, char **, GPContext *);
extern int set_config_value_string (Camera *, const char *, const char *, GPContext *);
static int _lookup_widget(CameraWidget*, const char *, CameraWidget **);

void capture_to_file(Camera *, GPContext *, char *);

char* get_iso(Camera *, GPContext *);
int set_iso(Camera *, GPContext *, const char *);

char* get_aperture(Camera *, GPContext *);
int set_aperture(Camera *, GPContext *, const char *);

char* get_shutterspeed(Camera *, GPContext *);
int set_shutterspeed(Camera *, GPContext *, const char *);

char* get_battery(Camera *, GPContext *);

#endif /* HELPERS_H_ */

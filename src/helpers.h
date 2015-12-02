/*
 * helpers.h
 *
 *  Created on: Dec 1, 2015
 *      Author: muka
 */

#ifndef HELPERS_H_
#define HELPERS_H_

extern GPContext* create_context(void);
extern int get_config_value_string (Camera *camera, const char *key, char **str, GPContext *context);
extern int set_config_value_string (Camera *, const char *, const char *, GPContext *);
static int _lookup_widget(CameraWidget*widget, const char *key, CameraWidget **child);

char* get_iso(Camera *camera, GPContext *context);
int set_iso(Camera *camera, GPContext *context, const char *);

char* get_aperture(Camera *camera, GPContext *context);
int set_aperture(Camera *camera, GPContext *context, const char *);

char* get_shutterspeed(Camera *camera, GPContext *context);
int set_shutterspeed(Camera *camera, GPContext *context, const char *);

char* get_battery(Camera *camera, GPContext *context);

#endif /* HELPERS_H_ */

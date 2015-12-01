/*
 * photoapp.h
 *
 *  Created on: Nov 30, 2015
 *      Author: muka
 */

#ifndef PHOTOAPP_H_
#define PHOTOAPP_H_

// Prototypes
extern GPContext* create_context(void);
extern int get_config_value_string (Camera *camera, const char *key, char **str, GPContext *context);
extern int set_config_value_string (Camera *, const char *, const char *, GPContext *);
static int _lookup_widget(CameraWidget*widget, const char *key, CameraWidget **child);


#endif /* PHOTOAPP_H_ */

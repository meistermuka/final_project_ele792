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
// Image Quality Constants
#define JPEG_BASIC		"JPEG Basic"
#define JPEG_NORMAL		"JPEG Normal"
#define JPEG_FINE		"JPEG Fine"
#define NEF_BASIC		"NEF+Basic"
#define NEF_NORMAL		"NEF+Normal"
#define NEF_FINE		"NEF+Fine"
#define TIFF_RGB		"TIFF (RGB)"
// Focus Mode Constants
#define AF_S			"AF-S"
#define AF_C			"AF-C"
#define AF_A			"AF-A"
// ISO Constants
#define ISO100			"100"
#define ISO125			"125"
#define ISO160			"160"
#define ISO200			"200"
#define ISO250			"250"
#define ISO320			"320"
#define ISO400			"400"
#define ISO500			"500"
#define ISO640			"640"
#define ISO800			"800"
#define ISO1000			"1000"
#define ISO1250			"1250"
#define ISO1600			"1600"
#define ISO2000 		"2000"
#define ISO2500			"2500"
#define ISO3200			"3200"
#define ISO4000			"4000"
#define ISO5000			"5000"
#define ISO6400			"6400"
#define ISO8000			"8000"
#define ISO10000		"10000"
#define ISO12800		"12800"
#define ISO25600		"25600"
// Shutterspeed Constants
#define SPEED1_8000		"1/8000"
#define SPEED1_6400		"1/6400"
#define SPEED1_5000		"1/5000"
#define SPEED1_4000		"1/4000"
#define SPEED1_3200		"1/3200"
#define SPEED1_2500		"1/2500"
#define SPEED1_2000		"1/2000"
#define SPEED1_1600		"1/1600"
#define SPEED1_1250		"1/1250"
#define SPEED1_1000		"1/1000"
#define SPEED1_800		"1/800"
#define SPEED1_640		"1/640"
#define SPEED1_500		"1/500"
#define SPEED1_400		"1/400"
#define SPEED1_320		"1/320"
#define SPEED1_250		"1/250"
#define SPEED1_200		"1/200"
#define SPEED1_160		"1/160"
#define SPEED1_125		"1/125"
#define SPEED1_100		"1/100"
#define SPEED1_80		"1/80"
#define SPEED1_60		"1/60"
#define SPEED1_50		"1/50"
#define SPEED1_40		"1/40"
#define SPEED1_30		"1/30"
#define SPEED1_25		"1/25"
#define SPEED1_20		"1/20"
#define SPEED1_15		"1/15"
#define SPEED1_13		"1/13"
#define SPEED1_10		"1/10"
#define SPEED1_8		"1/8"
#define SPEED1_6		"1/6"
#define SPEED1_5		"1/5"
#define SPEED1_4		"1/4"
#define SPEED1_3		"1/3"
#define SPEED10_25		"10/25"
#define SPEED1_2		"1/2"
#define SPEED10_16		"10/16"
#define SPEED10_13		"10/13"
#define SPEED1			"1"
#define SPEED13_10		"13/10"
#define SPEED16_10		"16/10"
#define SPEED2			"2"
#define SPEED25_10		"25/10"
#define SPEED3			"3"
#define SPEED4			"4"
#define SPEED5			"5"
#define SPEED6			"6"
#define SPEED8			"8"
#define SPEED10			"10"
#define SPEED13			"13"
#define SPEED15			"15"
#define SPEED20			"20"
#define SPEED25			"25"
#define SPEED30			"30"
// Focus Metering Mode
#define FMETER_MULTI_SPORT		"Multi-spot"
#define FMETER_SINGLE_AREA		"Single Area"
#define FMETER_CLOSEST_SUB		"Closest Subject"
#define FMETER_GROUP_DYNAMIC	"Group Dynamic"
// Exposure Metering Mode
#define EMETER_CENTER_WEIGHT	"Center Weighted"
#define EMETER_MULTI_SPOT		"Multi Spot"
#define EMETER_CENTER_SPOT		"Center Spot"


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

char* get_exposure_comp(Camera *, GPContext *);
int set_exposure_comp(Camera *, GPContext *, const char *);

char* get_whitebalance(Camera *, GPContext *);
int set_whitebalance(Camera *, GPContext *, const char *);

char* get_meteringmode_focus(Camera *, GPContext *);
int set_meteringmode_focus(Camera *, GPContext *, const char *);

char* get_focusmode(Camera *, GPContext *);
int set_focusmode(Camera *, GPContext *, const char *);

char* get_imagequality(Camera *, GPContext *);
int set_imagequality(Camera *, GPContext *, const char *);

char* get_lensinfo(Camera *, GPContext *);

char* get_flashinfo(Camera *, GPContext *);


#endif /* HELPERS_H_ */

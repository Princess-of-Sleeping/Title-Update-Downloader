
#include <psp2/kernel/processmgr.h>

#include <psp2/io/dirent.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/io/devctl.h>

#include <psp2/net/http.h>
#include <psp2/net/net.h>
#include <psp2/net/netctl.h>

#include <psp2/ctrl.h>
#include <psp2/power.h>
#include <psp2/shellutil.h>
#include <psp2/sysmodule.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "graphics.h"
#include "hmac-sha256.h"

#define printf psvDebugScreenPrintf
#define printf2 psvDebugScreenPrintf2
#define SetTextColor psvDebugScreenSetFgColor


char download_path[0x400];
char download_dir_path[0x400];
char titleid[0x200];
char hmac_ret[0x20];
char title_update_xml_url[0x400];
char buf[0x4000];
char buf_0x1000[0x1000];
char version[6];
char pkg_url[0x200];
char drive[8];

hmac_sha256 hmac;
SceIoStat stat;

/* ctrl.c */

int get_key(int type);
void press_exit(void);
void not_press_wait(void);


/* gupd.c */

int sceGameUpdatePackageDownload(void);









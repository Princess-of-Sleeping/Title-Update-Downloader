#include "main.h"

int main(int argc, char *argv[]){

	scePowerSetArmClockFrequency(444);

	sceKernelPowerLock(0);

	sceShellUtilInitEvents(0);
	sceShellUtilLock(SCE_SHELL_UTIL_LOCK_TYPE_PS_BTN);

	psvDebugScreenInit();

	sceGameUpdatePackageDownload();

	printf("*** ALL DONE ***\n\n");

	press_exit();



	return 0;
}

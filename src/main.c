#include "main.h"

int main(int argc, char *argv[]){

	scePowerSetArmClockFrequency(444);

	sceKernelPowerLock(0);

	sceShellUtilInitEvents(0);
	sceShellUtilLock(0xFFF);

	psvDebugScreenInit();

	sceInstallGamesUpdatePackageDownload();

	printf("*** ALL DONE ***\n\n");
	printf("Press any key to exit this application.\n");

	sceKernelPowerUnlock(0);
	sceShellUtilUnlock(0xFFF);

	press_exit();



	return 0;
}

#include "main.h"

uint8_t key[] = {
		0xE5, 0xE2, 0x78, 0xAA, 0x1E, 0xE3, 0x40, 0x82, 0xA0, 0x88, 0x27, 0x9C, 0x83, 0xF9, 0xBB, 0xC8,
		0x06, 0x82, 0x1C, 0x52, 0xF2, 0xAB, 0x5D, 0x2B, 0x4A, 0xBD, 0x99, 0x54, 0x50, 0x35, 0x51, 0x14
		};

int IsExistDrive(char *drive){

	int ret = sceIoDopen(drive);

	if(ret >= 0){

		sceIoDclose(ret);
		return 0;

	}
	return ret;

}

uint64_t getPartitionSize(char *partition, char *arg) {

	SceIoStat stat;
	memset(&stat, 0, sizeof(SceIoStat));

	int stat_ret = sceIoGetstat(partition, &stat);

	if (stat_ret < 0){
		return stat_ret;
	}

	SceIoDevInfo info;
	memset(&info, 0, sizeof(SceIoDevInfo));
	int res = sceIoDevctl(partition, 0x3001, 0, 0, &info, sizeof(SceIoDevInfo));
	if (res < 0){
		return res;
	}

	if (memcmp(arg, "FreeSize", 8) == 0) {
		return info.free_size;
	}  else if (memcmp(arg, "MaxSize", 7) == 0) {
		return info.max_size;
	}

	return -1;

}


void netInit() {
	sceSysmoduleLoadModule(SCE_SYSMODULE_NET);
	
	SceNetInitParam netInitParam;
	int size = 1*1024*1024;
	netInitParam.memory = malloc(size);
	netInitParam.size = size;
	netInitParam.flags = 0;
	sceNetInit(&netInitParam);

	sceNetCtlInit();
}

void httpInit() {
	sceSysmoduleLoadModule(SCE_SYSMODULE_HTTP);

	sceHttpInit(1*1024*1024);
}


int sceInstallGamesUpdatePackageDownload(void){

	int res = 0;


	strcpy(drive, "host0:");

	if(IsExistDrive(drive) < 0){
		strcpy(drive, "sd0:");
	}

	if(IsExistDrive(drive) < 0){
		strcpy(drive, "uma0:");
	}

	if(IsExistDrive(drive) < 0){
		strcpy(drive, "ux0:");
	}

	sprintf(download_dir_path, "%spackage/", drive);


	sceIoMkdir(download_dir_path, 0777);



	/*

		The base icon of the bubble displayed in SceShell is here.
		Icon information of the game card etc will also be saved here unless you delete the bubble.

	 */
	SceUID dfd0 = sceIoDopen("ux0:appmeta/");

	res = 0;

	do {

		netInit();
		httpInit();

		SceIoDirent dir;
		memset(&dir, 0, sizeof(SceIoDirent));

		res = sceIoDread(dfd0, &dir);
		if (res > 0 && (SCE_S_ISDIR(dir.d_stat.st_mode)) && (memcmp(dir.d_name, "PCS", 3) == 0)) {

			memset(titleid, 0, sizeof(titleid));
			memset(&hmac, 0, sizeof(hmac));
			memset(hmac_ret, 0, sizeof(hmac_ret));
			memset(title_update_xml_url, 0, sizeof(title_update_xml_url));
			memset(buf, 0, sizeof(buf));			memset(version, 0, sizeof(version));
			memset(download_path, 0, sizeof(download_path));
			memset(&stat, 0, sizeof(stat));
			memset(pkg_url, 0, sizeof(pkg_url));
			memset(drive, 0, sizeof(drive));

			sprintf(titleid, "np_%s", dir.d_name);

			hmac_sha256_initialize(&hmac, key, sizeof(key));

			hmac_sha256_update(&hmac, (void*)titleid, strlen(titleid));

			hmac_sha256_finalize(&hmac, NULL, 0);

			for (int i = 0; i < 32; ++i) {

				if(i == 0){
					sprintf(hmac_ret, "%02x", hmac.digest[i]);
				}else{
					sprintf(hmac_ret, "%s%02x", hmac_ret, hmac.digest[i]);
				}

			}


			SetTextColor(COLOR_CYAN);
			printf("#");
			SetTextColor(COLOR_WHITE);

			printf2("%s %s\n\n", dir.d_name, hmac_ret);



			sprintf(title_update_xml_url,
				"http://gs-sec.ww.np.dl.playstation.net/pl/np/%s/%s/%s-ver.xml",
				dir.d_name,
				hmac_ret,
				dir.d_name);



			uint64_t length = 0;


			int ret1, tpl, conn, req;

			tpl = sceHttpCreateTemplate("PsVita TitleUpdate", 2, 1);
			conn = sceHttpCreateConnectionWithURL(tpl, title_update_xml_url, 0);
			req = sceHttpCreateRequestWithURL(conn, 0, title_update_xml_url, 0);
			ret1 = sceHttpSendRequest(req, NULL, 0);
			ret1 = sceHttpGetResponseContentLength(req, &length);
			sceHttpReadData(req, buf, sizeof(buf));


			int i;

			char *test1;
			char *test2;

			for(i=0;i<sizeof(buf);i++){

				test1 = (buf + i);

				if(strncmp(test1, "<pac", 4) == 0){

					buf[i-1] = 0;
					for(int i1=i;i1<sizeof(buf);i1++){

						test2 = (buf + i1);

						if(strncmp(test2, ">", 1) == 0){
							buf[i1+1] = 0;
						}
					}
					for(int i2=0;i2<sizeof(buf)/2;i2++){
							buf[i2] = buf[i2+i];
					}
					break;
				}


			}



			if ((uint64_t)length < 100) {
				printf2("There is no update to this game.\n\n\n");

				goto error;
				//continue;
			}



			char *test3;


			for(i=0;i<sizeof(buf);i++){

				test3 = (buf + i);

				if(strncmp(test3, "on=\"", 4) == 0){

					for(int ij=0;ij<sizeof(buf)/2;ij++){
						buf[ij] = buf[ij+i+4];
						if(buf[ij] == 0x22){
							buf[ij] = 0;
							break;
						}
					}
					break;
				}
			}


			strcpy(version, buf);
			printf2("Update Version : %s\n\n", version);


			sprintf(download_path, "%s%s-%s.pkg", download_dir_path, dir.d_name, version);

			sceIoGetstat(download_path, &stat);


			char *test4;

			for(i=0;i<sizeof(buf);i++){

				test4 = (buf + i);

				if(strncmp(test4, "url=\"", 5) == 0){

					for(int ij=0;ij<sizeof(buf)/2;ij++){
						buf[ij] = buf[ij+i+5];
						if(buf[ij] == 0x22){
							buf[ij] = 0;
							break;
						}
					}
					break;
				}
			}



			strcpy(pkg_url, buf);

			//printf("%s\n", pkg_url);


			//tpl = sceHttpCreateTemplate("PsVita TitleUpdate", 2, 1);
			conn = sceHttpCreateConnectionWithURL(tpl, pkg_url, 0);
			req = sceHttpCreateRequestWithURL(conn, 0, pkg_url, 0);
			sceHttpSendRequest(req, NULL, 0);
			sceHttpGetResponseContentLength(req, &length);




			if((uint64_t)stat.st_size == (uint64_t)length){
				printf2("This version of PKG has already been downloaded.\n\n\n");
				goto error;
			}




			//printf("PKG Size : %lld\n", (uint32_t)length);


			printf2("free space of memory card ... ");



			if(getPartitionSize(drive, "FreeSize") > (uint64_t)length){

				printf2("ok\n\n");

			}else{

				printf2("There is not enough free space\n");
				printf2("A downloading space of %lld bytes is necessary.\n", ((uint64_t)length - getPartitionSize("ux0:", 0)));

				goto error;

				//continue;

			}



			printf2("Download Path : %s\n\n", download_path);

			printf2("Downloading : ");

			int fd = sceIoOpen(download_path, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 6);

			if(fd < 0){printf2("File Open Error.\n\n");continue;}

			int total_read = 0, read = 0;
			int x = psvDebugScreenGetX();
			int y = psvDebugScreenGetY();

			SceCtrlData pad;

			while (1) {

				read = sceHttpReadData(req, buf, sizeof(buf));

				if (read < 0) {
					sceIoClose(fd);
					sceIoRemove(download_path);

					fd = -1;

					psvDebugScreenSetXY(0, 0);
					printf2("sceHttpReadData error! ret : 0x%x", read);
					break;
				}

				if (read == 0)break;

				ret1 = sceIoWrite(fd, buf, read);

				if (ret1 < 0 || ret1 != read) {
					psvDebugScreenSetXY(0, 0);

					printf2("sceIoWrite error!\nret : 0x%x", ret1);
					if (ret1 < 0)
						return ret1;
					return -1;
				}

				total_read += read;

				psvDebugScreenSetXY(x, y);

				printf("%d / %d", total_read, (uint32_t)length);

				sceCtrlPeekBufferPositive(0, &pad, 1);

				if (pad.buttons != 0){

					sceIoClose(fd);
					sceIoRemove(download_path);

					fd = -1;

					not_press_wait();

					printf2("\n\nCanceled by user.");

					break;
				}


			}

			printf("\n\n\n");
			if(fd >= 0)sceIoClose(fd);


				error:sceKernelDelayThread(1000);

			sceHttpTerm();
			//UnloadHttp();
			//UnloadNet();


		}
	} while (res > 0);

	sceIoDclose(dfd0);

	printf("\n");

	return res;

}
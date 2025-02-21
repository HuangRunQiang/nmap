/*
 * Copyright (c) 2002 - 2005 NetGroup, Politecnico di Torino (Italy)
 * Copyright (c) 2005 - 2008 CACE Technologies, Davis (California)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Politecnico di Torino, CACE Technologies
 * nor the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "ftmacros.h"
#include "diag-control.h"

/*
 * sockutils.h may include <crtdbg.h> on Windows, and pcap-int.h will
 * include portability.h, and portability.h, on Windows, expects that
 * <crtdbg.h> has already been included, so include sockutils.h first.
 */
#include "sockutils.h"
#include "pcap-int.h"	// for the details of the pcap_t structure
#include "pcap-rpcap.h"
#include "rpcap-protocol.h"
#include <errno.h>		// for the errno variable
#include <stdlib.h>		// for malloc(), free(), ...
#include <string.h>		// for strstr, etc

#ifndef _WIN32
#include <dirent.h>		// for readdir
#endif

/* String identifier to be used in the pcap_findalldevs_ex() */
#define PCAP_TEXT_SOURCE_FILE "File"
#define PCAP_TEXT_SOURCE_FILE_LEN (sizeof PCAP_TEXT_SOURCE_FILE - 1)
/* String identifier to be used in the pcap_findalldevs_ex() */
#define PCAP_TEXT_SOURCE_ADAPTER "Network adapter"
#define PCAP_TEXT_SOURCE_ADAPTER_LEN (sizeof "Network adapter" - 1)

/* String identifier to be used in the pcap_findalldevs_ex() */
#define PCAP_TEXT_SOURCE_ON_LOCAL_HOST "on local host"
#define PCAP_TEXT_SOURCE_ON_LOCAL_HOST_LEN (sizeof PCAP_TEXT_SOURCE_ON_LOCAL_HOST + 1)

/****************************************************
 *                                                  *
 * Function bodies                                  *
 *                                                  *
 ****************************************************/

int pcap_findalldevs_ex(const char *source, struct pcap_rmtauth *auth, pcap_if_t **alldevs, char *errbuf)
{
	int type;
	char name[PCAP_BUF_SIZE], path[PCAP_BUF_SIZE], filename[PCAP_BUF_SIZE];
	size_t pathlen;
	size_t stringlen;
	pcap_t *fp;
	char tmpstring[PCAP_BUF_SIZE + 1];		/* Needed to convert names and descriptions from 'old' syntax to the 'new' one */
	pcap_if_t *lastdev;	/* Last device in the pcap_if_t list */
	pcap_if_t *dev;		/* Device we're adding to the pcap_if_t list */

	/* List starts out empty. */
	(*alldevs) = NULL;
	lastdev = NULL;

	if (strlen(source) > PCAP_BUF_SIZE)
	{
		snprintf(errbuf, PCAP_ERRBUF_SIZE, "The source string is too long. Cannot handle it correctly.");
		return -1;
	}

	/*
	 * Determine the type of the source (file, local, remote)
	 * There are some differences if pcap_findalldevs_ex() is called to list files and remote adapters.
	 * In the first case, the name of the directory we have to look into must be present (therefore
	 * the 'name' parameter of the pcap_parsesrcstr() is present).
	 * In the second case, the name of the adapter is not required (we need just the host). So, we have
	 * to use a first time this function to get the source type, and a second time to get the appropriate
	 * info, which depends on the source type.
	 */
	if (pcap_parsesrcstr(source, &type, NULL, NULL, NULL, errbuf) == -1)
		return -1;

	switch (type)
	{
	case PCAP_SRC_IFLOCAL:
		if (pcap_parsesrcstr(source, &type, NULL, NULL, NULL, errbuf) == -1)
			return -1;

		/* Initialize temporary string */
		tmpstring[PCAP_BUF_SIZE] = 0;

		/* The user wants to retrieve adapters from a local host */
		if (pcap_findalldevs(alldevs, errbuf) == -1)
			return -1;

		if (*alldevs == NULL)
		{
			snprintf(errbuf, PCAP_ERRBUF_SIZE,
				"No interfaces found! Make sure libpcap/Npcap is properly installed"
				" on the local machine.");
			return -1;
		}

		/* Scan all the interfaces and modify name and description */
		/* This is a trick in order to avoid the re-implementation of the pcap_findalldevs here */
		dev = *alldevs;
		while (dev)
		{
			char *localdesc, *desc;

			/* Create the new device identifier */
			if (pcap_createsrcstr(tmpstring, PCAP_SRC_IFLOCAL, NULL, NULL, dev->name, errbuf) == -1)
				return -1;

			/* Delete the old pointer */
			free(dev->name);

			/* Make a copy of the new device identifier */
			dev->name = strdup(tmpstring);
			if (dev->name == NULL)
			{
				pcap_fmt_errmsg_for_errno(errbuf,
				    PCAP_ERRBUF_SIZE, errno,
				    "malloc() failed");
				pcap_freealldevs(*alldevs);
				return -1;
			}

			/*
			 * Create the description.
			 */
			if ((dev->description == NULL) || (dev->description[0] == 0))
				localdesc = dev->name;
			else
				localdesc = dev->description;
			if (pcap_asprintf(&desc, "%s '%s' %s",
			    PCAP_TEXT_SOURCE_ADAPTER, localdesc,
			    PCAP_TEXT_SOURCE_ON_LOCAL_HOST) == -1)
			{
				pcap_fmt_errmsg_for_errno(errbuf,
				    PCAP_ERRBUF_SIZE, errno,
				    "malloc() failed");
				pcap_freealldevs(*alldevs);
				return -1;
			}

			/* Now overwrite the description */
			free(dev->description);
			dev->description = desc;

			dev = dev->next;
		}

		return 0;

	case PCAP_SRC_FILE:
	{
#ifdef _WIN32
		WIN32_FIND_DATA filedata;
		HANDLE filehandle;
#else
		struct dirent *filedata;
		DIR *unixdir;
#endif

		if (pcap_parsesrcstr(source, &type, NULL, NULL, name, errbuf) == -1)
			return -1;

		/* Check that the filename is correct */
		stringlen = strlen(name);

		/* The directory must end with '\' in Win32 and '/' in UNIX */
#ifdef _WIN32
#define ENDING_CHAR '\\'
#else
#define ENDING_CHAR '/'
#endif

		if (name[stringlen - 1] != ENDING_CHAR)
		{
			name[stringlen] = ENDING_CHAR;
			name[stringlen + 1] = 0;

			stringlen++;
		}

		/* Save the path for future reference */
		snprintf(path, sizeof(path), "%s", name);
		pathlen = strlen(path);

#ifdef _WIN32
		/* To perform directory listing, Win32 must have an 'asterisk' as ending char */
		if (name[stringlen - 1] != '*')
		{
			name[stringlen] = '*';
			name[stringlen + 1] = 0;
		}

		filehandle = FindFirstFile(name, &filedata);

		if (filehandle == INVALID_HANDLE_VALUE)
		{
			snprintf(errbuf, PCAP_ERRBUF_SIZE, "Error when listing files: does folder '%s' exist?", path);
			return -1;
		}

#else
		/* opening the folder */
		unixdir= opendir(path);

		/* get the first file into it */
		filedata= readdir(unixdir);

		if (filedata == NULL)
		{
			DIAG_OFF_FORMAT_TRUNCATION
			snprintf(errbuf, PCAP_ERRBUF_SIZE, "Error when listing files: does folder '%s' exist?", path);
			DIAG_ON_FORMAT_TRUNCATION
			closedir(unixdir);
			return -1;
		}
#endif

		/* Add all files we find to the list. */
		do
		{
#ifdef _WIN32
			/* Skip the file if the pathname won't fit in the buffer */
			if (pathlen + strlen(filedata.cFileName) >= sizeof(filename))
				continue;
			snprintf(filename, sizeof(filename), "%s%s", path, filedata.cFileName);
#else
			if (pathlen + strlen(filedata->d_name) >= sizeof(filename))
				continue;
			DIAG_OFF_FORMAT_TRUNCATION
			snprintf(filename, sizeof(filename), "%s%s", path, filedata->d_name);
			DIAG_ON_FORMAT_TRUNCATION
#endif

			fp = pcap_open_offline(filename, errbuf);

			if (fp)
			{
				/* allocate the main structure */
				dev = (pcap_if_t *)malloc(sizeof(pcap_if_t));
				if (dev == NULL)
				{
					pcap_fmt_errmsg_for_errno(errbuf,
					    PCAP_ERRBUF_SIZE, errno,
					    "malloc() failed");
					pcap_freealldevs(*alldevs);
#ifdef _WIN32
					FindClose(filehandle);
#else
					closedir(unixdir);
#endif
					return -1;
				}

				/* Initialize the structure to 'zero' */
				memset(dev, 0, sizeof(pcap_if_t));

				/* Append it to the list. */
				if (lastdev == NULL)
				{
					/*
					 * List is empty, so it's also
					 * the first device.
					 */
					*alldevs = dev;
				}
				else
				{
					/*
					 * Append after the last device.
					 */
					lastdev->next = dev;
				}
				/* It's now the last device. */
				lastdev = dev;

				/* Create the new source identifier */
				if (pcap_createsrcstr(tmpstring, PCAP_SRC_FILE, NULL, NULL, filename, errbuf) == -1)
				{
					pcap_freealldevs(*alldevs);
#ifdef _WIN32
					FindClose(filehandle);
#else
					closedir(unixdir);
#endif
					return -1;
				}

				dev->name = strdup(tmpstring);
				if (dev->name == NULL)
				{
					pcap_fmt_errmsg_for_errno(errbuf,
					    PCAP_ERRBUF_SIZE, errno,
					    "malloc() failed");
					pcap_freealldevs(*alldevs);
#ifdef _WIN32
					FindClose(filehandle);
#else
					closedir(unixdir);
#endif
					return -1;
				}

				/*
				 * Create the description.
				 */
				if (pcap_asprintf(&dev->description,
				    "%s '%s' %s", PCAP_TEXT_SOURCE_FILE,
				    filename, PCAP_TEXT_SOURCE_ON_LOCAL_HOST) == -1)
				{
					pcap_fmt_errmsg_for_errno(errbuf,
					    PCAP_ERRBUF_SIZE, errno,
					    "malloc() failed");
					pcap_freealldevs(*alldevs);
#ifdef _WIN32
					FindClose(filehandle);
#else
					closedir(unixdir);
#endif
					return -1;
				}

				pcap_close(fp);
			}
		}
#ifdef _WIN32
		while (FindNextFile(filehandle, &filedata) != 0);
#else
		while ( (filedata= readdir(unixdir)) != NULL);
#endif


		/* Close the search handle. */
#ifdef _WIN32
		FindClose(filehandle);
#else
		closedir(unixdir);
#endif

		return 0;
	}

	case PCAP_SRC_IFREMOTE:
		return pcap_findalldevs_ex_remote(source, auth, alldevs, errbuf);

	default:
		pcap_strlcpy(errbuf, "Source type not supported", PCAP_ERRBUF_SIZE);
		return -1;
	}
}

/**
 * @brief 打开一个pcap会话。
 *
 * 此函数用于打开一个pcap会话，根据提供的参数来确定会话的类型和属性。
 *
 * @param source 源字符串，指定文件、本地设备或远程设备
 * @param snaplen 捕获数据包的快照长度
 * @param flags 标志，指定会话的特性
 * @param read_timeout 读取超时时间
 * @param auth 远程认证信息
 * @param errbuf 错误信息缓冲区
 * @return 返回pcap_t指针，表示打开的会话；如果打开失败，则返回NULL
 */
pcap_t *pcap_open(const char *source, int snaplen, int flags, int read_timeout, struct pcap_rmtauth *auth, char *errbuf)
{
    char name[PCAP_BUF_SIZE];
    int type;
    pcap_t *fp;
    int status;

    /*
     * 空设备名称等同于“any”设备 -
     * 这可能在此平台上不受支持，但这意味着您将获得“不受支持”错误
     * 而不是，比如，我们尝试取消引用空指针时崩溃。
     */
    if (source == NULL)
        source = "any";

    if (strlen(source) > PCAP_BUF_SIZE)
    {
        snprintf(errbuf, PCAP_ERRBUF_SIZE, "源字符串太长，无法正确处理。");
        return NULL;
    }

    /*
     * 确定源的类型（文件、本地、远程），
     * 并且如果是文件或本地设备，则获取文件名或捕获设备的名称。
     */
    if (pcap_parsesrcstr(source, &type, NULL, NULL, name, errbuf) == -1)
        return NULL;

    switch (type)
    {
    case PCAP_SRC_FILE:
        return pcap_open_offline(name, errbuf);

    case PCAP_SRC_IFLOCAL:
        fp = pcap_create(name, errbuf);
        break;

    case PCAP_SRC_IFREMOTE:
        /*
         * 虽然我们已经有主机、端口和接口，但我们更喜欢只传递“source”给pcap_open_rpcap()，
         * 以便它必须再次调用pcap_parsesrcstr()。
         * 这不太优化，但更清晰。
         */
        return pcap_open_rpcap(source, snaplen, flags, read_timeout, auth, errbuf);

    default:
        pcap_strlcpy(errbuf, "不支持的源类型", PCAP_ERRBUF_SIZE);
        return NULL;
    }

    if (fp == NULL)
        return NULL;
    status = pcap_set_snaplen(fp, snaplen);
    if (status < 0)
        goto fail;
    if (flags & PCAP_OPENFLAG_PROMISCUOUS)
    {
        status = pcap_set_promisc(fp, 1);
        if (status < 0)
            goto fail;
    }
    if (flags & PCAP_OPENFLAG_MAX_RESPONSIVENESS)
    {
        status = pcap_set_immediate_mode(fp, 1);
        if (status < 0)
            goto fail;
    }
#ifdef _WIN32
    /*
     * 仅在Windows上支持此标志。
     * XXX - 是否有办法在UNIX上支持它？
     * 这不完全是一个“设置方向”操作；
     * 我认为它的意思是“不要捕获使用pcap_sendpacket()或
     * pcap_inject()注入的数据包”。
     */
    /* 如果请求，禁用回环捕获 */
    if (flags & PCAP_OPENFLAG_NOCAPTURE_LOCAL)
        fp->opt.nocapture_local = 1;
#endif /* _WIN32 */
    status = pcap_set_timeout(fp, read_timeout);
    if (status < 0)
        goto fail;
    status = pcap_activate(fp);
    if (status < 0)
        goto fail;
    return fp;

fail:
    DIAG_OFF_FORMAT_TRUNCATION
    if (status == PCAP_ERROR)
        snprintf(errbuf, PCAP_ERRBUF_SIZE, "%s: %s",
                 name, fp->errbuf);
    else if (status == PCAP_ERROR_NO_SUCH_DEVICE ||
             status == PCAP_ERROR_PERM_DENIED ||
             status == PCAP_ERROR_PROMISC_PERM_DENIED)
        snprintf(errbuf, PCAP_ERRBUF_SIZE, "%s: %s (%s)",
                 name, pcap_statustostr(status), fp->errbuf);
    else
        snprintf(errbuf, PCAP_ERRBUF_SIZE, "%s: %s",
                 name, pcap_statustostr(status));
    DIAG_ON_FORMAT_TRUNCATION
    pcap_close(fp);
    return NULL;
}


struct pcap_samp *pcap_setsampling(pcap_t *p)
{
	return &p->rmt_samp;
}

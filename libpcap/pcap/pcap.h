/* -*- Mode: c; tab-width: 8; indent-tabs-mode: 1; c-basic-offset: 8; -*- */
/*
 * 版权所有 (c) 1993, 1994, 1995, 1996, 1997
 * 加利福尼亚大学校董会。保留所有权利。
 *
 * 在满足以下条件的情况下，允许以源代码和二进制形式重新分发和使用：
 * 1. 源代码的再分发必须保留上述版权声明、本条件列表和以下免责声明。
 * 2. 以二进制形式重新分发必须在文档和/或提供的其他材料中复制上述版权声明、本条件列表和以下免责声明。
 * 3. 所有提及此软件功能或使用的广告材料必须显示以下声明：
 *    本产品包括由劳伦斯伯克利实验室计算机系统工程小组开发的软件。
 * 4. 未经特定事先书面许可，不得使用大学或实验室的名称来认可或推广从本软件派生的产品。
 *
 * 本软件由版权所有者和贡献者 "按原样" 提供，任何明示或暗示的担保，
 * 包括但不限于对适销性和特定用途的适用性的暗示担保，
 * 都是被拒绝的。在任何情况下，版权所有者或贡献者均不对任何直接、间接、偶然、特殊、惩罚性或后果性损害（包括但不限于替代商品或服务的采购、使用、数据或利润损失或业务中断）承担任何责任，无论是合同责任、严格责任还是侵权行为（包括疏忽或其他）的理论，即使事先已被告知可能发生此类损害的可能性。
 *
 */

/*
 * WinPcap 的远程数据包捕获机制和扩展：
 *
 * 版权所有 (c) 2002 - 2003
 * Torino 高级技术学院 NetGroup (意大利)
 * 保留所有权利。
 *
 * 在满足以下条件的情况下，允许以源代码和二进制形式重新分发和使用：
 *
 * 1. 源代码的再分发必须保留上述版权声明、本条件列表和以下免责声明。
 * 2. 以二进制形式重新分发必须在文档和/或提供的其他材料中复制上述版权声明、本条件列表和以下免责声明。
 * 3. 不得使用 Torino 高级技术学院或其贡献者的名称来认可或推广从本软件派生的产品，除非事先获得特定的书面许可。
 *
 * 本软件由版权所有者和贡献者 "按原样" 提供，任何明示或暗示的担保，
 * 包括但不限于对适销性和特定用途的适用性的暗示担保，
 * 都是被拒绝的。在任何情况下，版权所有者或贡献者均不对任何直接、间接、偶然、特殊、惩罚性或后果性损害（包括但不限于替代商品或服务的采购、使用、数据或利润损失或业务中断）承担任何责任，无论是合同责任、严格责任还是侵权行为（包括疏忽或其他）的理论，即使事先已被告知可能发生此类损害的可能性。
 *
 */

#ifndef lib_pcap_pcap_h
#define lib_pcap_pcap_h

/*
 * 用于 pcap API 的错误代码。
 * 这些代码都是负数，因此您可以通过检查是否为负值来检查返回这些代码的调用的成功或失败。
 */
#define PCAP_ERROR			-1	/* 通用错误代码 */
#define PCAP_ERROR_BREAK		-2	/* 循环被 pcap_breakloop 终止 */
#define PCAP_ERROR_NOT_ACTIVATED	-3	/* 捕获需要激活 */
#define PCAP_ERROR_ACTIVATED		-4	/* 操作无法在已激活的捕获上执行 */
#define PCAP_ERROR_NO_SUCH_DEVICE	-5	/* 不存在此设备 */
#define PCAP_ERROR_RFMON_NOTSUP		-6	/* 此设备不支持 rfmon（监控）模式 */
#define PCAP_ERROR_NOT_RFMON		-7	/* 仅在监控模式下支持的操作 */
#define PCAP_ERROR_PERM_DENIED		-8	/* 没有权限打开设备 */
#define PCAP_ERROR_IFACE_NOT_UP		-9	/* 接口未启用 */
#define PCAP_ERROR_CANTSET_TSTAMP_TYPE	-10	/* 此设备不支持设置时间戳类型 */
#define PCAP_ERROR_PROMISC_PERM_DENIED	-11	/* 您没有以混杂模式捕获的权限 */
#define PCAP_ERROR_TSTAMP_PRECISION_NOTSUP -12  /* 请求的时间戳精度不受支持 */

/*
 * pcap API 的警告代码。
 * 这些代码都是正数且非零，因此它们看起来不像错误。
 */
#define PCAP_WARNING			1	/* 通用警告代码 */
#define PCAP_WARNING_PROMISC_NOTSUP	2	/* 此设备不支持混杂模式 */
#define PCAP_WARNING_TSTAMP_TYPE_NOTSUP	3	/* 不支持请求的时间戳类型 */

/*
 * 初始化选项。
 * 未在此处列出的所有位均保留用于扩展。
 *
 * 在类 UNIX 系统上，默认假定本地字符编码为 UTF-8，因此不进行字符编码转换。
 *
 * 在 Windows 上，默认的本地字符编码是本地 ANSI 代码页。
 */
#define PCAP_CHAR_ENC_LOCAL	0x00000000U	/* 字符串使用本地字符编码 */
#define PCAP_CHAR_ENC_UTF_8	0x00000001U	/* 字符串使用 UTF-8 编码 */

PCAP_AVAILABLE_1_10
PCAP_API int	pcap_init(unsigned int, char *);

/*
 * 我们正在废弃 pcap_lookupdev()，原因有多个（不是线程安全，在 WinPcap 中可能表现奇怪）。
 * 调用者应使用 pcap_findalldevs() 并使用第一个设备。
 */
PCAP_AVAILABLE_0_4
PCAP_DEPRECATED("use 'pcap_findalldevs' and use the first device")
PCAP_API char	*pcap_lookupdev(char *);

/**
 * @brief 查找网络设备的IP地址和子网掩码
 *
 * @param dev       网络设备名称
 * @param netp      存储网络地址的指针
 * @param maskp     存储子网掩码的指针
 * @param errbuf    存储错误信息的缓冲区
 *
 * @return 成功返回0，失败返回-1并设置错误信息到errbuf
 */
PCAP_API int pcap_lookupnet(const char *dev, bpf_u_int32 *netp, bpf_u_int32 *maskp, char *errbuf);

/**
 * @brief 创建一个用于捕获数据包的会话
 *
 * @param dev       网络设备名称
 * @param errbuf    存储错误信息的缓冲区
 *
 * @return 成功返回指向pcap_t结构体的指针，失败返回NULL并设置错误信息到errbuf
 */
PCAP_API pcap_t *pcap_create(const char *dev, char *errbuf);

/**
 * @brief 设置捕获数据包时的最大长度
 *
 * @param p         pcap_t结构体的指针
 * @param snaplen   最大长度
 *
 * @return 成功返回0，失败返回-1
 */
PCAP_API int pcap_set_snaplen(pcap_t *p, int snaplen);

/**
 * @brief 设置是否开启混杂模式
 *
 * @param p         pcap_t结构体的指针
 * @param promisc   是否开启混杂模式，非零表示开启，零表示关闭
 *
 * @return 成功返回0，失败返回-1
 */
PCAP_API int pcap_set_promisc(pcap_t *p, int promisc);

/**
 * @brief 检查是否可以设置RFMON模式
 *
 * @param p         pcap_t结构体的指针
 *
 * @return 可以设置返回非零值，否则返回零
 */
PCAP_API int pcap_can_set_rfmon(pcap_t *p);

/**
 * @brief 设置是否开启RFMON模式
 *
 * @param p         pcap_t结构体的指针
 * @param rfmon     是否开启RFMON模式，非零表示开启，零表示关闭
 *
 * @return 成功返回0，失败返回-1
 */
PCAP_API int pcap_set_rfmon(pcap_t *p, int rfmon);

/**
 * @brief 设置超时时间
 *
 * @param p         pcap_t结构体的指针
 * @param timeout   超时时间（毫秒）
 *
 * @return 成功返回0，失败返回-1
 */
PCAP_API int pcap_set_timeout(pcap_t *p, int timeout);

/**
 * @brief 设置时间戳类型
 *
 * @param p         pcap_t结构体的指针
 * @param type      时间戳类型
 *
 * @return 成功返回0，失败返回-1
 */
PCAP_API int pcap_set_tstamp_type(pcap_t *p, int type);

/**
 * @brief 设置是否开启立即模式
 *
 * @param p         pcap_t结构体的指针
 * @param immediate 是否开启立即模式，非零表示开启，零表示关闭
 *
 * @return 成功返回0，失败返回-1
 */
PCAP_API int pcap_set_immediate_mode(pcap_t *p, int immediate);

/**
 * @brief 设置缓冲区大小
 *
 * @param p         pcap_t结构体的指针
 * @param size      缓冲区大小（字节）
 *
 * @return 成功返回0，失败返回-1
 */
PCAP_API int pcap_set_buffer_size(pcap_t *p, int size);

/**
 * @brief 设置时间戳精度
 *
 * @param p         pcap_t结构体的指针
 * @param precision 时间戳精度
 *
 * @return 成功返回0，失败返回-1
 */
PCAP_API int pcap_set_tstamp_precision(pcap_t *p, int precision);

/**
 * @brief 获取时间戳精度
 *
 * @param p         pcap_t结构体的指针
 *
 * @return 返回时间戳精度
 */
PCAP_API int pcap_get_tstamp_precision(pcap_t *p);

/**
 * @brief 激活会话
 *
 * @param p         pcap_t结构体的指针
 *
 * @return 成功返回0，失败返回-1
 */
PCAP_API int pcap_activate(pcap_t *p);

/**
 * @brief 获取支持的时间戳类型列表
 *
 * @param p         pcap_t结构体的指针
 * @param tstamp_types 存储时间戳类型列表的指针
 *
 * @return 成功返回0，失败返回-1
 */
PCAP_API int pcap_list_tstamp_types(pcap_t *p, int **tstamp_types);

/**
 * @brief 释放时间戳类型列表
 *
 * @param tstamp_types 时间戳类型列表
 */
PCAP_API void pcap_free_tstamp_types(int *tstamp_types);

/**
 * @brief 将时间戳类型名称转换为对应的值
 *
 * @param name      时间戳类型名称
 *
 * @return 返回时间戳类型的值，如果无效返回-1
 */
PCAP_API int pcap_tstamp_type_name_to_val(const char *name);

/**
 * @brief 将时间戳类型的值转换为对应的名称
 *
 * @param type      时间戳类型的值
 *
 * @return 返回时间戳类型的名称，如果无效返回NULL
 */
PCAP_API const char *pcap_tstamp_type_val_to_name(int type);

/**
 * @brief 将时间戳类型的值转换为对应的描述
 *
 * @param type      时间戳类型的值
 *
 * @return 返回时间戳类型的描述，如果无效返回NULL
 */
PCAP_API const char *pcap_tstamp_type_val_to_description(int type);

#ifdef __linux__
/**
 * @brief 设置Linux特定的协议
 *
 * @param p         pcap_t结构体的指针
 * @param protocol  协议类型
 *
 * @return 成功返回0，失败返回-1
 */
PCAP_API int pcap_set_protocol_linux(pcap_t *p, int protocol);
#endif


/*
 * 时间戳类型。
 * 并非所有系统和接口都一定支持这些类型。
 *
 * 支持 PCAP_TSTAMP_HOST 的系统提供的时间戳是由主机机器提供的，
 * 而不是由捕获设备提供的，但不承诺时间戳的任何特性。
 *
 * PCAP_TSTAMP_HOST_LOWPREC 是由主机机器提供的时间戳，
 * 具有低精度但相对便宜；通常使用系统时钟完成，
 * 因此通常与从系统调用中获取的时间同步。
 *
 * PCAP_TSTAMP_HOST_HIPREC 是由主机机器提供的高精度时间戳；
 * 获取它可能更昂贵。它与系统时钟同步。
 *
 * PCAP_TSTAMP_HOST_HIPREC_UNSYNCED 是由主机机器提供的高精度时间戳；
 * 获取它可能更昂贵。它与系统时钟不同步，并且可能存在与在不同 CPU 上接收的数据包的时间戳有关的问题，这取决于平台。它可能比 PCAP_TSTAMP_HOST_HIPREC 更可能是严格单调的。
 *
 * PCAP_TSTAMP_ADAPTER 是由捕获设备提供的高精度时间戳；
 * 它与系统时钟同步。
 *
 * PCAP_TSTAMP_ADAPTER_UNSYNCED 是由捕获设备提供的高精度时间戳；
 * 它与系统时钟不同步。
 *
 * 请注意，与系统时钟同步的时间戳可能会后退，因为系统时钟可能会后退。
 * 如果时钟与系统时钟不同步，可能是因为系统时钟不保持准确时间、其他时钟不保持准确时间或两者都不保持准确时间。
 *
 * 请注意，主机提供的时间戳通常对应于时间戳代码看到数据包的时间；
 * 这可能是数据包的第一个或最后一个比特位接收到网络适配器后的未知时间量，这是由于数据包到达的中断批处理、排队延迟等。
 */
#define PCAP_TSTAMP_HOST			0	/* 主机提供的，未知特性 */
#define PCAP_TSTAMP_HOST_LOWPREC		1	/* 主机提供的，低精度，与系统时钟同步 */
#define PCAP_TSTAMP_HOST_HIPREC			2	/* 主机提供的，高精度，与系统时钟同步 */
#define PCAP_TSTAMP_ADAPTER			3	/* 设备提供的，与系统时钟同步 */
#define PCAP_TSTAMP_ADAPTER_UNSYNCED		4	/* 设备提供的，与系统时钟不同步 */
#define PCAP_TSTAMP_HOST_HIPREC_UNSYNCED	5	/* 主机提供的，高精度，与系统时钟不同步 */

/*
 * 时间戳分辨率类型。
 * 并非在进行实时捕获时所有系统和接口都一定支持所有这些分辨率；在读取保存文件时可以请求所有这些分辨率。
 */
#define PCAP_TSTAMP_PRECISION_MICRO	0	/* 使用微秒精度的时间戳，默认值 */
#define PCAP_TSTAMP_PRECISION_NANO	1	/* 使用纳秒精度的时间戳 */

/**
 * @file pcap.h
 * @brief PCAP library functions
 */

#ifndef PCAP_H
#define PCAP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 打开一个用于实时抓包的网络设备。
 *
 * 该函数打开一个用于实时抓包的网络设备，并返回一个指向 pcap_t 结构的指针。
 * 网络设备由其名称指定。
 *
 * @param device 要从中抓取的网络设备的名称。
 * @param snaplen 设置抓包句柄的快照长度。
 * @param promisc 一个指示是否将接口置于混杂模式的标志。
 * @param to_ms 读取超时值（以毫秒为单位）。
 * @param errbuf 如果函数失败，用于保存错误消息的缓冲区。
 * @return 成功时返回 pcap_t 结构的指针，失败时返回 NULL。
 */
PCAP_API pcap_t *pcap_open_live(const char *device, int snaplen, int promisc, int to_ms, char *errbuf);

/**
 * @brief 打开一个用于离线抓包的虚拟网络设备。
 *
 * 该函数打开一个用于离线抓包的虚拟网络设备，并返回一个指向 pcap_t 结构的指针。
 * 参数指定了虚拟设备的属性。
 *
 * @param linktype 虚拟设备的链路类型。
 * @param snaplen 设置抓包句柄的快照长度。
 * @return 成功时返回 pcap_t 结构的指针，失败时返回 NULL。
 */
PCAP_API pcap_t *pcap_open_dead(int linktype, int snaplen);

/**
 * @brief 打开一个用于离线抓包的虚拟网络设备，并指定时间戳精度。
 *
 * 该函数打开一个用于离线抓包的虚拟网络设备，并返回一个指向 pcap_t 结构的指针。
 * 参数指定了虚拟设备的属性和时间戳精度。
 *
 * @param linktype 虚拟设备的链路类型。
 * @param snaplen 设置抓包句柄的快照长度。
 * @param precision 时间戳的精度。
 * @return 成功时返回 pcap_t 结构的指针，失败时返回 NULL。
 */
PCAP_API pcap_t *pcap_open_dead_with_tstamp_precision(int linktype, int snaplen, u_int precision);

/**
 * @brief 打开一个带有时间戳精度的离线抓包文件。
 *
 * 该函数打开一个带有时间戳精度的离线抓包文件，并返回一个指向 pcap_t 结构的指针。
 * 参数指定了抓包文件的路径、时间戳精度和错误消息缓冲区。
 *
 * @param path 抓包文件的路径。
 * @param precision 时间戳的精度。
 * @param errbuf 如果函数失败，用于保存错误消息的缓冲区。
 * @return 成功时返回 pcap_t 结构的指针，失败时返回 NULL。
 */
PCAP_API pcap_t *pcap_open_offline_with_tstamp_precision(const char *path, u_int precision, char *errbuf);

/**
 * @brief 打开一个离线抓包文件。
 *
 * 该函数打开一个离线抓包文件，并返回一个指向 pcap_t 结构的指针。
 * 参数指定了抓包文件的路径和错误消息缓冲区。
 *
 * @param path 抓包文件的路径。
 * @param errbuf 如果函数失败，用于保存错误消息的缓冲区。
 * @return 成功时返回 pcap_t 结构的指针，失败时返回 NULL。
 */
PCAP_API pcap_t *pcap_open_offline(const char *path, char *errbuf);

#ifdef _WIN32
/**
 * @brief 使用时间戳精度打开一个带有句柄的离线抓包文件。
 *
 * 该函数使用时间戳精度打开一个带有句柄的离线抓包文件，并返回一个指向 pcap_t 结构的指针。
 * 参数指定了句柄、时间戳精度和错误消息缓冲区。
 *
 * @param handle 句柄。
 * @param precision 时间戳的精度。
 * @param errbuf 如果函数失败，用于保存错误消息的缓冲区。
 * @return 成功时返回 pcap_t 结构的指针，失败时返回 NULL。
 */
PCAP_API pcap_t *pcap_hopen_offline_with_tstamp_precision(intptr_t handle, u_int precision, char *errbuf);

/**
 * @brief 使用句柄打开一个离线抓包文件。
 *
 * 该函数使用句柄打开一个离线抓包文件，并返回一个指向 pcap_t 结构的指针。
 * 参数指定了句柄和错误消息缓冲区。
 *
 * @param handle 句柄。
 * @param errbuf 如果函数失败，用于保存错误消息的缓冲区。
 * @return 成功时返回 pcap_t 结构的指针，失败时返回 NULL。
 */
PCAP_API pcap_t *pcap_hopen_offline(intptr_t handle, char *errbuf);
#endif

#ifdef __cplusplus
}
#endif

#endif /* PCAP_H */

  /*
   * 如果正在构建 libpcap，则这些是 savefile.c 中的内部例程，
   * 因此不能将它们定义为宏。
   *
   * 如果未构建 libpcap，则鉴于使用 libpcap 构建的 C 运行时版本可能与使用 libpcap 的应用程序构建的 C 运行时版本不同，
   * 并且 FILE 结构可能因两个 C 运行时版本而异，调用 _fileno() 必须使用用于打开 FILE * 的 C 运行时版本中的版本，而不是用于构建 libpcap 的 C 运行时版本中的版本。
   * （也许一旦通用 CRT 统治世界，这将不再是一个问题。）
   */
  #ifndef BUILDING_PCAP
    #define pcap_fopen_offline_with_tstamp_precision(f,p,b) \
	pcap_hopen_offline_with_tstamp_precision(_get_osfhandle(_fileno(f)), p, b)
    #define pcap_fopen_offline(f,b) \
	pcap_hopen_offline(_get_osfhandle(_fileno(f)), b)
  #endif
#else /*_WIN32*/
  PCAP_AVAILABLE_1_5
  PCAP_API pcap_t	*pcap_fopen_offline_with_tstamp_precision(FILE *, u_int, char *);

  PCAP_AVAILABLE_0_9
  PCAP_API pcap_t	*pcap_fopen_offline(FILE *, char *);
#endif /*_WIN32*/

/**
 * @file pcap.h
 * @brief PCAP library functions
 */

/**
 * @brief 关闭pcap句柄，并释放相关资源
 * 
 * @param p pcap句柄的指针
 */
PCAP_AVAILABLE_0_4
PCAP_API void pcap_close(pcap_t *p);

/**
 * @brief 进入循环以捕获数据包并调用回调函数进行处理
 * 
 * @param p pcap句柄的指针
 * @param cnt 最大捕获数据包的数量，-1表示无限制
 * @param callback 指向回调函数的指针
 * @param user 指向用户数据的指针，将传递给回调函数
 * @return 当循环终止时返回-1，否则返回0
 */
PCAP_AVAILABLE_0_4
PCAP_API int pcap_loop(pcap_t *p, int cnt, pcap_handler callback, u_char *user);

/**
 * @brief 捕获数据包并调用回调函数进行处理，不进入循环
 * 
 * @param p pcap句柄的指针
 * @param cnt 最大捕获数据包的数量，-1表示无限制
 * @param callback 指向回调函数的指针
 * @param user 指向用户数据的指针，将传递给回调函数
 * @return 成功捕获并处理数据包的数量
 */
PCAP_AVAILABLE_0_4
PCAP_API int pcap_dispatch(pcap_t *p, int cnt, pcap_handler callback, u_char *user);

/**
 * @brief 获取pcap句柄的下一个数据包
 * 
 * @param p pcap句柄的指针
 * @param header 指向pcap_pkthdr结构的指针，用于存储数据包头部信息
 * @return 指向数据包的指针
 */
PCAP_AVAILABLE_0_4
PCAP_API const u_char *pcap_next(pcap_t *p, struct pcap_pkthdr *header);

/**
 * @brief 获取pcap句柄的下一个数据包，扩展版本
 * 
 * @param p pcap句柄的指针
 * @param header 指向指向pcap_pkthdr结构的指针的指针，用于存储数据包头部信息
 * @param packet 指向指向数据包的指针的指针，用于存储数据包
 * @return 返回值表示捕获到的数据包的类型
 *     - 正数：成功捕获到数据包，返回值表示数据包的长度
 *     - 0：超时，未捕获到数据包
 *     - 负数：出错，返回值表示错误代码
 */
PCAP_AVAILABLE_0_8
PCAP_API int pcap_next_ex(pcap_t *p, struct pcap_pkthdr **header, const u_char **packet);

/**
 * @brief 终止pcap循环捕获
 * 
 * 调用此函数将导致pcap_loop或pcap_dispatch函数立即返回。
 * 
 * @param p pcap句柄的指针
 */
PCAP_AVAILABLE_0_8
PCAP_API void pcap_breakloop(pcap_t *p);

/**
 * @brief 获取pcap统计信息
 * 
 * @param p pcap句柄的指针
 * @param stats 指向pcap_stat结构的指针，用于存储统计信息
 * @return 成功获取统计信息时返回0，否则返回-1
 */
PCAP_AVAILABLE_0_4
PCAP_API int pcap_stats(pcap_t *p, struct pcap_stat *stats);

/**
 * @brief 设置pcap过滤器
 * 
 * @param p pcap句柄的指针
 * @param fp 指向bpf_program结构的指针，表示过滤器程序
 * @return 成功设置过滤器时返回0，否则返回-1
 */
PCAP_AVAILABLE_0_4
PCAP_API int pcap_setfilter(pcap_t *p, struct bpf_program *fp);

/**
 * @brief 设置pcap捕获方向
 * 
 * @param p pcap句柄的指针
 * @param d 方向，可以是PCAP_D_IN、PCAP_D_OUT或PCAP_D_INOUT
 * @return 成功设置捕获方向时返回0，否则返回-1
 */
PCAP_AVAILABLE_0_9
PCAP_API int pcap_setdirection(pcap_t *p, pcap_direction_t d);

/**
 * @brief 获取pcap句柄的非阻塞模式状态
 * 
 * @param p pcap句柄的指针
 * @param errbuf 存储错误消息的缓冲区
 * @return 成功获取非阻塞模式状态时返回0，否则返回-1
 */
PCAP_AVAILABLE_0_7
PCAP_API int pcap_getnonblock(pcap_t *p, char *errbuf);

/**
 * @brief 设置pcap句柄的非阻塞模式状态
 * 
 * @param p pcap句柄的指针
 * @param nonblock 非阻塞模式标志，非0表示开启非阻塞模式，0表示关闭非阻塞模式
 * @param errbuf 存储错误消息的缓冲区
 * @return 成功设置非阻塞模式状态时返回0，否则返回-1
 */
PCAP_AVAILABLE_0_7
PCAP_API int pcap_setnonblock(pcap_t *p, int nonblock, char *errbuf);

/**
 * @brief 向pcap句柄注入数据包
 * 
 * @param p pcap句柄的指针
 * @param buf 指向要注入的数据包的指针
 * @param size 数据包的长度
 * @return 成功注入数据包时返回0，否则返回-1
 */
PCAP_AVAILABLE_0_9
PCAP_API int pcap_inject(pcap_t *p, const void *buf, size_t size);

/**
 * @brief 发送数据包到网络
 * 
 * @param p pcap句柄的指针
 * @param buf 指向要发送的数据包的指针
 * @param size 数据包的长度
 * @return 成功发送数据包时返回0，否则返回-1
 */
PCAP_AVAILABLE_0_8
PCAP_API int pcap_sendpacket(pcap_t *p, const u_char *buf, int size);

/**
 * @brief 将pcap错误代码转换为字符串
 * 
 * @param errnum pcap错误代码
 * @return 表示错误代码的字符串
 */
PCAP_AVAILABLE_1_0
PCAP_API const char *pcap_statustostr(int errnum);

/**
 * @brief 将错误代码转换为字符串
 * 
 * @param errnum 错误代码
 * @return 表示错误代码的字符串
 */
PCAP_AVAILABLE_0_4
PCAP_API const char *pcap_strerror(int errnum);

/**
 * @brief 获取pcap错误消息
 * 
 * @param p pcap句柄的指针
 * @return 表示错误消息的字符串
 */
PCAP_AVAILABLE_0_4
PCAP_API char *pcap_geterr(pcap_t *p);

/**
 * @brief 打印pcap错误消息
 * 
 * @param p pcap句柄的指针
 * @param prefix 错误消息前缀
 */
PCAP_AVAILABLE_0_4
PCAP_API void pcap_perror(pcap_t *p, const char *prefix);

/**
 * @brief 编译BPF过滤器程序
 * 
 * @param p pcap句柄的指针
 * @param fp 指向bpf_program结构的指针，用于存储编译后的过滤器程序
 * @param str 过滤器表达式字符串
 * @param optimize 是否优化过滤器程序
 * @param netmask 网络掩码
 * @return 成功编译过滤器程序时返回0，否则返回-1
 */
PCAP_AVAILABLE_0_4
PCAP_API int pcap_compile(pcap_t *p, struct bpf_program *fp, const char *str, int optimize, bpf_u_int32 netmask);

/**
 * @brief 编译BPF过滤器程序，不依赖pcap库
 * 
 * @param snaplen 最大捕获数据包的长度
 * @param linktype 数据链路类型
 * @param
  * @param fp 指向bpf_program结构的指针，用于存储编译后的过滤器程序
 * @param str 过滤器表达式字符串
 * @param optimize 是否优化过滤器程序
 * @param netmask 网络掩码
 * @return 成功编译过滤器程序时返回0，否则返回-1
 */
PCAP_AVAILABLE_0_5
PCAP_DEPRECATED("use pcap_open_dead(), pcap_compile() and pcap_close()")
PCAP_API int pcap_compile_nopcap(int snaplen, int linktype, struct bpf_program *fp, const char *str, int optimize, bpf_u_int32 netmask);

/**
 * @brief 释放BPF过滤器程序占用的内存
 * 
 * @param fp 指向bpf_program结构的指针
 */
PCAP_AVAILABLE_0_6
PCAP_API void pcap_freecode(struct bpf_program *fp);

/**
 * @brief 在离线模式下对数据包应用BPF过滤器
 * 
 * @param fp 指向bpf_program结构的指针，表示过滤器程序
 * @param header 指向pcap_pkthdr结构的指针，表示数据包头部信息
 * @param packet 指向数据包的指针
 * @return 数据包通过过滤器时返回非0值，否则返回0
 */
PCAP_AVAILABLE_1_0
PCAP_API int pcap_offline_filter(const struct bpf_program *fp, const struct pcap_pkthdr *header, const u_char *packet);

/**
 * @brief 获取pcap句柄的数据链路类型
 * 
 * @param p pcap句柄的指针
 * @return 数据链路类型
 */
PCAP_AVAILABLE_0_4
PCAP_API int pcap_datalink(pcap_t *p);

/**
 * @brief 获取pcap句柄的扩展数据链路类型
 * 
 * @param p pcap句柄的指针
 * @return 扩展数据链路类型
 */
PCAP_AVAILABLE_1_0
PCAP_API int pcap_datalink_ext(pcap_t *p);

/**
 * @brief 获取pcap句柄支持的所有数据链路类型
 * 
 * @param p pcap句柄的指针
 * @param dltlist 指向int数组的指针，用于存储数据链路类型列表
 * @return 成功获取数据链路类型列表时返回0，否则返回-1
 */
PCAP_AVAILABLE_0_8
PCAP_API int pcap_list_datalinks(pcap_t *p, int **dltlist);

/**
 * @brief 设置pcap句柄的数据链路类型
 * 
 * @param p pcap句柄的指针
 * @param dlt 数据链路类型
 * @return 成功设置数据链路类型时返回0，否则返回-1
 */
PCAP_AVAILABLE_0_8
PCAP_API int pcap_set_datalink(pcap_t *p, int dlt);

/**
 * @brief 释放由pcap_list_datalinks函数分配的内存
 * 
 * @param dltlist 数据链路类型列表的指针
 */
PCAP_AVAILABLE_0_8
PCAP_API void pcap_free_datalinks(int *dltlist);

/**
 * @brief 将数据链路类型名称转换为对应的数值
 * 
 * @param name 数据链路类型名称
 * @return 数据链路类型数值
 */
PCAP_AVAILABLE_0_8
PCAP_API int pcap_datalink_name_to_val(const char *name);

/**
 * @brief 将数据链路类型数值转换为对应的名称
 * 
 * @param dlt 数据链路类型数值
 * @return 数据链路类型名称
 */
PCAP_AVAILABLE_0_8
PCAP_API const char *pcap_datalink_val_to_name(int dlt);

/**
 * @brief 将数据链路类型数值转换为对应的描述
 * 
 * @param dlt 数据链路类型数值
 * @return 数据链路类型描述
 */
PCAP_AVAILABLE_0_8
PCAP_API const char *pcap_datalink_val_to_description(int dlt);

/**
 * @brief 将数据链路类型数值转换为对应的描述或DLT值
 * 
 * @param dlt 数据链路类型数值
 * @return 数据链路类型描述或DLT值
 */
PCAP_AVAILABLE_1_10
PCAP_API const char *pcap_datalink_val_to_description_or_dlt(int dlt);

/**
 * @brief 获取pcap句柄的快照长度
 * 
 * @param p pcap句柄的指针
 * @return 快照长度
 */
PCAP_AVAILABLE_0_4
PCAP_API int pcap_snapshot(pcap_t *p);

/**
 * @brief 检查pcap句柄是否使用了字节顺序转换
 * 
 * @param p pcap句柄的指针
 * @return 如果pcap句柄使用了字节顺序转换，则返回非0值，否则返回0
 */
PCAP_AVAILABLE_0_4
PCAP_API int pcap_is_swapped(pcap_t *p);

/**
 * @brief 获取pcap句柄的主版本号
 * 
 * @param p pcap句柄的指针
 * @return 主版本号
 */
PCAP_AVAILABLE_0_4
PCAP_API int pcap_major_version(pcap_t *p);

/**
 * @brief 获取pcap句柄的次版本号
 * 
 * @param p pcap句柄的指针
 * @return 次版本号
 */
PCAP_AVAILABLE_0_4
PCAP_API int pcap_minor_version(pcap_t *p);

/**
 * @brief 获取pcap句柄的缓冲区大小
 * 
 * @param p pcap句柄的指针
 * @return 缓冲区大小
 */
PCAP_AVAILABLE_1_9
PCAP_API int pcap_bufsize(pcap_t *p);

/**
 * @brief 获取pcap句柄关联的文件
 * 
 * @param p pcap句柄的指针
 * @return 关联的文件指针
 */
PCAP_AVAILABLE_0_4
PCAP_API FILE *pcap_file(pcap_t *p);

#ifdef _WIN32
/*
 * 这可能不应该保留在 WinPcap 中；大多数如果不是全部的 UN*X 代码在 Windows 上不会工作。我们弃用它；如果有人真的需要访问与 pcap_t 关联的任何 HANDLE（不能保证有一个），我们可以添加一个仅适用于 Windows 的 pcap_handle() API，返回 HANDLE。
 */
PCAP_AVAILABLE_0_4
PCAP_DEPRECATED("如果需要，请求一个返回 HANDLE 的 'pcap_handle'")
PCAP_API int	pcap_fileno(pcap_t *);
#else /* _WIN32 */
PCAP_AVAILABLE_0_4
PCAP_API int	pcap_fileno(pcap_t *);
#endif /* _WIN32 */

#ifdef _WIN32
  PCAP_API int	pcap_wsockinit(void);
#endif

PCAP_AVAILABLE_0_4
PCAP_API pcap_dumper_t *pcap_dump_open(pcap_t *, const char *);

/**
 * @file pcap_dump.h
 * @brief PCAP dump file handling functions
 */

#ifdef _WIN32
/**
 * @brief 打开一个用于写入的pcap dump文件，使用文件流
 * 
 * @param p pcap句柄的指针
 * @param f 要写入的文件流
 * @return 指向pcap dumper结构的指针
 */
PCAP_AVAILABLE_0_9
PCAP_API pcap_dumper_t *pcap_dump_hopen(pcap_t *p, intptr_t f);

#ifndef BUILDING_PCAP
/**
 * @brief 打开一个用于写入的pcap dump文件，使用文件指针
 * 
 * 此宏用于使用文件指针打开一个用于写入的pcap dump文件。
 * 
 * @param p pcap句柄的指针
 * @param f 要写入的文件指针
 * @return 指向pcap dumper结构的指针
 */
#define pcap_dump_fopen(p, f) \
    pcap_dump_hopen(p, _get_osfhandle(_fileno(f)))
#endif
#else /*_WIN32*/
/**
 * @brief 打开一个用于写入的pcap dump文件，使用FILE流
 * 
 * @param p pcap句柄的指针
 * @param fp 要写入的FILE流
 * @return 指向pcap dumper结构的指针
 */
PCAP_AVAILABLE_0_9
PCAP_API pcap_dumper_t *pcap_dump_fopen(pcap_t *p, FILE *fp);
#endif /*_WIN32*/

/**
 * @brief 打开一个用于追加写入的pcap dump文件
 * 
 * @param p pcap句柄的指针
 * @param filename 要追加写入的文件名
 * @return 指向pcap dumper结构的指针
 */
PCAP_AVAILABLE_1_7
PCAP_API pcap_dumper_t *pcap_dump_open_append(pcap_t *p, const char *filename);

/**
 * @brief 获取与pcap dumper关联的FILE流
 * 
 * @param dumper pcap dumper结构的指针
 * @return 与pcap dumper关联的FILE流
 */
PCAP_AVAILABLE_0_8
PCAP_API FILE *pcap_dump_file(pcap_dumper_t *dumper);

/**
 * @brief 获取pcap dump文件中的当前文件位置
 * 
 * @param dumper pcap dumper结构的指针
 * @return pcap dump文件中的当前文件位置
 */
PCAP_AVAILABLE_0_9
PCAP_API long pcap_dump_ftell(pcap_dumper_t *dumper);

/**
 * @brief 获取pcap dump文件中的当前文件位置（64位）
 * 
 * @param dumper pcap dumper结构的指针
 * @return pcap dump文件中的当前文件位置（64位）
 */
PCAP_AVAILABLE_1_9
PCAP_API int64_t pcap_dump_ftell64(pcap_dumper_t *dumper);

/**
 * @brief 刷新pcap dumper的缓冲区，将数据写入文件
 * 
 * @param dumper pcap dumper结构的指针
 * @return 成功刷新缓冲区时返回0，否则返回-1
 */
PCAP_AVAILABLE_0_8
PCAP_API int pcap_dump_flush(pcap_dumper_t *dumper);

/**
 * @brief 关闭pcap dumper，并释放相关资源
 * 
 * @param dumper pcap dumper结构的指针
 */
PCAP_AVAILABLE_0_4
PCAP_API void pcap_dump_close(pcap_dumper_t *dumper);

/**
 * @brief 将捕获到的数据包写入pcap dump文件
 * 
 * @param dumper pcap dumper结构的指针
 * @param header 指向捕获到的数据包头部的指针
 * @param packet 指向捕获到的数据包的指针
 */
PCAP_AVAILABLE_0_4
PCAP_API void pcap_dump(u_char *dumper, const struct pcap_pkthdr *header, const u_char *packet);

/**
 * @brief 查找所有可用的网络设备
 * 
 * @param alldevs 用于存储网络设备列表的指针
 * @param errbuf 存储错误消息的缓冲区
 * @return 成功查找到网络设备时返回0，否则返回-1
 */
PCAP_AVAILABLE_0_7
PCAP_API int pcap_findalldevs(pcap_if_t **alldevs, char *errbuf);

/**
 * @brief 释放由pcap_findalldevs函数分配的内存
 * 
 * @param alldevs 网络设备列表的指针
 */
PCAP_AVAILABLE_0_7
PCAP_API void pcap_freealldevs(pcap_if_t *alldevs);

/**
 * @brief 获取当前libpcap库的版本字符串
 * 
 * @return 当前libpcap库的版本字符串
 */
PCAP_AVAILABLE_0_8
PCAP_API const char *pcap_lib_version(void);


#if defined(_WIN32)

  /*
   * Win32 定义
   */

  /*!
    \brief 一个将发送到网络的原始数据包队列，将使用 pcap_sendqueue_transmit() 发送。
  */
  struct pcap_send_queue
  {
	u_int maxlen;	/* 队列的最大大小，以字节为单位。此变量包含缓冲区字段的大小。 */
	u_int len;	/* 队列的当前大小，以字节为单位。 */
	char *buffer;	/* 包含要发送的数据包的缓冲区。 */
  };

  typedef struct pcap_send_queue pcap_send_queue;

  /*!
    \brief 此 typedef 是 pcap_get_airpcap_handle() 函数的支持
  */
  #if !defined(AIRPCAP_HANDLE__EAE405F5_0171_9592_B3C2_C19EC426AD34__DEFINED_)
    #define AIRPCAP_HANDLE__EAE405F5_0171_9592_B3C2_C19EC426AD34__DEFINED_
    typedef struct _AirpcapHandle *PAirpcapHandle;
  #endif

  PCAP_API int pcap_setbuff(pcap_t *p, int dim);
  PCAP_API int pcap_setmode(pcap_t *p, int mode);
  PCAP_API int pcap_setmintocopy(pcap_t *p, int size);

  PCAP_API HANDLE pcap_getevent(pcap_t *p);

/**
 * @brief 获取指定OID的请求
 * 
 * @param p 捕获会话句柄
 * @param oid OID值
 * @param data 存放请求数据的缓冲区
 * @param len 缓冲区长度
 * @return 返回0表示成功，-1表示失败
 */
PCAP_AVAILABLE_1_8
PCAP_API int pcap_oid_get_request(pcap_t *p, bpf_u_int32 oid, void *data, size_t *len);

/**
 * @brief 设置指定OID的请求
 * 
 * @param p 捕获会话句柄
 * @param oid OID值
 * @param data 存放请求数据的缓冲区
 * @param len 缓冲区长度
 * @return 返回0表示成功，-1表示失败
 */
PCAP_AVAILABLE_1_8
PCAP_API int pcap_oid_set_request(pcap_t *p, bpf_u_int32 oid, const void *data, size_t len);

/**
 * @brief 分配一个发送队列
 * 
 * @param memsize 队列的最大大小（以字节为单位）
 * @return 返回指向发送队列的指针，如果失败则返回NULL
 */
PCAP_API pcap_send_queue* pcap_sendqueue_alloc(u_int memsize);

/**
 * @brief 销毁发送队列
 * 
 * @param queue 指向发送队列的指针
 */
PCAP_API void pcap_sendqueue_destroy(pcap_send_queue* queue);

/**
 * @brief 将数据包放入发送队列
 * 
 * @param queue 指向发送队列的指针
 * @param pkt_header 数据包的头部信息
 * @param pkt_data 数据包的内容
 * @return 返回0表示成功，-1表示失败
 */
PCAP_API int pcap_sendqueue_queue(pcap_send_queue* queue, const struct pcap_pkthdr *pkt_header, const u_char *pkt_data);

/**
 * @brief 传输发送队列中的数据包
 * 
 * @param p 捕获会话句柄
 * @param queue 指向发送队列的指针
 * @param sync 如果为1，则进行同步传输，否则进行异步传输
 * @return 返回实际传输的数据包数
 */
PCAP_API u_int pcap_sendqueue_transmit(pcap_t *p, pcap_send_queue* queue, int sync);

/**
 * @brief 获取扩展的捕获统计信息
 * 
 * @param p 捕获会话句柄
 * @param pcap_stat_size 指向整数的指针，用于存储捕获统计信息的长度
 * @return 返回指向捕获统计信息的指针，如果失败则返回NULL
 */
PCAP_API struct pcap_stat *pcap_stats_ex(pcap_t *p, int *pcap_stat_size);

/**
 * @brief 设置用户缓冲区大小
 * 
 * @param p 捕获会话句柄
 * @param size 缓冲区大小
 * @return 返回0表示成功，-1表示失败
 */
PCAP_API int pcap_setuserbuffer(pcap_t *p, int size);

/**
 * @brief 实时捕获并将数据包保存到文件
 * 
 * @param p 捕获会话句柄
 * @param filename 要保存到的文件名
 * @param maxsize 最大文件大小（以字节为单位）
 * @param maxpacks 最大数据包数
 * @return 返回0表示成功，-1表示失败
 */
PCAP_API int pcap_live_dump(pcap_t *p, char *filename, int maxsize, int maxpacks);

/**
 * @brief 结束实时捕获并保存数据包到文件
 * 
 * @param p 捕获会话句柄
 * @param sync 如果为1，则进行同步结束，否则进行异步结束
 * @return 返回0表示成功，-1表示失败
 */
PCAP_API int pcap_live_dump_ended(pcap_t *p, int sync);

/**
 * @brief 启动 OEM 模式
 * 
 * @param err_str 错误信息字符串
 * @param flags 标志
 * @return 返回0表示成功，-1表示失败
 */
PCAP_API int pcap_start_oem(char* err_str, int flags);


  PCAP_API PAirpcapHandle pcap_get_airpcap_handle(pcap_t *p);

  #define MODE_CAPT 0
  #define MODE_STAT 1
  #define MODE_MON 2

#elif defined(MSDOS)

  /*
   * MS-DOS 定义
   */

/**
 * @brief 获取扩展的捕获统计信息
 * 
 * @param p 捕获会话句柄
 * @param pcap_stat_ex 指向扩展捕获统计信息结构的指针
 * @return 返回0表示成功，-1表示失败
 */
PCAP_API int pcap_stats_ex(pcap_t *p, struct pcap_stat_ex *pcap_stat_ex);

/**
 * @brief 设置等待函数
 * 
 * @param p 捕获会话句柄
 * @param yield 指向等待函数的指针
 * @param wait 等待标志
 */
PCAP_API void pcap_set_wait(pcap_t *p, void (*yield)(void), int wait);

/**
 * @brief 获取 MAC 数据包数
 * 
 * @return 返回 MAC 数据包数
 */
PCAP_API u_long pcap_mac_packets(void);


#else /* UN*X */

  /*
   * UN*X 定义
   */

  PCAP_AVAILABLE_0_8
  PCAP_API int	pcap_get_selectable_fd(pcap_t *);

  PCAP_AVAILABLE_1_9
  PCAP_API const struct timeval *pcap_get_required_select_timeout(pcap_t *);

#endif /* _WIN32/MSDOS/UN*X */

/*
 * 远程捕获定义。
 *
 * 仅当 libpcap 已配置为包括远程捕获支持时，这些例程才存在。
 */

/*
 * 地址、端口、接口名称的最大缓冲区大小。
 *
 * 如果适配器名称等大于此值，则会被截断。
 * 用户不使用此值；然而，必须意识到主机名/接口名称超过此值将被截断。
 */
#define PCAP_BUF_SIZE 1024


/*
 * 传递给 pcap_open() 的输入源类型。
 */
#define PCAP_SRC_FILE		2	/* 本地保存文件 */
#define PCAP_SRC_IFLOCAL	3	/* 本地网络接口 */
#define PCAP_SRC_IFREMOTE	4	/* 远程主机上的接口，使用 RPCAP */


/*
 * pcap_open() 允许的格式如下：
 * - file://path_and_filename [打开本地文件]
 * - rpcap://devicename [打开本地主机上选择的设备，不使用 RPCAP 协议]
 * - rpcap://host/devicename [打开远程主机上可用的设备]
 * - rpcap://host:port/devicename [打开远程主机上可用的设备，使用非标准端口进行 RPCAP]
 * - adaptername [打开本地适配器；为了兼容性而保留，但强烈不建议使用]
 * - (NULL) [打开第一个本地适配器；为了兼容性而保留，但强烈不建议使用]
 *
 * pcap_findalldevs_ex() 允许的格式如下：
 * - file://folder/ [列出给定文件夹中的所有文件]
 * - rpcap:// [列出所有本地适配器]
 * - rpcap://host:port/ [列出远程主机上可用的设备]
 *
 * 在上述所有格式中，"rpcaps://" 可以替换为 "rpcap://" 以启用 SSL（如果已编译）。
 *
 * 关于 'host' 和 'port' 参数，它们可以是数字或文字。由于完全支持 IPv6，以下是允许的格式：
 *
 * - host（文字）：例如 host.foo.bar
 * - host（数字 IPv4）：例如 10.11.12.13
 * - host（数字 IPv4，IPv6 格式）：例如 [10.11.12.13]
 * - host（数字 IPv6）：例如 [1:2:3::4]
 * - port：可以是数字（例如 '80'）或文字（例如 'http'）
 *
 * 这里是一些允许的示例：
 * - rpcap://host.foo.bar/devicename [全部文字，无端口号]
 * - rpcap://host.foo.bar:1234/devicename [全部文字，带端口号]
 * - rpcap://10.11.12.13/devicename [IPv4 数字，无端口号]
 * - rpcap://10.11.12.13:1234/devicename [IPv4 数字，带端口号]
 * - rpcap://[10.11.12.13]:1234/devicename [IPv4 数字，IPv6 格式，带端口号]
 * - rpcap://[1:2:3::4]/devicename [IPv6 数字，无端口号]
 * - rpcap://[1:2:3::4]:1234/devicename [IPv6 数字，带端口号]
 * - rpcap://[1:2:3::4]:http/devicename [IPv6 数字，带文字端口号]
 */

/*
 * 捕获源的 URL 方案。
 */
/*
 * 此字符串表示用户想要从本地文件打开捕获。
 */
#define PCAP_SRC_FILE_STRING "file://"
/*
 * 此字符串表示用户想要从网络接口打开捕获。此字符串不一定涉及使用 RPCAP 协议。如果所需的接口位于本地主机上，则不涉及 RPCAP 协议，而是使用本地功能。
 */
#define PCAP_SRC_IF_STRING "rpcap://"

/*
 * 传递给 pcap_open() 的标志。
 */

/*
 * 指定是否使用混杂模式。
 */
#define PCAP_OPENFLAG_PROMISCUOUS		0x00000001

/*
 * 指定对于 RPCAP 捕获，数据传输（在远程捕获的情况下）是否必须使用 UDP 协议。
 *
 * 如果值为 '1'，表示要使用 UDP 数据连接；如果值为 '0'，表示要使用 TCP 数据连接；控制连接始终基于 TCP。
 * UDP 连接更轻量，但不能保证所有捕获的数据包都到达客户端工作站。此外，在网络拥塞的情况下可能会有问题。
 * 如果源不是远程接口，则此标志无意义。在这种情况下，它会被简单地忽略。
 */
#define PCAP_OPENFLAG_DATATX_UDP		0x00000002

/*
 * 指定远程探测器是否捕获自己生成的流量。
 *
 * 如果远程探测器使用相同的接口捕获流量并将数据发送回调用者，则捕获的流量包括 RPCAP 流量。如果打开了此标志，RPCAP 流量将从捕获中排除，因此返回给收集器的跟踪不包括此流量。
 *
 * 对本地接口或保存文件无效。
 */
#define PCAP_OPENFLAG_NOCAPTURE_RPCAP		0x00000004

/*
 * 指定本地适配器是否捕获自己生成的流量。
 *
 * 此标志告诉底层捕获驱动程序丢弃其自己发送的数据包。这在构建桥接等应用程序时很有用，这些应用程序应忽略它们刚刚发送的流量。
 *
 * 仅在 Windows 上受支持。
 */
#define PCAP_OPENFLAG_NOCAPTURE_LOCAL		0x00000008

/*
 * 此标志配置适配器以获得最大响应速度。
 *
 * 在 nbytes 的值很大的情况下，WinPcap 在将数据复制到用户之前等待几个数据包的到达。这保证了较少的系统调用，即较低的处理器使用率，即更好的性能，这对于嗅探器之类的应用程序很有用。如果用户设置了 PCAP_OPENFLAG_MAX_RESPONSIVENESS 标志，捕获驱动程序将在应用程序准备接收数据时立即复制数据包。这对实时应用程序（例如桥接）是有益的。
 *
 * 使用 pcap_create()/pcap_activate() 相当于"immediate mode"。
 */
#define PCAP_OPENFLAG_MAX_RESPONSIVENESS	0x00000010

/*
 * 远程身份验证方法。
 * 这些用于 pcap_rmtauth 结构的 'type' 成员。
 */

/*
 * 空身份验证。
 *
 * 'NULL' 身份验证必须等于 '零'，以便旧应用程序可以将 pcap_rmtauth 结构的每个字段都设置为零，这样就有效了。
 */
#define RPCAP_RMTAUTH_NULL 0
/*
 * 用户名/密码身份验证。
 *
 * 使用此类型的身份验证，RPCAP 协议将使用提供的用户名/密码在远程机器上对用户进行身份验证。如果身份验证成功（并且用户有权打开网络设备），则 RPCAP 连接将继续；否则将被丢弃。
 *
 * *******注意********：除非使用 TLS，否则用户名和密码以明文发送到捕获服务器。在您不完全控制的网络上不要使用此功能（即在 rpcap:// 而不是 rpcaps:// 上）！（并且在您对“完全控制”的定义中要非常小心！）
 */
#define RPCAP_RMTAUTH_PWD 1

/*
 * 此结构保留了在远程机器上对用户进行身份验证所需的信息。
 *
 * 远程机器可以根据提供的信息授予或拒绝访问。
 * 如果需要 NULL 身份验证，'username' 和 'password' 都可以是 NULL 指针。
 *
 * 如果源不是远程接口，则此结构无意义；在这种情况下，需要此结构的函数也可以接受 NULL 指针。
 */
struct pcap_rmtauth
{
	/*
	 * \brief 所需身份验证的类型。
	 *
	 * 为了提供最大的灵活性，我们支持不同类型的身份验证，基于此 'type' 变量的值。当前支持的身份验证方法定义在
	 * \link remote_auth_methods Remote Authentication Methods Section\endlink 中。
	 */
	int type;
	/*
	 * \brief 包含用于在远程机器上进行身份验证的用户名的以零结尾的字符串。
	 *
	 * 在 RPCAP_RMTAUTH_NULL 身份验证的情况下，此字段无意义，可以是 NULL。
	 */
	char *username;
	/*
	 * \brief 包含用于在远程机器上进行身份验证的密码的以零结尾的字符串。
	 *
	 * 在 RPCAP_RMTAUTH_NULL 身份验证的情况下，此字段无意义，可以是 NULL。
	 */
	char *password;
};

/*
 * 这个函数可以打开保存文件、本地设备或远程运行 RPCAP 服务器的设备。
 *
 * 对于打开保存文件，可以使用 pcap_open_offline 系列函数，它们同样有效；使用它们的代码将在更多平台上工作，而不仅限于使用 pcap_open() 打开保存文件的代码。
 *
 * 对于打开本地设备，可以使用 pcap_open_live()；它支持大多数 pcap_open() 支持的功能，并且使用它的代码将在更多平台上工作，而不仅限于使用 pcap_open() 的代码。
 还可以使用 pcap_create() 和 pcap_activate()；它们支持 pcap_open() 支持的所有功能，除了仅适用于 Windows 的 PCAP_OPENFLAG_NOCAPTURE_LOCAL，并支持其他功能。
 *
 * 对于打开远程捕获，目前只有 pcap_open() 是可用的 API。
 */
PCAP_AVAILABLE_1_9
PCAP_API pcap_t	*pcap_open(const char *source, int snaplen, int flags,
	    int read_timeout, struct pcap_rmtauth *auth, char *errbuf);

PCAP_AVAILABLE_1_9
PCAP_API int	pcap_createsrcstr(char *source, int type, const char *host,
	    const char *port, const char *name, char *errbuf);

PCAP_AVAILABLE_1_9
PCAP_API int	pcap_parsesrcstr(const char *source, int *type, char *host,
	    char *port, char *name, char *errbuf);

/*
 * 此例程可以扫描目录以获取保存文件、列出本地捕获设备，或列出运行 RPCAP 服务器的远程机器上的捕获设备。
 *
 * 用于扫描保存文件时，它可以在 UN*X 系统和 Windows 系统上使用；对于它看到的每个目录条目，它尝试使用 pcap_open_offline() 打开文件作为保存文件，并且仅在打开成功时将其包括在文件列表中，因此它会过滤掉用户没有读取权限的文件，以及不是 libpcap 可读取的有效保存文件。
 *
 * 用于列出本地捕获设备时，它只是 pcap_findalldevs() 的一个包装；使用 pcap_findalldevs() 的代码将在更多平台上工作，而不仅限于使用 pcap_findalldevs_ex() 的代码。
 *
 * 用于列出远程捕获设备时，pcap_findalldevs_ex() 目前是唯一可用的 API。
 */
PCAP_AVAILABLE_1_9
PCAP_API int	pcap_findalldevs_ex(const char *source,
	    struct pcap_rmtauth *auth, pcap_if_t **alldevs, char *errbuf);

/*
 * 取样方法。
 *
 * 这些允许 pcap_loop()、pcap_dispatch()、pcap_next() 和 pcap_next_ex() 仅查看一部分数据包，而不是所有数据包。
 *
 * 目前，它们仅在 Windows 本地捕获上工作。
 */

/*
 * 指定当前捕获不进行取样。
 *
 * 在这种情况下，不应用任何取样算法。
 */
#define PCAP_SAMP_NOSAMP	0

/*
 * 指定每 N 个数据包中只返回一个数据包。
 *
 * 在这种情况下，'pcap_samp' 结构的 'value' 字段表示要丢弃的数据包数（减去 1），然后才会接受一个数据包。
 * 换句话说，如果 'value = 10'，则第一个数据包返回给调用者，接下来的 9 个数据包将被丢弃。
 */
#define PCAP_SAMP_1_EVERY_N	1

/*
 * 指定我们必须每 N 毫秒返回 1 个数据包。
 *
 * 在这种情况下，'pcap_samp' 结构的 'value' 字段表示在毫秒中的'等待时间'，然后才会接受一个数据包。
 * 换句话说，如果 'value = 10'，则第一个数据包返回给调用者；下一个返回的数据包将是在经过 10 毫秒后到达的第一个数据包。
 */
#define PCAP_SAMP_FIRST_AFTER_N_MS 2

/*
 * 此结构定义了与取样相关的信息。
 *
 * 如果请求了取样，捕获设备应该仅读取来自源的一部分数据包。返回的数据包取决于取样参数。
 *
 * 警告：取样过程应用于过滤过程*之后*。换句话说，首先进行数据包过滤，然后取样过程选择'过滤'数据包的子集，并将它们返回给调用者。
 */
struct pcap_samp
{
	/*
	 * 用于取样的方法；参见上文。
	 */
	int method;

	/*
	 * 此值取决于定义的取样方法。
	 * 其含义请参见上文。
	 */
	int value;
};

/*
 * 新函数。
 */
PCAP_AVAILABLE_1_9
PCAP_API struct pcap_samp *pcap_setsampling(pcap_t *p);

/*
 * RPCAP 主动模式。
 */

/* 主机名的最大长度（RPCAP 主动模式所需） */
#define RPCAP_HOSTLIST_SIZE 1024

/**
 * @brief 在远程捕获模式下接受来自远程主机的连接。
 *
 * @param address           本地地址。
 * @param port              本地端口。
 * @param hostlist          允许连接的主机列表。
 * @param connectinghost    连接的主机。
 * @param auth              远程认证信息。
 * @param errbuf            用于存储错误信息的缓冲区。
 *
 * @return 返回一个 SOCKET，表示远程捕获的连接，或者返回无效的 SOCKET。
 */
PCAP_API SOCKET pcap_remoteact_accept(const char *address, const char *port,
                                      const char *hostlist, char *connectinghost,
                                      struct pcap_rmtauth *auth, char *errbuf);

/**
 * @brief 在远程捕获模式下接受来自远程主机的连接，支持 SSL。
 *
 * @param address           本地地址。
 * @param port              本地端口。
 * @param hostlist          允许连接的主机列表。
 * @param connectinghost    连接的主机。
 * @param auth              远程认证信息。
 * @param uses_ssl          是否使用 SSL。
 * @param errbuf            用于存储错误信息的缓冲区。
 *
 * @return 返回一个 SOCKET，表示远程捕获的连接，或者返回无效的 SOCKET。
 */
PCAP_API SOCKET pcap_remoteact_accept_ex(const char *address, const char *port,
                                         const char *hostlist, char *connectinghost,
                                         struct pcap_rmtauth *auth, int uses_ssl, char *errbuf);

/**
 * @brief 列出远程捕获主机列表。
 *
 * @param hostlist  存储主机列表的缓冲区。
 * @param sep       分隔符。
 * @param size      缓冲区大小。
 * @param errbuf    用于存储错误信息的缓冲区。
 *
 * @return 成功时返回主机列表的长度，失败时返回 -1。
 */
PCAP_API int pcap_remoteact_list(char *hostlist, char sep, int size, char *errbuf);

/**
 * @brief 关闭远程捕获连接。
 *
 * @param host      要关闭连接的主机名。
 * @param errbuf    用于存储错误信息的缓冲区。
 *
 * @return 成功时返回 0，失败时返回 -1。
 */
PCAP_API int pcap_remoteact_close(const char *host, char *errbuf);

/**
 * @brief 清理远程捕获相关资源。
 */
PCAP_API void pcap_remoteact_cleanup(void);


#ifdef __cplusplus
}
#endif

#endif /* lib_pcap_pcap_h */

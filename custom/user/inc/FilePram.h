#ifndef __FILEPRAM_H__
#define __FILEPRAM_H__

//#define FRAM_BufferWrite(WriteAddr, pBuffer, NumBytesToWrite) 0// E2prom_WriteBytes(WriteAddr, pBuffer, NumBytesToWrite)

//#define FRAM_BufferRead(pBuffer,NumBytesToRead,ReadAddr)  0// E2prom_ReadBytes(ReadAddr,pBuffer,NumBytesToRead)

#define PRAM_FILE_NAME    "device_pram.txt"


#define PARAMTER_MAX_LEN                   25//取前面定义的参数长度最大值

/***********参数ID定义，顺序按照PARAMETER_STRUCT排列**********************/
#define E2_TERMINAL_HEARTBEAT_ID 			0x0001//参数ID 0x0001,终端心跳发送间隔//单位秒//默认值60
#define E2_MAIN_SERVER_APN_ID 				0x0010//参数ID 0x0010,主服务器APN
#define E2_MAIN_SERVER_DIAL_NAME_ID 		0x0011//参数ID 0x0011,主服务器无线通信拨号用户名
#define E2_MAIN_SERVER_DIAL_PASSWORD_ID 	0x0012//参数ID 0x0012,主服务器无线通信拨号密码
#define E2_MAIN_SERVER_IP_ID 				0x0013//参数ID 0x0013,主服务器地址//IP或域名
#define E2_BACKUP_SERVER_APN_ID 			0x0014//参数ID 0x0014,备份服务器APN//无线通信拨号访问点
#define E2_BACKUP_SERVER_DIAL_NAME_ID 		0x0015//参数ID 0x0015,备份服务器无线通信拨号用户名
#define E2_BACKUP_SERVER_DIAL_PASSWORD_ID 	0x0016//参数ID 0x0016,备份服务器无线通信拨号密码
#define E2_BACKUP_SERVER_IP_ID 				0x0017//参数ID 0x0017,备份服务器地址//IP或域名
#define E2_MAIN_SERVER_TCP_PORT_ID 			0x0018//参数ID 0x0018,主服务器TCP端口
#define E2_SLEEP_REPORT_TIME_ID 			0x0027//参数ID 0x0027,休眠时汇报时间间隔//单位为秒//默认值为240
#define E2_EMERGENCY_REPORT_TIME_ID 		0x0028//参数ID 0x0028,紧急报警时汇报时间间隔//单位为秒//默认值为10
#define	E2_ACCON_REPORT_TIME_ID				0x0029//参数ID 0x0029,缺省时间汇报间隔,单位为秒,默认为30
#define E2_CORNER_REPORT_ID 				0x0030//参数ID 0x0030,拐点补传角度,单位度/秒//默认值为30
#define E2_MONITOR_SERVER_SMS_PHONE_ID      0x0043//监控平台SMS电话号码
#define E2_SMS_TEXT_ALARM_PHONE_ID 			0x0044//SMS文本报警电话号码
#define E2_ALARM_MASK_ID 					0x0050//参数ID 0x0050,报警屏蔽字//bit9,11,12默认屏蔽,LCD屏故障,TTS故障,IC卡模块故障默认为屏蔽
#define E2_MAX_SPEED_ID 					0x0055//参数ID 0x0055,最高速度,单位km/h,DWORD,默认值130,九通需求为100
#define E2_OVER_SPEED_KEEP_TIME_ID 			0x0056//参数ID 0x0056,超速持续时间,单位为秒,DWORD,默认为10秒,九通需求为立马上报
#define	E2_SET_CRASH_ALARM_ID				0x005D//参数ID 0x005D,碰撞报警参数设置,碰撞加速度默认设置为4g
#define	E2_SET_ROLLOVER_ALARM_ID	        0x005E//参数ID 0x005E,侧翻报警参数设置：侧翻角度，单位1 度，默认为45 度
#define	E2_CAR_TOTAL_MILE_ID				0x0080//参数ID 0x0080，车辆里程表读数，0.1km
#define E2_CAR_PROVINCE_ID 					0x0081//参数ID 0x0081,车辆所在的省域ID
#define E2_CAR_CITY_ID 						0x0082//参数ID 0x0082,车辆所在的市域ID
#define E2_CAR_PLATE_NUM_ID 				0x0083//参数ID 0x0083,公安交通管理部门颁发的机动车号牌
#define E2_CAR_PLATE_COLOR_ID 				0x0084//车参数ID 0x0084,车牌颜色//按照JT/T415-2006的5.4.12,1:蓝色,2:黄色,3:黑色,4:白色,9:其它

#define	E2_GPS_SET_MODE_ID					0x0090//参数ID 0x0090,GNSS 定位模式，默认为0x02 北斗定位

#define E2_PRAM_BASE_CUSTOM_ID           0xF200
#define E2_PRAM_BASE_CUSTOM_ID1          0x0001F000

#define E2_LOGINGPRS_PASSWORD_ID		(E2_PRAM_BASE_CUSTOM_ID+0x00)//参数ID 0xF200,鉴权码
#define E2_MANUFACTURE_ID				(E2_PRAM_BASE_CUSTOM_ID+0x05)//参数ID 0xF205，自定义，制造商ID，5字节	
#define E2_DEVICE_ID					(E2_PRAM_BASE_CUSTOM_ID+0x06)//参数ID 0xF206，自定义，厂商终端ID，7字节
#define E2_DEVICE_PHONE_ID				(E2_PRAM_BASE_CUSTOM_ID+0x07)//参数ID 0xF207，自定义，终端手机号，6字节，
#define E2_CAR_VIN_ID 					(E2_PRAM_BASE_CUSTOM_ID+0x08)//参数ID 0xF208，自定义，车辆识别码(VIN)，17字节，ASCII码，
#define	E2_FIRMWARE_VERSION_ID			(E2_PRAM_BASE_CUSTOM_ID+0x0C)//参数ID 0xF20C，自定义，固件版本号
#define E2_DEFINE_ALARM_MASK_WORD_ID	(E2_PRAM_BASE_CUSTOM_ID+0x43)//参数ID 0xF243，自定义，自定义报警屏蔽字，DWORD
#define E2_DEFINE_HARD_VER_ID			(E2_PRAM_BASE_CUSTOM_ID1+0x04)//参数ID 0x0001F004，自定义，自定义硬件版本号，DWORD
#define E2_SMS_ALARM_PHONE_NUM_ID		(E2_PRAM_BASE_CUSTOM_ID1+0x05)//参数ID 0x0001F005，自定义，自定义用户短信报警电话，DWORD
#define E2_SMS_OPERATE_PWD_ID			(E2_PRAM_BASE_CUSTOM_ID1+0x06)//参数ID 0x0001F006，自定义，自定义用户短信操作密码，DWORD
#define E2_LOW_VOL_THRESHOLD_ID			(E2_PRAM_BASE_CUSTOM_ID1+0x0f)//参数ID 0x0001F00F，自定义，自定义低电压阈值，WORD
#define E2_DEVICE_BACKUP_ID             (E2_PRAM_BASE_CUSTOM_ID+0x2a)//参数ID 0xF22a，自定义，终端ID号备份
#define	E2_SIM_ICCID_NUMBER_ID          (E2_PRAM_BASE_CUSTOM_ID+0x7F)//参数ID 0xF27F, 自定义，SIM卡的ICCID号码,BCD[10],只读

/*****************ID对应的长度***************************/
#define E2_TERMINAL_HEARTBEAT_LEN 		4//参数ID 0x0001,终端心跳发送间隔//单位秒//默认值60
#define E2_MAIN_SERVER_APN_LEN 			25//参数ID 0x0010,主服务器APN
#define E2_MAIN_SERVER_DIAL_NAME_LEN 		25//参数ID 0x0011,主服务器无线通信拨号用户名
#define E2_MAIN_SERVER_DIAL_PASSWORD_LEN 	25//参数ID 0x0012,主服务器无线通信拨号密码
#define E2_MAIN_SERVER_IP_LEN 			25//参数ID 0x0013,主服务器地址//IP或域名
#define E2_BACKUP_SERVER_APN_LEN 		25//参数ID 0x0014,备份服务器APN//无线通信拨号访问点
#define E2_BACKUP_SERVER_DIAL_NAME_LEN 		25//参数ID 0x0015,备份服务器无线通信拨号用户名
#define E2_BACKUP_SERVER_DIAL_PASSWORD_LEN 	25//参数ID 0x0016,备份服务器无线通信拨号密码
#define E2_BACKUP_SERVER_IP_LEN 		25//参数ID 0x0017,备份服务器地址//IP或域名
#define E2_MAIN_SERVER_TCP_PORT_LEN 		4//参数ID 0x0018,主服务器TCP端口
#define E2_SLEEP_REPORT_TIME_LEN 		4//参数ID 0x0027,休眠时汇报时间间隔//单位为秒//默认值为240
#define E2_EMERGENCY_REPORT_TIME_LEN		4//参数ID0x0028，紧急报警时汇报时间间隔，单位为秒，默认值为10
#define	E2_ACCON_REPORT_TIME_LEN		4//参数ID 0x0029,缺省时间汇报间隔,单位为秒,默认为30
#define E2_CORNER_REPORT_LEN 			4//参数ID 0x0030,拐点补传角度,单位度/秒//默认值为30
#define E2_MONITOR_SERVER_SMS_PHONE_LEN		20	//参数ID0x0043，监控平台SMS电话号码
#define E2_SMS_TEXT_ALARM_PHONE_LEN     	20  //参数ID0x0044，SMS文本报警电话号码
#define E2_ALARM_MASK_LEN 			4//参数ID 0x0050,报警屏蔽字//bit9,11,12默认屏蔽,LCD屏故障,TTS故障,IC卡模块故障默认为屏蔽
#define E2_MAX_SPEED_LEN 			4//参数ID 0x0055,最高速度,单位km/h,DWORD,默认值130,九通需求为100
#define E2_OVER_SPEED_KEEP_TIME_LEN 		4//参数ID 0x0056,超速持续时间,单位为秒,DWORD,默认为10秒,九通需求为立马上报
#define	E2_SET_CRASH_ALARM_LEN			2//参数ID 0x005D,碰撞报警参数设置,碰撞加速度默认设置为4g
#define	E2_SET_ROLLOVER_ALARM_LEN	        2//参数ID 0x005E,侧翻报警参数设置：侧翻角度，单位1 度，默认为45 度
#define E2_CAR_TOTAL_MILE_LEN			4	//参数ID0x0080，车辆里程表读数，0.1km
#define E2_CAR_PROVINCE_LEN 			2//参数ID 0x0081,车辆所在的省域ID
#define E2_CAR_CITY_LEN 			2//参数ID 0x0082,车辆所在的市域ID
#define E2_CAR_PLATE_NUM_LEN 			25//参数ID 0x0083,公安交通管理部门颁发的机动车号牌
#define E2_CAR_PLATE_COLOR_LEN 			1//车参数ID 0x0084,牌颜色//按照JT/T415-2006的5.4.12,1:蓝色,2:黄色,3:黑色,4:白色,9:其它
#define	E2_GPS_SET_MODE_LEN			1//参数ID 0x0090,GNSS 定位模式，默认为0x02 北斗定位

#define E2_LOGINGPRS_PASSWORD_ID_LEN		25//参数ID 0xF200,鉴权码
#define E2_MANUFACTURE_ID_LEN			5//参数ID 0xF205，自定义，制造商ID，5字节	
#define E2_DEVICE_ID_LEN			7//参数ID 0xF206，自定义，厂商终端ID，7字节
#define E2_DEVICE_PHONE_ID_LEN			6//参数ID 0xF207，自定义，终端手机号，6字节，
#define	E2_CAR_VIN_ID_LEN				17//参数ID 0xF208车辆识别码(VIN)，17字节，ASCII码
#define	E2_FIRMWARE_VERSION_ID_LEN		5//参数ID 0xF20C，自定义，固件版本号
#define E2_DEVICE_BACKUP_ID_LEN			7//参数ID 0xF22a，自定义，厂商终端ID，7字节
#define E2_DEFINE_ALARM_MASK_WORD_ID_LEN	4//参数ID 0xF243，自定义，自定义报警屏蔽字，DWORD
#define E2_DEFINE_HARD_VER_ID_LEN			10//参数ID0x0001F0004，自定义，自定义硬件版本号，DWORD
#define E2_SMS_ALARM_PHONE_NUM_ID_LEN		20	//参数ID 0x0001F005，自定义，自定义用户短信报警电话�
#define E2_SMS_OPERATE_PWD_ID_LEN     		6  //参数ID 0x0001F006，自定义，自定义用户短信操作密码
#define E2_LOW_VOL_THRESHOLD_ID_LEN			2//参数ID 0x0001F00F，自定义，自定义低电压阈值，WORD

/////////自定义项最大ID为最后的ID加1,请根据最后一项人工修改/////////////////////////////////////
#define	E2_SELF_DEFINE_ID_MAX               (E2_DEFINE_ALARM_MASK_WORD_ID+1)//自定义项最大ID,注意：每次新增参数时需要将最后的参数ID放在这






#define FRAM_BASE_ADDR               0//前面一百字节当做铁电用
#define EEPROM_PRAM_BASE_ADDR           100//前面一百字节当做铁电用
//*****************************************************************************************
//*****************************************************************************************
//*****************************************************************************************
//*****************************************************************************************
//***************************************参数保存的文件位置*******************************
//*****************************************************************************************
//*****************************************************************************************
//*****************************************************************************************
#define E2_PRAM_START_ADDR                  EEPROM_PRAM_BASE_ADDR
#define E2_PRAM_END_ADDR                    E2_PRAM_START_ADDR+E2_SMS_OPERATE_PWD_ID_ADDR+E2_SMS_OPERATE_PWD_ID_LEN+4
//参数的起始地址

#define E2_TERMINAL_HEARTBEAT_ADDR          EEPROM_PRAM_BASE_ADDR//4字节		
//参数ID0x0001，终端心跳发送间隔，单位秒，默认值60

#define E2_MAIN_SERVER_APN_ADDR             (E2_TERMINAL_HEARTBEAT_ADDR+E2_TERMINAL_HEARTBEAT_LEN+1)//25字节		
//参数ID0x0010，主服务器APN

#define E2_MAIN_SERVER_DIAL_NAME_ADDR       (E2_MAIN_SERVER_APN_ADDR+E2_MAIN_SERVER_APN_LEN+2)		
//参数ID0x0011，主服务器无线通信拨号用户名

#define E2_MAIN_SERVER_DIAL_PASSWORD_ADDR   (E2_MAIN_SERVER_DIAL_NAME_ADDR+E2_MAIN_SERVER_DIAL_NAME_LEN+2)			
//参数ID0x0012，主服务器无线通信拨号密码

#define E2_MAIN_SERVER_IP_ADDR              (E2_MAIN_SERVER_DIAL_PASSWORD_ADDR+E2_MAIN_SERVER_DIAL_PASSWORD_LEN+2)	
//参数ID0x0013，主服务器地址，IP或域名

#define E2_BACKUP_SERVER_APN_ADDR		(E2_MAIN_SERVER_IP_ADDR+E2_MAIN_SERVER_IP_LEN+2)				
//参数ID0x0014，备份服务器APN，无线通信拨号访问点

#define E2_BACKUP_SERVER_DIAL_NAME_ADDR		(E2_BACKUP_SERVER_APN_ADDR+E2_BACKUP_SERVER_APN_LEN+2)	
//参数ID0x0015，备份服务器无线通信拨号用户名

#define E2_BACKUP_SERVER_DIAL_PASSWORD_ADDR	(E2_BACKUP_SERVER_DIAL_NAME_ADDR+E2_BACKUP_SERVER_DIAL_NAME_LEN+2)		
//参数ID0x0016，备份服务器无线通信拨号密码

#define E2_BACKUP_SERVER_IP_ADDR		(E2_BACKUP_SERVER_DIAL_PASSWORD_ADDR+E2_BACKUP_SERVER_DIAL_PASSWORD_LEN+2)			
//参数ID0x0017，备份服务器地址，IP或域名

#define E2_MAIN_SERVER_TCP_PORT_ADDR        (E2_BACKUP_SERVER_IP_ADDR+E2_BACKUP_SERVER_IP_LEN+2)			
//参数ID0x0018，主服务器TCP端口

#define E2_SLEEP_REPORT_TIME_ADDR           (E2_MAIN_SERVER_TCP_PORT_ADDR+E2_MAIN_SERVER_TCP_PORT_LEN+2)		
//参数ID0x0027，休眠时汇报时间间隔，单位为秒，默认值为60

#define E2_EMERGENCY_REPORT_TIME_ADR  (E2_SLEEP_REPORT_TIME_ADDR+E2_SLEEP_REPORT_TIME_LEN+1)
//参数D0x0028,紧急报警时汇报时间间隔//单位为秒//默认值为10

#define	E2_ACCON_REPORT_TIME_ADDR           (E2_EMERGENCY_REPORT_TIME_ADR+E2_EMERGENCY_REPORT_TIME_LEN+1)		
//E2_PRAM_BASE_CUSTOM_ID+0x0029缺省时间汇报间隔,单位为秒

#define E2_CORNER_REPORT_ADDR		(E2_ACCON_REPORT_TIME_ADDR+E2_ACCON_REPORT_TIME_LEN+1)	
//参数ID0x0030，拐点补传角度,单位度/秒，默认值为15

#define E2_MONITOR_SERVER_SMS_PHONE_ADDR    (E2_CORNER_REPORT_ADDR+E2_CORNER_REPORT_LEN+1) 
//参数ID0x0043，监控平台SMS电话号码

#define E2_SMS_TEXT_ALARM_PHONE_ADDR		(E2_MONITOR_SERVER_SMS_PHONE_ADDR+E2_MONITOR_SERVER_SMS_PHONE_LEN+2)	
//参数ID0x0044，SMS文本报警电话号码

#define E2_ALARM_MASK_ADDR					(E2_SMS_TEXT_ALARM_PHONE_ADDR+E2_SMS_TEXT_ALARM_PHONE_LEN+2)
//参数ID0x0050，报警屏蔽字，默认为全开

#define E2_MAX_SPEED_ADDR			(E2_ALARM_MASK_ADDR+E2_ALARM_MASK_LEN+1)				
//参数ID0x0055，最高速度

#define E2_OVER_SPEED_KEEP_TIME_ADDR		(E2_MAX_SPEED_ADDR+E2_MAX_SPEED_LEN+1)			
//参数ID0x0056，超速持续时间，单位为秒，默认为10秒

#define E2_SET_CRASH_ALARM_ADDR		                (E2_OVER_SPEED_KEEP_TIME_ADDR+E2_OVER_SPEED_KEEP_TIME_LEN+1)
//参数ID0x005D//碰撞报警参数设置 

#define E2_SET_ROLLOVER_ALARM_ADDR		        (E2_SET_CRASH_ALARM_ADDR+E2_SET_CRASH_ALARM_LEN+1)
//参数ID0x005E//侧翻报警参数设置：侧翻角度，单位1 度，默认为30 度

#define E2_CAR_TOTAL_MILE_ADDR			(E2_SET_ROLLOVER_ALARM_ADDR+E2_SET_ROLLOVER_ALARM_LEN+1)		
//参数ID0x0080，车辆里程表读数，0.1km

#define E2_CAR_PROVINCE_ID_ADDR			(E2_CAR_TOTAL_MILE_ADDR+E2_CAR_TOTAL_MILE_LEN+1)		
//参数ID0x0081，车辆所在的省域ID

#define E2_CAR_CITY_ID_ADDR			(E2_CAR_PROVINCE_ID_ADDR+E2_CAR_PROVINCE_LEN+1)			
//参数ID0x0082，车辆所在的市域ID

#define E2_CAR_PLATE_NUM_ADDR			(E2_CAR_CITY_ID_ADDR+E2_CAR_CITY_LEN+1)			
//参数ID0x0083，公安交通管理部门颁发的机动车号牌

#define E2_CAR_PLATE_COLOR_ADDR			(E2_CAR_PLATE_NUM_ADDR+E2_CAR_PLATE_NUM_LEN+2)	
//参数ID0x0084，车牌颜色，按照JT/T415-2006的5.4.12

#define E2_GPS_SET_MODE_ADDR		                (E2_CAR_PLATE_COLOR_ADDR+E2_CAR_PLATE_COLOR_LEN+1)
//参数ID0x0090//GNSS 定位模式，默认为E2_PRAM_BASE_CUSTOM_ID+0x 北斗定位


//////////////////////////////////////////////////////////////
//*************************自定义参数ID地址************************
#define E2_LOGINGPRS_PASSWORD_ID_ADDR			(E2_GPS_SET_MODE_ADDR+E2_GPS_SET_MODE_LEN+1)
//参数IDE2_PRAM_BASE_CUSTOM_ID+0x00，自定义，厂商终端ID，7字节

#define E2_MANUFACTURE_ID_ADDR		(E2_LOGINGPRS_PASSWORD_ID_ADDR+E2_LOGINGPRS_PASSWORD_ID_LEN+2)
//参数IDE2_PRAM_BASE_CUSTOM_ID+0x05，制造商ID，5字节	

#define E2_DEVICE_ID_ADDR			(E2_MANUFACTURE_ID_ADDR+E2_MANUFACTURE_ID_LEN+2)
//参数IDE2_PRAM_BASE_CUSTOM_ID+0x06，自定义，厂商终端ID，7字节

#define E2_DEVICE_PHONE_ID_ADDR			(E2_DEVICE_ID_ADDR+E2_DEVICE_ID_LEN+2)
//参数IDE2_PRAM_BASE_CUSTOM_ID+0x07，自定义，终端手机号，6字节

#define E2_CAR_VIN_ID_ADDR			(E2_DEVICE_PHONE_ID_ADDR+E2_DEVICE_PHONE_ID_LEN+2)
//参数ID 0xF208车辆识别码(VIN)，17字节，ASCII码

#define	E2_FIRMWARE_VERSION_ID_ADR	(E2_CAR_VIN_ID_ADDR+E2_CAR_VIN_ID_LEN+2)
//参数IDE2_PRAM_BASE_CUSTOM_ID 0x0C，自定义，固件版本号

#define E2_DEVICE_BACKUP_ID_ADDR			(E2_FIRMWARE_VERSION_ID_ADR+E2_FIRMWARE_VERSION_ID_LEN+2)
//参数IDE2_PRAM_BASE_CUSTOM_ID+0x2a，自定义，厂商终端ID，7字节

#define	E2_DEFINE_ALARM_MASK_WORD_ID_ADDR		(E2_DEVICE_BACKUP_ID_ADDR+E2_DEVICE_BACKUP_ID_LEN+2)
//参数IDE2_PRAM_BASE_CUSTOM_ID+0x43，自定义，车辆识别码

//32参数ID0x0001F0004，自定义，自定义硬件版本号，DWORD
#define	E2_DEFINE_HARD_VER_ID_ADDR 		(E2_DEFINE_ALARM_MASK_WORD_ID_ADDR+E2_DEFINE_ALARM_MASK_WORD_ID_LEN+2)	

//参数ID 0x0001F005，自定义，自定义用户短信报警电话
#define	E2_SMS_ALARM_PHONE_NUM_ID_ADDR 		(E2_DEFINE_HARD_VER_ID_ADDR+E2_DEFINE_HARD_VER_ID_LEN+2)	

//参数ID 0x0001F006，自定义，自定义用户短信操作密码
#define E2_SMS_OPERATE_PWD_ID_ADDR     		(E2_SMS_ALARM_PHONE_NUM_ID_ADDR+E2_SMS_ALARM_PHONE_NUM_ID_LEN+2)  

//参数ID 0x0001F00F，自定义，自定义低电压阈值，WORD
#define E2_LOW_VOL_THRESHOLD_ID_ADDR		(E2_SMS_OPERATE_PWD_ID_ADDR+E2_SMS_OPERATE_PWD_ID_LEN+2)

//*****************************************************************************************
//*****************************************************************************************
//*****************************************************************************************
//*****************************************************************************************
//***************************************自定义数据类型********************************
//*****************************************************************************************
//*****************************************************************************************
//*****************************************************************************************
#define FRAM_OIL_CTRL_ADDR                      FRAM_BASE_ADDR //断油路
#define FRAM_OIL_CTRL_ADDR_LEN                  1
///////////////////////////////////

///////////////////////////////
//升级任务触发标志，等于1表示触发了远程升级，等于2表示升级成功，一字节
//等于3表示升级失败，等于4表示已发送过0x0108命令了，每次上电鉴权完成后需要读取该值，
#define FRAM_FIRMWARE_UPDATA_FLAG_ADDR          2//升级任务触发标志，等于1表示触发了远程升级，等于2表示升级成功，                                                    //等于3表示升级失败，等于4表示已发送过0x0108命令了，每次上电鉴权完成后需要读取该值，
#define FRAM_FIRMWARE_UPDATA_FLAG_LEN           1

/////////////////////////////////////
#define FRAM_FIRMWARE_UPDATA_TYPE_ADDR          4//远程升级类型,0为终端,9为载重控制器,目前只支持这两个
#define FRAM_FIRMWARE_UPDATA_TYPE_LEN           1

#define FRAM_LAST_LOCATION_ADDR                 6	//上一个有效定位点
#define FRAM_LAST_LOCATION_LEN                  11	//上一个有效定位点

#define FRAM_MILEAGE_ADDR                       18	//总里程存储地址,带校验字节,
#define FRAM_MILEAGE_LEN                        4	//总里程存储长度,带校验字节

#define FRAM_EMERGENCY_FLAG_ADDR				23	//紧急报警上一次状态
#define FRAM_EMERGENCY_FLAG_LEN					1

#define FRAM_ROLLOVER_BASS_ADDR					25	//侧翻基准角度
#define FRAM_ROLLOVER_BASS_LEN					4

#define FRAM_SLEEP_FLAG_ADDR					30	//休眠进入标志
#define FRAM_SLEEP_FLAG_LEN						1

#define FRAM_ACC_LINE_FLAG_ADDR					32	//是否有ACC 线
#define FRAM_ACC_LINE_FLAG_LEN					1

#define FRAM_TEMP_TRACK_NUM_ADDR            	34 //临时跟踪剩余次数
#define FRAM_TEMP_TRACK_NUM_LEN             	4

#define FRAM_TEMP_TRACK_SPACE_ADDR              38   //临时跟踪间隔
#define FRAM_TEMP_TRACK_SPACE_LEN               2  

#define FRAM_TEMP_TRACK_VERIFY_ADDR              40   //crc32校验
#define FRAM_TEMP_TRACK_VERIFY_LEN               4  

#define FRAM_FCT_SELF_CHECK_ADDR              	44   //FCT自检标志
#define FRAM_FCT_SELF_CHECK_LEN               	1  

#define FRAM_RTC_WAKEUP_ADDR              		45   //RTC唤醒上报时间偏移量
#define FRAM_RTC_WAKEUP_LEN               		4  

//////////////////////










//-------------------------------------------------------------------------------------------------
//------------------------------------函数声明-----------------------------------------------------
//-------------------------------------------------------------------------------------------------
//********************************本地变量**********************************
/*************************************************************OK
** 函数名称: EepromPram_ConvertParamIDToIndex
** 功能描述: 二分法检索，返回要查找的记录
** 入口参数: 
** 出口参数: 返回要查找的Index
** 返回参数: 
** 全局变量: 无
** 调用模块:
*************************************************************/
unsigned char EepromPram_ConvertParamIDToIndex(unsigned int ParamID);
/*********************************************************************
//函数名称	:EepromPram_WritePram
//功能		:写eeprom参数
//输入		:PramID:参数ID;pBuffer:指向数据（待写入的字节流,写入的是数值的话则高字节应在前）的指针;
//		    :BufferLen:数据的长度
//输出		:
//使用资源	:
//全局变量	:   
//调用函数	:
//中断资源	:  
//返回		:
//备注		:写入正确时，返回的长度等于BufferLen;写入错误时，返回0,此时可尝试再调用该函数写一次
*********************************************************************/
u16 EepromPram_WritePramEx(unsigned short index, u8 *pBuffer, u8 BufferLen);
/*********************************************************************
//函数名称	:EepromPram_WritePram(u32 PramID, u8 *pBuffer, BufferLen)
//功能		:写eeprom参数
//输入		:PramID:参数ID;pBuffer:指向数据（待写入的字节流,写入的是数值的话则高字节应在前）的指针;
//		:BufferLen:数据的长度
//输出		:
//使用资源	:
//全局变量	:   
//调用函数	:
//中断资源	:  
//返回		:
//备注		:写入正确时，返回的长度等于BufferLen;写入错误时，返回0,此时可尝试再调用该函数写一次
*********************************************************************/
u16 EepromPram_WritePram(u32 PramID, u8 *pBuffer, u8 BufferLen);
/*********************************************************************
//函数名称	:EepromPram_ReadPram(u32 PramID, u8 *pBuffer)
//功能		:读eeprom参数
//输入		:PramID:参数ID;pBuffer:指向数据（读出的字节流，读出的是数值则高字节在前）的指针;
//输出		:
//使用资源	:
//全局变量	:   
//调用函数	:
//中断资源	:  
//返回		:
//备注		:返回值>0表示正确读出，返回值==0表示读错误;
*********************************************************************/
u16 EepromPram_ReadPramEx(unsigned short index, u8 *pBuffer);

/*********************************************************************
//函数名称	:EepromPram_UpdateVariable(u16 PramID)
//功能		:更新PramID变量
//输入		:
//输出		:
//使用资源	:
//全局变量	:   
//调用函数	:
//中断资源	:  
//返回		:
//备注		:千万注意:EepromPram_UpdateVariable(E2_CAR_TOTAL_MILE_ID)不可随便调用，只能在设置完E2_CAR_TOTAL_MILE_ID后调用              
*********************************************************************/
void EepromPram_UpdateVariable(u32 PramID);
/*********************************************************************
//函数名称	:EepromPram_DefaultSet(void)
//功能		:系统第1次运行时的默认设置
//输入		:
//输出		:
//使用资源	:
//全局变量	:   
//调用函数	:
//中断资源	:  
//返回		:
//备注		:使用恢复默认出厂设置也会将参数设置为如下所示
*********************************************************************/
void EepromPram_DefaultSet(void);
/*************************************************************OK
** 函数名称: EepromPram_GetLenEepromPramID
** 功能描述: 
** 入口参数: 
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
*************************************************************/
unsigned short EepromPram_GetLenEepromPramID(void);

unsigned char FRAM_BufferWrite(unsigned short WriteAddr,unsigned char *pBuffer,unsigned short  NumBytesToWrite);


unsigned char FRAM_BufferRead(unsigned char *pBuffer,unsigned short NumBytesToRead,unsigned long ReadAddr);


#endif

#if !defined(_PROTOCOL_DATA_DEF_H_)
#define _PROTOCOL_DATA_DEF_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "infra_types.h"
#include "infra_defs.h"

/*******************************用户可配置项***************************************/
#define V2G_MAX_PORT_NUM (2)		//充电接口数量
#define V2G_MAX_INPUT_METER_NUM (2) //直流充电桩输入电表数量
#define V2G_MAX_ALARM_LEN (5)		//最大故障告警数
/*********************************************************************************/
#define V2G_MAX_TRADE_LEN (40 + 1)		  //最大订单号长度：设备唯一标识（充电桩资产码：24位）+充电接口标识（2 位）+ 序列号(12 位)
#define V2G_MAX_CAR_VIN_LEN (17 + 1)	  //最大车辆唯一识别码长度
#define V2G_MAX_MAC_ADDR_LEN (32 + 1)	  //设备mac地址长度
#define V2G_MAX_MODEL_ID_LEN (16 + 1)	  //最大计费模型id长度
#define V2G_MAX_QRCODE_LEN (256)		  //二维码长度
#define V2G_MAX_ICCID_LEN (24)			  //sim卡iccid长度
#define V2G_MAX_PILE_TYPE_LEN (20)		  //桩类型长度
#define V2G_MAX_DEV_SN_LEN (16 + 1)		  //厂商出厂编码
#define V2G_MAX_MODEL_DEVSEG (14)		  //最大计费模型时段数
#define V2G_MAX_METER_ADDR_LEN (6)		  //电表地址长度
#define V2G_MAX_TIMESTAMP_LEN (15 + 1)	  //时间戳长度
#define V2G_MAX_SOFTWAREVER_LEN (256 + 1) //充电桩软件版本号长度
#define V2G_MAX_HARDWAREVER_LEN (256 + 1) //充电桩硬件版本号长度
#define V2G_MAX_SDKVER_LEN (256 + 1)	  //SDK版本号长度
#define V2G_MAX_LOGQUERY_LEN (38 + 1)	  //查询日志流水号长度
#define V2G_MAX_LOGAREA_LEN (255 + 1)	  //日志响应数据最大长度
#define V2G_MAX_USERID_LEN (255 + 1)	  //用户ID最大长度
#define V2G_MAX_RESULT_LEN (255 + 1)	  //执行结果描述最大长度
#define V2G_MAX_ENERGY_DATA_LEN (5)	      //充放电电量数据最大长度
#define V2G_MAX_VOL_CUR_DATA_LEN (3)	  //充放电电压电流数据最大长度
#define V2G_MAX_POWER_DATA_LEN (4)	      //充放电交流功率数据最大长度

typedef struct
{
	char product_key[IOTX_PRODUCT_KEY_LEN + 1];			//设备品类标识字符串
	char product_secret[IOTX_PRODUCT_SECRET_LEN + 1];	//设备品类密钥
	char device_name[IOTX_DEVICE_NAME_LEN + 1];			//某台设备的标识字符串:未注册前为设备出厂编号（16位长度），注册后为设备在物联管理平台的资产码（24位长度）
	char device_secret[IOTX_DEVICE_SECRET_LEN + 1];		//某台设备的设备密钥
	char device_reg_code[IOTX_DEVICE_REG_CODE_LEN + 1]; //某台设备的设备注册码
	char device_uid[IOTX_DEVICE_UID_LEN + 1];			//某台设备的出厂编号
} v2g_device_meta;

typedef enum
{
	V2G_CMD_EVENT_FIREWARE_INFO = 0,
	V2G_CMD_EVENT_VEHICLEINFO,
	V2G_CMD_EVENT_PILEABNORMAL,
	V2G_CMD_EVENT_VEHICLEABNORMAL,
	V2G_CMD_EVENT_EXECUTIONSTATUS_CHANGE,
	V2G_CMD_EVENT_VER_INFO,
	V2G_CMD_EVENT_LOGQUERY_RESULT,
	V2G_CMD_EVENT_PILE_WORKSTATUS,
	V2G_CMD_EVENT_ASK_CONFIG,
	V2G_CMD_EVENT_DCPILE_CHANGE,
} v2g_cmd_event_enum;

typedef enum
{
	V2G_CMD_PROPERTY_BATTERY_STATUS= 0,
	V2G_CMD_PROPERTY_VEHICLE_STATUS,
} v2g_cmd_property_enum;

//固件信息上报事件参数

typedef struct
{
	char simNo[V2G_MAX_ICCID_LEN];								   // 1			SIM卡号
	char modelNo[V2G_MAX_MODEL_ID_LEN];						       // 2			计量计费模型编号
	char stakeModel[V2G_MAX_PILE_TYPE_LEN];						   // 3			充电桩型号
	unsigned int vendorCode;									   // 4			生产厂商编码
	char deSn[V2G_MAX_DEV_SN_LEN];								   // 5			出厂编号//字符串
	unsigned char deType;										   // 6			桩类型
	unsigned char connetNum;									   // 7			充电接口数量
	char simMac[V2G_MAX_MAC_ADDR_LEN];							   // 8			网络MAC地址//字符串
	unsigned int longitude;										   // 9			经度
	unsigned int latitude;										   // 10		纬度
	unsigned int height;										   // 11		高度
	unsigned int gridType;										   // 12		坐标类型
	char btMac[V2G_MAX_MAC_ADDR_LEN];							   // 13		蓝牙MAC地址
	unsigned char meaType;										   // 14		计量方式
	unsigned int otRate;										   // 15		额定功率
	unsigned int otMinVol;										   // 16		输出最小电压
	unsigned int otMaxVol;										   // 17		输出最大电压
	unsigned int otCur;											   // 18		输出最大电流
	char inMeter[V2G_MAX_INPUT_METER_NUM][V2G_MAX_METER_ADDR_LEN]; // 19		交流输入电表地址//压缩BCD
	char outMeter[V2G_MAX_PORT_NUM][V2G_MAX_METER_ADDR_LEN];	   // 20		计量用电能表地址//压缩BCD
	unsigned int CT;											   // 21		电流互感器系数 默认值1
	unsigned char isGateLock;									   // 22		是否有智能门锁
	unsigned char isGroundLock;									   // 23		是否有地锁

	unsigned int minChargingCurrent;							   // 24		最小充电电流
	unsigned int minChargingPower;								   // 25		最小充电功率
	unsigned int maxDischargeVoltage;							   // 26		最高放电电压
	unsigned int minDischargeVoltage;							   // 27		最低放电电压
	unsigned int maxDischargeCurrent;							   // 28		最大放电电流
	unsigned int minDischargeCurrent;							   // 29		最小放电电流

} __attribute__((__packed__)) v2g_event_fireware_info;

//设备版本信息
typedef struct
{
	unsigned char devRegMethod;					   // 1		设备注册方式
	char pileSoftwareVer[V2G_MAX_SOFTWAREVER_LEN]; // 2		充电桩软件版本号
	char pileHardwareVer[V2G_MAX_HARDWAREVER_LEN]; // 3		充电桩硬件版本号
	char sdkVer[V2G_MAX_SDKVER_LEN];			   // 4		SDK版本号
}__attribute__((__packed__)) v2g_event_ver_info;

//设备配置数据
typedef struct
{
	unsigned int gunInfoFreq;					   		// 1		充电枪监测事件上报频率
	unsigned int vehicleInfoFreq;						// 2		车辆工作状态实时监测属性
	unsigned int batteryInfoFreq;						// 3		车辆电池工作状态实时监测属性
	unsigned int pileWarnings;						   	// 4		设备告警信息上传频率
	unsigned int vehicleWarnings;						// 5		车辆告警信息上传频率
	unsigned int offlinChaLen;						   	// 6		离线后可充放电时长
	unsigned int grndLock;						   		// 7		地锁监测上送频率
	unsigned int doorLock;							    // 8		门锁监测上送频率
	char qrCode[V2G_MAX_PORT_NUM][V2G_MAX_QRCODE_LEN]; 	// 9	 	二维码数据
} __attribute__((__packed__))  v2g_data_dev_config;

//设备日志查询服务下发参数

typedef struct
{
	unsigned char gunNo;				   // 1 枪号	
	char startDate[V2G_MAX_TIMESTAMP_LEN]; // 2 查询起始时间戳	
	char stopDate[V2G_MAX_TIMESTAMP_LEN];  // 3 查询终止时间戳	
	unsigned char askType;				   // 4 查询类型	
	char logQueryNo[V2G_MAX_LOGQUERY_LEN]; // 5 查询流水号
} v2g_service_query_log;

//日志查询服务回复参数

typedef struct
{
	unsigned char gunNo;				   // 1	枪号
	char startDate[V2G_MAX_TIMESTAMP_LEN]; // 2	查询起始时间
	char stopDate[V2G_MAX_TIMESTAMP_LEN];  // 3	查询终止时间		   
	unsigned char askType;				   // 4	查询类型
	unsigned char result;				   // 5	响应结果
	char logQueryNo[V2G_MAX_LOGQUERY_LEN]; // 6	查询流水号
} v2g_service_feedback_query_log;

//设备维护指令服务下发参数
typedef struct
{
	unsigned char ctrlType; // 控制类型
} v2g_service_dev_maintain;

//设备维护指令服务回复参数

typedef struct
{
	unsigned char ctrlType; // 1	当前控制类型
	unsigned int reason;	// 2	失败原因
} v2g_service_feedback_dev_maintain;

//设备维护状态查询服务回复参数
typedef struct
{
	unsigned char ctrlType; // 1		当前类型
	unsigned char result;	// 2		查询结果
} v2g_service_feedback_maintain_query;

// 充电枪电子锁控制服务下发参数

typedef struct
{
	unsigned char gunNo;	 // 1	充电枪编号
	unsigned char lockParam; // 2	控制
} v2g_service_lockCtrl;

//充电枪电子锁控制服务回复参数

typedef struct
{
	unsigned char gunNo;	  // 1	充电枪编号
	unsigned char lockStatus; // 2	电子锁状态
	unsigned int resCode;	  // 3	结果
} v2g_service_feedback_lockCtrl;

//计费模型更新服务下发参数

typedef struct
{
	unsigned char timeNum;				   		// 1		电费模型时段数N 取值范围：1—48
	char timeSeg[V2G_MAX_MODEL_DEVSEG][5]; 		// 2		电费模型时段开始时间点
	unsigned int  segFlag[V2G_MAX_MODEL_DEVSEG];// 3		电费模型时段标志
	unsigned int chargeFee[4];			   		// 4		电费模型
	unsigned int serviceFee[4];			   		// 5		服务费费模型
} v2g_service_issue_feeModel;

//计费模型更新结果设备回复参数

typedef struct
{
	unsigned char res;				   // 1		失败原因
} v2g_service_feedback_feeModel;

//远程启动充放电时段参数

typedef struct
{
	char beginTime[V2G_MAX_TIMESTAMP_LEN];	// 1	开始时间
	char endTime[V2G_MAX_TIMESTAMP_LEN];	// 2	结束时间
	unsigned char direction;				// 3	充放电方向
	unsigned int power;						// 4	功率
} startcharge_times;

//远程启动充放电服务下发参数

typedef struct
{
	unsigned char gunNo;						// 1	充电枪编号
	char applyNo[V2G_MAX_TRADE_LEN]; 			// 2	订单编号
	char userId[V2G_MAX_USERID_LEN];			// 3	用户ID
	char VIN[V2G_MAX_CAR_VIN_LEN];				// 4	vin码
	unsigned char decisionType;					// 5	决策类型
	char decisionTime[V2G_MAX_TIMESTAMP_LEN]; 	// 6	决策时间
	startcharge_times times;					// 7	充放电时段数据单元

} v2g_service_remoteStart;

//启动充电服务设备回复参数

typedef struct
{
	unsigned char gunNo;				// 1	充电枪编号
	char applyNo[V2G_MAX_TRADE_LEN]; 	// 2	订单编号
	char VIN[V2G_MAX_CAR_VIN_LEN];		// 3	vin码
	unsigned char result;				// 4	执行结果
	char resultDes[V2G_MAX_RESULT_LEN];	// 5	执行结果描述

} v2g_service_feedback_remoteStart;

//平台下发充电模式服务

typedef struct
{
	unsigned char gunNo; 		// 1 充电枪编号
	unsigned char chargeMode;	// 2 充电模式
} v2g_service_charge_mode;

//平台下发充电模式服务

typedef struct
{
	unsigned char gunNo; 					// 1 充电枪编号
	unsigned char result;					// 2 执行结果
	char resultDes[V2G_MAX_RESULT_LEN];		// 3 执行结果描述
} v2g_service_feedback_charge_mode;

//车辆信息上报事件参数

typedef struct
{
	unsigned char gunNo;					// 1	充电枪编号
	char VIN[V2G_MAX_CAR_VIN_LEN];			// 2	vin码
	unsigned char carType;					// 3	V2G车	
	unsigned char currentSOC;				// 4	当前SOC
	unsigned char lowestSOC;				// 5	最低允许SOC
	unsigned int batteryTotalCapacity;		// 6	整车动力蓄电池系统总容量
	unsigned int batteryTotalEnergy;		// 7	整车动力蓄电池系统总能量
	unsigned int batteryRatedTotalVoltage;	// 8	额定总电压
	unsigned int maxAllowTemperature;		// 9	最高允许温度
	unsigned int maxAllowChargingCurrent;	// 10	最高允许充电电流
	unsigned int maxAllowChargingVoltage;	// 11	最高允许充电电压
	unsigned int maxAllowChargingPower;		// 12	最高允许充电功率
	unsigned int maxAllowDischargeCurrent;	// 13	最高允许放电电流
	unsigned int maxAllowDischargeVoltage;	// 14	最高允许放电电压
	unsigned int maxAllowDischargePower;	// 15	最高允许放电功率
	unsigned char batteryType;				// 16	电池类型
	unsigned int participateV2GNum;			// 17	参与 V2G 循环次数
	unsigned int surplusV2GNum;				// 18	剩余 V2G 循环次数
	unsigned int surplusMileage;			// 19	剩余续航里程
} v2g_event_vehicleInfo;

//召测车辆信息服务

typedef struct
{
	unsigned char gunNo;				// 1	充电枪编号
} v2g_service_callVehicle;

//召测车辆信息服务设备回复参数

typedef struct
{
	unsigned char gunNo;					// 1	充电枪编号
	char VIN[V2G_MAX_CAR_VIN_LEN];			// 2	vin码
	unsigned char carType;					// 3	V2G车
	unsigned char currentSOC;				// 4	当前SOC
	unsigned char lowestSOC;				// 5	最低允许SOC
	unsigned int batteryTotalCapacity;		// 6	整车动力蓄电池系统总容量
	unsigned int batteryTotalEnergy;		// 7	整车动力蓄电池系统总能量
	unsigned int batteryRatedTotalVoltage;	// 8	额定总电压
	unsigned int maxAllowTemperature;		// 9	最高允许温度
	unsigned int maxAllowChargingCurrent;	// 10	最高允许充电电流
	unsigned int maxAllowChargingVoltage;	// 11	最高允许充电电压
	unsigned int maxAllowChargingPower;		// 12	最高允许充电功率
	unsigned int maxAllowDischargeCurrent;	// 13	最高允许放电电流
	unsigned int maxAllowDischargeVoltage;	// 14	最高允许放电电压
	unsigned int maxAllowDischargePower;	// 15	最高允许放电功率
	unsigned char batteryType;				// 16	电池类型
	unsigned int participateV2GNum;			// 17	参与 V2G 循环次数
	unsigned int surplusV2GNum;				// 18	剩余 V2G 循环次数
	unsigned int surplusMileage;			// 19	剩余续航里程
} v2g_service_feedback_callVehicle;

//远程停止充电服务下发参数

typedef struct
{
	unsigned char gunNo;					// 1	充电枪编号
	char VIN[V2G_MAX_CAR_VIN_LEN]; 			// 2	vin码
	char applySheetNo[V2G_MAX_TRADE_LEN];	// 3	订单编号
} v2g_service_remoteStop;

//远程停止充电服务设备回复参数

typedef struct
{
	unsigned char result;					// 1	结果
	char resultDes[V2G_MAX_RESULT_LEN]; 	// 2	结果描述
	char applySheetNo[V2G_MAX_TRADE_LEN];	// 3	订单编号
} v2g_service_feedback_remoteStop;

//设备故障告警事件上传参数

typedef struct
{
	unsigned char gunNo;						  // 1	枪编号
	unsigned short faultSum;					  // 2	故障总
	unsigned short warnSum;						  // 3	告警总
	unsigned short faultValue[V2G_MAX_ALARM_LEN]; // 4	故障点数据
	unsigned short warnValue[V2G_MAX_ALARM_LEN];  // 5	告警点数据
	char startTime[V2G_MAX_TIMESTAMP_LEN];		  // 6 	发生时间
} v2g_event_pile_abnormal;

//车辆故障告警事件上传参数

typedef struct
{
	unsigned char gunNo;						  // 1	枪编号
	char vehicleVIN[V2G_MAX_CAR_VIN_LEN]; 		  // 2	vin码
	char startTime[V2G_MAX_TIMESTAMP_LEN];		  // 3 	发生时间
	unsigned short faultSum;					  // 4	故障总
	unsigned short warnSum;						  // 5	告警总
	unsigned short faultValue[V2G_MAX_ALARM_LEN]; // 6	故障点数据
	unsigned short warnValue[V2G_MAX_ALARM_LEN];  // 7	告警点数据
} v2g_event_vehicle_abnormal;

//充放电执行事件上传参数

typedef struct
{
	unsigned char gunNo;											// 1	充电枪编号
	char applySheetNo[V2G_MAX_TRADE_LEN]; 							// 2    订单编号
	char vehicleVIN[V2G_MAX_CAR_VIN_LEN]; 							// 3	vin码
	unsigned char executionStatus;									// 4	执行状态
	unsigned int chargingEnergyValue[V2G_MAX_ENERGY_DATA_LEN];		// 5	直流电表-充电电能示值
	unsigned int disChargingEnergyValue[V2G_MAX_ENERGY_DATA_LEN];	// 6	直流电表-放电电能示值
	unsigned int cumulativeTime;									// 7	累计充（放） 电时间
	unsigned int cumulativeCharge[V2G_MAX_ENERGY_DATA_LEN];			// 8	直流电表-累计充电电量
	unsigned int cumulativeDischarge[V2G_MAX_ENERGY_DATA_LEN];		// 9	直流电表-累计放电电量
	unsigned char socNumber;										// 10	当前荷电状态 SOC（%） 
	unsigned int acChargingEnergyValue[V2G_MAX_ENERGY_DATA_LEN];	// 11	交流电表-充电电能示值
	unsigned int acDisChargingEnergyValue[V2G_MAX_ENERGY_DATA_LEN];	// 12	交流电表-放电电能示值
	unsigned int acCumulativeCharge[V2G_MAX_ENERGY_DATA_LEN];		// 13	交流电表-累计充电电量
	unsigned int acCumulativeDischarge[V2G_MAX_ENERGY_DATA_LEN];	// 14	交流电表-累计放电电量
	unsigned int eventDesc;											// 15	事件描述
} v2g_event_executionStatus_change;

//直流充放电设备充电枪监测事件

typedef struct
{
	unsigned char gunNo;				   							// 1	枪序号
	char applySheetNo[V2G_MAX_TRADE_LEN]; 							// 2    订单编号
	unsigned char executionStatus;				   					// 3	执行状态
	unsigned int chargingEnergyValue[V2G_MAX_ENERGY_DATA_LEN];		// 4	直流电表-充电电能示值
	unsigned int disChargingEnergyValue[V2G_MAX_ENERGY_DATA_LEN];	// 5	直流电表-放电电能示值	
	unsigned int voltage;				   							// 6	直流电表-充（放）电压
	unsigned int current;				   							// 7	直流电表-充（放）电电流测量值
	unsigned int electricPower;				   						// 8	直流电表-充（放）电功率测量值
	unsigned int cumulativeTime;				   					// 9	累计充（放）电时间
	unsigned int cumulativeCharge[V2G_MAX_ENERGY_DATA_LEN];			// 10	直流电表-累计充电电量	
	unsigned int cumulativeDischarge[V2G_MAX_ENERGY_DATA_LEN];		// 11	直流电表-累计放电电量	
	unsigned int acChargingEnergyValue[V2G_MAX_ENERGY_DATA_LEN];	// 12	交流电表-充电电能示值	
	unsigned int acDisChargingEnergyValue[V2G_MAX_ENERGY_DATA_LEN];	// 13	交流电表-放电电能示值	
	unsigned int acVoltage[V2G_MAX_VOL_CUR_DATA_LEN];				// 14	交流电表-充（放）电压	
	unsigned int acCurrent[V2G_MAX_VOL_CUR_DATA_LEN];				// 15	交流电表-充（放）电电流测量值	
	unsigned int acElectricPower[V2G_MAX_POWER_DATA_LEN];			// 16	交流电表-充（放）电功率测量值	
	unsigned int acCumulativeCharge[V2G_MAX_ENERGY_DATA_LEN];		// 17	交流电表-累计充电电量	
	unsigned int acCumulativeDischarge[V2G_MAX_ENERGY_DATA_LEN];	// 18	交流电表-累计放电电量	
} v2g_event_pile_workstatus;

//召测充电桩信息服务回复参数

typedef struct
{
	unsigned int maxChargingVoltage;	// 1	最高充电电压
	unsigned int minChargingVoltage; 	// 2	最低充电电压
	unsigned int maxChargingCurrent; 	// 3	最大充电电流
	unsigned int minChargingCurrent; 	// 4	最小充电电流
	unsigned int maxChargingPower; 		// 5	最大充电功率
	unsigned int minChargingPower; 		// 6	最小充电功率
	unsigned int maxDischargeVoltage; 	// 7	最高放电电压
	unsigned int minDischargeVoltage; 	// 8	最低放电电压
	unsigned int maxDischargeCurrent; 	// 9	最大放电电流
	unsigned int minDischargeCurrent; 	// 10	最小放电电流
} v2g_service_feedback_callPile_info;

//车辆电池状态实时监测

typedef struct
{
	unsigned char gunNo;							// 1	枪序号
	char vehicleVIN[V2G_MAX_CAR_VIN_LEN]; 			// 2	车辆 VIN 
	unsigned int highBatteryVoltage;				// 3	最高单体动力蓄电池电压
	unsigned int highBatteryNumber;					// 4	最高单体动力蓄电池组号
	unsigned int lowBatteryVoltage;					// 5	最低单体动力蓄电池电压
	unsigned int lowBatteryNumber;					// 6	最低单体动力蓄电池组号
	unsigned int highOneBatteryTemperature;			// 7	最高单体动力蓄电池温度
	unsigned int highOneBatteryTemperatureNumber;	// 8	最高单体动力蓄电池温度所在组号
	unsigned int lowOneBatteryTemperature;			// 9	最低单体动力蓄电池温度
	unsigned int lowOneBatteryTemperatureNumber;	// 10	最低单体动力蓄电池温度所在组号
	unsigned int highBatteryTemperature;			// 11	最高动力蓄电池温度
	unsigned int highTemperatureNumber;				// 12	最高温度检测点编号
	unsigned int lowBatteryTemperature;				// 13	最低动力蓄电池温度
	unsigned int lowTemperatureNumber;				// 14	最低动力蓄电池温度检测点编号
	unsigned int batteryVoltage;					// 15	单体动力蓄电池电压过高/过低
	unsigned int socStatus;							// 16	整车动力蓄电池荷电状态 SOC
	unsigned int batteryCurrent;					// 17	动力蓄电池充（放）电过电流 
	unsigned int batteryTemperature;				// 18	动力蓄电池温度过高 
	unsigned int batteryInsulation;					// 19	动力蓄电池绝缘状态
	unsigned int batteryPackOutConnect;				// 20	动力蓄电池组输出连接器连接状态 
	unsigned int allowChargeOrDischarge;			// 21	充（放）电允许

} v2g_property_battery_status;

//车辆工作状态实时监测

typedef struct
{
	unsigned char gunNo;		  			// 1	枪序号
	char vehicleVIN[V2G_MAX_CAR_VIN_LEN]; 	// 2	车辆 VIN
	unsigned char executionStatus;	  		// 3	执行状态
	unsigned int voltage;		  			// 4	充（放）电电压测量值
	unsigned int current;		  			// 5	充（放）电电流测量值
	unsigned int electricPower;		  		// 6	充（放）电功率测量值
	unsigned char socNumber;		  		// 7	当前荷电状态 SOC
	unsigned int participateV2GNum;		  	// 8	参与V2G 循环次数
	unsigned int surplusV2GNum;		  		// 9	剩余V2G 循环次数 
	unsigned int surplusMileage;		  	// 10	剩余续航里程
} v2g_property_vehicle_status;

//设备日志查询结果上报事件
typedef union
{
	char rawData[V2G_MAX_LOGAREA_LEN];
} u_logData;

typedef struct
{
	unsigned char gunNo;				   // 1	充电枪编号
	char startDate[V2G_MAX_TIMESTAMP_LEN]; // 2	查询起始时间
	char stopDate[V2G_MAX_TIMESTAMP_LEN];  // 3	查询终止时间	
	unsigned char askType;				   // 4	查询类型
	unsigned char result;				   // 5	响应结果
	char logQueryNo[V2G_MAX_LOGQUERY_LEN]; // 6	查询流水号
	unsigned char retType;				   // 7	响应类型
	unsigned int logQueryEvtSum;		   // 8	日志结果上报事件总帧数
	unsigned int logQueryEvtNo;			   // 9	日志结果上报帧序号
	u_logData dataArea;					   // 10 响应数据区
} v2g_event_logQuery_Result;

//直流充电枪状态变更事件

typedef struct
{
	unsigned char gunNo;		   				// 1	充电枪编号
	char yxOccurTime[V2G_MAX_TIMESTAMP_LEN];	// 2	发生时刻
	unsigned char connCheckStatus; 				// 3	变位点数据
} evs_event_pile_status_change;

/******************************************需用户实现的回调函数*************************************************/

/**
 *
 * 函数 callback_service_query_log() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of callback_service_query_log() requires to be implemented by user of SDK.
 *
 * 
 */
/**
 * @brief Reply IOT platform device data get service by result through SDK.
 *---
 * @param [out] result: @n the result data will be written by user of SDK.
 * @return failed -1 success 0.
 * @see None.
 * @note None.
 */
int callback_service_query_log(v2g_service_query_log *param, v2g_service_feedback_query_log *result);

/**
 *
 * 函数 callback_service_lockCtrl() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of callback_service_lockCtrl() requires to be implemented by user of SDK.
 *
 * 
 */
/**
 * @brief Reply IOT platform chargeGun e-lock control service by param through SDK.
 *---
 * @param [in] param: @n the param received from IOT platform.
 * @param [out] result: @n the result data will be written by user of SDK.
 * @return failed -1 success 0.
 * @see None.
 * @note None.
 */
int callback_service_lockCtrl(v2g_service_lockCtrl *param, v2g_service_feedback_lockCtrl *result);

/**
 *
 * 函数 callback_service_issue_feeModel() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of callback_service_issue_feeModel() requires to be implemented by user of SDK.
 *
 * 
 */
/**
 * @brief Reply IOT platform update feemodel service by result through SDK.
 *---
 * @param [out] result: @n the result data will be written by user of SDK.
 * @return failed -1 success 0.
 * @see None.
 * @note None.
 */
int callback_service_issue_feeModel(v2g_service_issue_feeModel *param, v2g_service_feedback_feeModel *result);

/**
 *
 * 函数 callback_service_remoteStart() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of callback_service_remoteStart() requires to be implemented by user of SDK.
 *
 * 
 */
/**
 * @brief Reply IOT platform remote start service by param through SDK.
 *---
 * @param [in] param: @n the param received from IOT platform.
 * @param [out] result: @n the result data will be written by user of SDK.
 * @return failed -1 success 0.
 * @see None.
 * @note None.
 */
int callback_service_remoteStart(v2g_service_remoteStart *param, v2g_service_feedback_remoteStart *result);

/**
 *
 * 函数 callback_service_remoteStop() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of callback_service_remoteStop() requires to be implemented by user of SDK.
 *
 * 
 */
/**
 * @brief Reply IOT platform remote stop service by param through SDK.
 *---
 * @param [in] param: @n the param received from IOT platform.
 * @param [out] result: @n the result data will be written by user of SDK.
 * @return failed -1 success 0.
 * @see None.
 * @note None.
 */
int callback_service_remoteStop(v2g_service_remoteStop *param, v2g_service_feedback_remoteStop *result);

/**
 *
 * 函数 callback_service_callVehicle_info() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of callback_service_callVehicle_info() requires to be implemented by user of SDK.
 *
 * 
 */
/**
 * @brief Reply IOT platform call vehicle information service by param through SDK.
 *---
 * @param [in] param: @n the param received from IOT platform.
 * @param [out] result: @n the result data will be written by user of SDK.
 * @return failed -1 success 0.
 * @see None.
 * @note None.
 */
int callback_service_callVehicle_info(v2g_service_callVehicle *param, v2g_service_feedback_callVehicle *result);

/**
 *
 * 函数 callback_service_charge_mode() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of callback_service_charge_mode() requires to be implemented by user of SDK.
 *
 * 
 */
/**
 * @brief Reply IOT platform charge mode service by param through SDK.
 *---
 * @param [in] param: @n the param received from IOT platform.
 * @param [out] result: @n the result data will be written by user of SDK.
 * @return failed -1 success 0.
 * @see None.
 * @note None.
 */
int callback_service_charge_mode(v2g_service_charge_mode *param, v2g_service_feedback_charge_mode *result);

/**
 *
 * 函数 callback_service_callPile_info() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of callback_service_callPile_info() requires to be implemented by user of SDK.
 *
 * 
 */
/**
 * @brief Reply IOT platform call pile information service by param through SDK.
 *---
 * @param [in] param: @n the param received from IOT platform.
 * @param [out] result: @n the result data will be written by user of SDK.
 * @return failed -1 success 0.
 * @see None.
 * @note None.
 */
int callback_service_callPile_info(v2g_service_feedback_callPile_info *result);
/**
 *
 * 函数 callback_service_get_config() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of callback_service_get_config() requires to be implemented by user of SDK.
 *
 * 
 */
/**
 * @brief Reply IOT platform device configuration get service by result through SDK.
 *---
 * @param [out] result: @n the result data will be written by user of SDK.
 * @return failed -1 success 0.
 * @see None.
 * @note None.
 */
int callback_service_get_config(v2g_data_dev_config *result);

/**
 *
 * 函数 callback_service_get_maintain_status() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of callback_service_get_maintain_status() requires to be implemented by user of SDK.
 *
 * 
 */
/**
 * @brief Reply IOT platform device configuration get service by result through SDK.
 *---
 * @param [out] result: @n the result data will be written by user of SDK.
 * @return failed -1 success 0.
 * @see None.
 * @note None.
 */
int callback_service_get_maintain_status(v2g_service_feedback_maintain_query *result);

/**
 *
 * 函数 callback_service_set_maintain_status() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of callback_service_set_maintain_status() requires to be implemented by user of SDK.
 *
 * 
 */
/**
 * @brief Reply IOT platform device configuration set service by result through SDK.
 *---
 * @param [out] result: @n the result data will be written by user of SDK.
 * @return failed -1 success 0.
 * @see None.
 * @note None.
 */
int callback_service_set_maintain_status(v2g_service_dev_maintain *param, v2g_service_feedback_dev_maintain *result);

/**
 *
 * 函数 callback_service_update_config() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of callback_service_update_config() requires to be implemented by user of SDK.
 *
 * 
 */
/**
 * @brief Reply IOT platform update devicce configuration service by param through SDK.
 *---
 * @param [in] param: @n the param received from IOT platform.
 * @param [out] result: @n the result data will be written by user of SDK.
 * @return failed -1 success 0.
 * @see None.
 * @note None.
 */
int callback_service_update_config(v2g_data_dev_config *param, int *result);

/**
 *
 * 函数 callback_service_ota_update() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of callback_service_ota_update() requires to be implemented by user of SDK.
 *
 * 
 */
/**
 * @brief perform the ota order  from IOT platform .
 *---
 * @param [in] version: @n the new firmware version received from IOT platform.
 * 
 * @return do not update device -1 update device 0.
 * @see None.
 * @note None.
 */
int callback_service_ota_update(const char *version);

/**
 *
 * 函数 callback_service_time_sync() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of callback_service_time_sync() requires to be implemented by user of SDK.
 *
 * 
 */
/**
 * @brief send timestamp that Recived from IOT platform to user by timestamp through SDK.
 *---
 * @param [in] param: @n the param received from IOT platform.
 * @param [out] result: @n the result data will be written by user of SDK.
 * @return failed -1 success 0.
 * @see None.
 * @note None.
 */
int callback_service_time_sync(const unsigned int timestamp);

#endif

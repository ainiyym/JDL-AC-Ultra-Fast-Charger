//******************************************************************************
//* File Name: Debug.c
//* Project Name: JDL_CBCU
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Debug module source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Debug.h"
#include "STD_RlyM.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/
#define RX_BUFFER_SIZE 64

#define DEBUGM_INFO(fmt, ...) LOG_INFO(LOG_MODULE_XXX, fmt, ##__VA_ARGS__)	 /* log output */
#define DEBUGM_WARN(fmt, ...) LOG_WARN(LOG_MODULE_XXX, fmt, ##__VA_ARGS__)	 /* log output */
#define DEBUGM_ERROR(fmt, ...) LOG_ERROR(LOG_MODULE_XXX, fmt, ##__VA_ARGS__) /* log output */
/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/
// 指令回调函数类型定义
typedef void (*CommandCallback)(void);
// 指令结构体
typedef struct
{
	const char *command;	  // 指令字符串
	CommandCallback callback; // 对应的回调函数
} CommandEntry;

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Global variables Declaration
|******************************************************************************/
static uint8_t g_rxBuffer[RX_BUFFER_SIZE];
static uint32_t g_rxIndex = 0;

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/
static void FunctionA(void);
static void FunctionB(void);
static void FunctionC(void);
static void FunctionD(void);

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/
// 指令表
CommandEntry g_commandTable[] = {
	{"1\r\n", FunctionA},
	{"2\r\n", FunctionB},
	{"3\r\n", FunctionC},
	{"4\r\n", FunctionD},
	{NULL, NULL} // 结束标记
};

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
// 用户定义的函数A和函数B
static void FunctionA(void)
{
	// 这里实现功能A
	RELAYM_ReqRelaySwitchOn(SYS_CONNECTOR1);
	DEBUGM_INFO("%s.\r\n", __FUNCTION__);
}

static void FunctionB(void)
{
	// 这里实现功能B
	RELAYM_ReqRelaySwitchOff(SYS_CONNECTOR1);
	DEBUGM_INFO("%s.\r\n", __FUNCTION__);
}

static void FunctionC(void)
{
	// 这里实现功能C
	RELAYM_ReqRelaySwitchOn(SYS_CONNECTOR2);
	DEBUGM_INFO("%s.\r\n", __FUNCTION__);
}

static void FunctionD(void)
{
	// 这里实现功能D
	RELAYM_ReqRelaySwitchOff(SYS_CONNECTOR2);
	DEBUGM_INFO("%s.\r\n", __FUNCTION__);
}

// LOG调试核心函数
void UsartCommand_Parser(void)
{
	uint32_t receivedSize;
	uint8_t tempBuffer[RX_BUFFER_SIZE];

	// 从USART读取数据
	receivedSize = Mcal_Usart_AppReceiveData(MCAL_USART2_CH, tempBuffer, RX_BUFFER_SIZE - 1);

	if (receivedSize > 0)
	{
		// 将新数据添加到接收缓冲区
		for (uint32_t i = 0; i < receivedSize && g_rxIndex < RX_BUFFER_SIZE - 1; i++)
		{
			g_rxBuffer[g_rxIndex++] = tempBuffer[i];

			// 确保字符串以NULL结尾
			g_rxBuffer[g_rxIndex] = '\0';

			// 调试输出：打印每个接收到的字符
			DEBUGM_INFO("[DEBUG] Received: 0x%02X ('%c')\r\n",
						tempBuffer[i],
						(tempBuffer[i] >= 32 && tempBuffer[i] <= 126) ? tempBuffer[i] : '.');

			// 检查是否收到完整指令（以\r\n结尾）
			if (g_rxIndex >= 2 &&
				g_rxBuffer[g_rxIndex - 2] == '\r' &&
				g_rxBuffer[g_rxIndex - 1] == '\n')
			{

				DEBUGM_INFO("[DEBUG] Complete command received: %s", g_rxBuffer);

				// 遍历指令表查找匹配的指令
				for (uint32_t cmdIndex = 0; g_commandTable[cmdIndex].command != NULL; cmdIndex++)
				{
					if (strcmp((char *)g_rxBuffer, g_commandTable[cmdIndex].command) == 0)
					{
						DEBUGM_INFO("[INFO] Executing command: %s", g_rxBuffer);

						// 执行对应的回调函数
						g_commandTable[cmdIndex].callback();
						break;
					}
				}
				// 重置接收缓冲区
				g_rxIndex = 0;
				g_rxBuffer[0] = '\0';
			}
		}

		// 防止缓冲区溢出
		if (g_rxIndex >= RX_BUFFER_SIZE - 1)
		{
			DEBUGM_INFO("[ERROR] RX buffer overflow! Resetting...\r\n");
			g_rxIndex = 0;
			g_rxBuffer[0] = '\0';
		}
	}
}

// 打印可用指令列表
void UsartCommand_PrintHelp(void)
{
	DEBUGM_INFO("=== Available Commands ===\r\n");
	for (uint32_t i = 0; g_commandTable[i].command != NULL; i++)
	{
		DEBUGM_INFO("Command: %s", g_commandTable[i].command);
	}
	DEBUGM_INFO("======= Command List End =======\r\n");
}

/* EOL */

# Copilot / AI agent instructions — JDL AC Ultra-Fast Charger

目的：帮助 AI 代码代理（Copilot、自动化脚本）快速上手本仓库，聚焦可立即执行的知识、约定与示例。

## 一、项目“大局”概览 🔧
- 本仓库是嵌入式固件（目标：STM32F107，Keil MDK5 uVision 工程）。主工程文件：`JDL _AC_Ultra-Fast_Charger.uvprojx`。
- 代码分层：
  - `Source/BSW/`：基础服务（网络、TCP、串口等）。
  - `Source/Core/`：平台/RTOS 支撑。
  - `Source/APP/`：应用层模块（例如 `CloudM/`, `CanM/`, `FlashDBM/` 等）。
- Cloud 模块：`Source/APP/CloudM/Cloud_ProtocolM/Cloud_Protocol_Sg/` 实现了“SG”云协议（登录、时间同步、事件上报、服务调用、计费/订单等）。
- 数据持久化：使用 Flash/TSDB（`FlashDBM`），cloud 层会把订单/记录以 `cloud_protocol_sg_order_record_t` 格式保存到 TSDB 并在网络可用时上传。

## 二、关键开发流程（如何构建/快速检查/调试） ✅
- 全量固件编译：在 Keil uVision 中打开 `JDL _AC_Ultra-Fast_Charger.uvprojx` 并 Build（推荐用于生成可烧录固件与调试）。
- 快速本地语法/静态检查：VS Code 提供任务 `C/C++: gcc.exe 生成活动文件` （会调用 MinGW 的 `gcc` 编译当前文件），仅用于语法/类型检查，非目标平台二进制。
- 烧写/调试：使用 Keil 的调试器（ST-Link/ULINK 等），在硬件上验证网络、MQTT、Flash 写入等交互。
- 单元测试：仓库中没有显式 unit-test 框架或测试目录，变更后需在硬件或模拟器上验证（或添加独立 host-side 模块进行隔离测试）。

## 三、项目约定与常见模式 🧭
- 命名约定：模块前缀 + 功能，例如 `Cloud_Protocol_Sg_<Feature>.<c|h>`；函数以 `Cloud_Protocol_Sg_` 或 `Cloud_Protocol_<X>_` 为前缀。
- 头文件护栏：使用 `#if !defined (__FOO_H)` / `#define __FOO_H` 风格。
- 日志宏：使用 `CLOUD_INFO`, `CLOUD_WARN`, `CLOUD_ERROR` 等统一宏记录运行信息与错误（请保持日志语句简洁并包含 `__func__` 或上下文）。
- 配置宏：功能按宏控制（例如 `CLOUD_PROTOCOL_SG_PROTOCOL_ENABLE` 在 `Cloud_Protocol_Cfg.h` 中），更新宏后必须全工程 rebuild。
- JSON 处理：使用 `cJSON` 解析云端 payload（服务调用处理函数常接收 `cJSON *params`）。
- 消息处理模式：
  - 先用 `cloud_protocol_sg_detect_message_type()` 从 payload 判断消息类型；
  - 再用 `cloud_protocol_sg_detect_message_rcv_type()` 判断主动/被动；
  - 服务类使用 `cloud_protocol_dispatch_service_call()` 分发。

## 四、集成点与外部依赖 🔗
- MCU/工具链：STM32F107、Keil MDK5；工程文件为 `.uvprojx` / `.uvoptx`。
- 网络：TCP/MQTT 层在 `Source/BSW/Services/CommunicationServices/NetM/`，SG 协议的 IP/PORT 在 `Tcp_Cfg.h` (`CLOUD_PROTOCOL_SG_IP`, `CLOUD_PROTOCOL_SG_PORT`)。
- 数据库：Flash TSDB（FlashDBM），示例：`Cloud_Protocol_Sg_Order_SaveOrderToTsdb()` / `Cloud_Protocol_Sg_Order_GetNextPendingRecord()`。
- 协议约定：事件上报方法名由 `CLOUD_PROTOCOL_SG_EVENT_POST_METHOD_PREFIX`/`SUFFIX` 组合产生（见 `Cloud_Protocol_Sg_Cfg.h`）。

## 五、如何添加新云协议处理器（最小可行步骤，示例化） ✍️
1. 在 `Cloud_Protocol_Sg/Protocol/` 下新建 `Cloud_Protocol_Sg_<Feature>.[c|h]`，使用相同前缀与 header-guard 风格。
2. 在 `Cloud_Protocol_SgM.c` 的 init 部分调用 `Cloud_Protocol_Sg_<Feature>_Init()`（保持与现有模块一致的启动/周期调用方式）。
3. 如果要处理主动上报 -> 使用 `Cloud_Protocol_Sg_Build_Topic()` 构造 topic；若处理下发服务，扩展 `cloud_protocol_dispatch_service_call()` 的分支并在 `cloud_protocol_detect_service_call_type()` 注册 method。
4. 如果需要落盘：使用 `cloud_protocol_sg_order_record_t` 格式或添加对应 TSDB 存取函数（查看 `FlashDB_AppM.c` 示范）。
5. 写好日志（`CLOUD_INFO/WARN/ERROR`）并在 Keil 上做硬件验证；考虑离线/网络断开场景（`Cloud_Protocol_Sg_Order_SetNetworkStatus()`）。

## 六、已知陷阱与测试要点 ⚠️
- 网络离线策略：当网络断开时，活跃订单会记录 `offline_start_time` 并在 `CLOUD_PROTOCOL_SG_OFFLINE_TIMEOUT` 后强制结束并保存到 TSDB — 测试需模拟断网场景。
- 上传重试：上传失败的记录会在 TSDB 中保留并在网络恢复后按 `GetNextPendingRecord` 上报，避免重复导致的竞态需注意 `is_uploading_pending` 标志。
- 字符串边界：订单 ID 与 topic 长度有限制，要使用 `strncpy` 并确保以 '\0' 结尾（项目中多处有此实践）。

## 七、参考文件（快速索引） 📁
- 协议实现：`Source/APP/CloudM/Cloud_ProtocolM/Cloud_Protocol_Sg/`（`Cloud_Protocol_SgM.c`, `Cloud_Protocol_Sg_Login.c`, `..._ChargingOrder.c` 等）
- 配置：`Source/APP/CloudM/Cloud_ProtocolM/Cloud_Protocol_Cfg.h`, `Cloud_Protocol_Sg_Cfg.h`
- 网络配置：`Source/BSW/Services/.../NetM/TCP/Tcp_Cfg.h`（IP/PORT）
- TSDB/Flash：`Source/APP/FlashDBM/FlashDB_AppM.c`
- 消息分发示例：`Cloud_Protocol_Sg_ServiceCall.c`

---

请告诉我：有没有希望我补充的项（例如：常用 payload 示例、更多代码风格规则、或针对 CI/自动化的具体任务建议）？我会根据反馈继续迭代此文件。 ✅
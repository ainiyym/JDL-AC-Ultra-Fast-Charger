//*******************************************************************************
//*
//* File Name: fdbM.h 
//* Project Name: JDL_AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-09-11 16:56:24
//* Author: zhousong
//* 
/*******************************************************************************/
#ifndef _FDB_M_H_
#define _FDB_M_H_

#include "stdint.h"
extern struct fdb_kvdb kvdb;
extern struct fdb_tsdb tsdb_gun1;
extern struct fdb_tsdb tsdb_gun2;

extern int fdb_init(void);
extern void tsdb_time_synced_init(uint32_t* timestamp);
extern uint32_t* powerdown_get_ts_timestamp(void);
extern uint32_t get_ts0_time(void);
extern uint32_t get_ts1_time(void);

#endif /* _FDB_M_H_ */

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

extern struct fdb_kvdb kvdb;
extern struct fdb_tsdb tsdb_gun1;
extern struct fdb_tsdb tsdb_gun2;

extern int fdb_init(void);

#endif /* _FDB_M_H_ */

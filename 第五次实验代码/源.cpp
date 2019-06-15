﻿
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "Define.h"
#include "Operation.h"
using namespace std;
#define DIR		0
#define	CREATE	1
#define	DELETE	2
#define	OPEN	3
#define	CLOSE	4
#define	READ	5	
#define	WRITE	6
#define HELP	7
#define LSEEK	8
#define EXIT	9
#define	OPLIST	10
#define LDISK	11
#define CH_LENGTH	20


void main()
{

	printf("\t\t\t欢迎使用该文件系统，使用帮助如下\n");
	show_help();
	Init();
	create("lwmorz1");
	create("lwmorz2");
	create("lwmorz3");
	open("lwmorz1");
	open("lwmorz3");

	char ch[CH_LENGTH];
	Init_block(ch, CH_LENGTH);
	while (gets_s(ch))
	{

		int cmd;
		char filename[FILE_NAME_LENGTH];
		//初始化
		cmd = -1;
		Init_block(filename, FILE_NAME_LENGTH);
		if (strncmp("dir", ch, 3) == 0)			//浏览目录dir(non)
		{
			cmd = DIR;
		}
		if (strncmp("create", ch, 6) == 0)			//创建文件命令create(filename)
		{
			cmd = CREATE;
			strcat(filename, ch + 7);
		}
		if (strncmp("delete", ch, 6) == 0)			//删除文件命令delete(filename)
		{
			cmd = DELETE;
			strcat(filename, ch + 7);
		}
		if (strncmp("open", ch, 4) == 0)			//打开文件命令open(filename)
		{
			cmd = OPEN;
			strcat(filename, ch + 5);
		}
		if (strncmp("close", ch, 5) == 0)			//关闭文件命令close(index)
		{
			cmd = CLOSE;
		}
		if (strncmp("read", ch, 4) == 0)			//读文件命令read(index)
		{
			cmd = READ;
		}
		if (strncmp("write", ch, 5) == 0)			//写文件命令write(index)
		{
			cmd = WRITE;
		}
		if (strncmp("lseek", ch, 5) == 0)			//指针命令lseek(index,pos)
		{
			cmd = LSEEK;
		}
		if (strncmp("oplist", ch, 6) == 0)			// 查看打开文件表
		{
			cmd = OPLIST;
		}
		if (strncmp("exit", ch, 4) == 0)			// 退出命令exit
		{
			cmd = EXIT;
			break;
		}
		if (strncmp("ldisk", ch, 5) == 0)			//查看硬2盘内容（辅助用）
		{
			cmd = LDISK;
		}
		if (strncmp("help", ch, 4) == 0)			//帮助命令help(non)
		{
			cmd = HELP;
		}
		int index, count, pos;
		switch (cmd)
		{
		case DIR:
			directory();
			printf("----------------------------------------------\n");
			break;
		case CREATE:
			if (create(filename) == OK)
				printf("创建文件成功\n");
			printf("----------------------------------------------\n");
			break;
		case DELETE:
			if (destroy(filename) == OK)
				printf("删除文件成功\n");
			printf("----------------------------------------------\n");
			break;
		case OPEN:
			if (open(filename) == OK)
				printf("打开文件成功\n");
			printf("----------------------------------------------\n");
			break;
		case CLOSE:
			if (show_openlist() == 0)
			{
				printf("当前没有文件被打开\n");
				printf("----------------------------------------------\n");
				break;
			}
			printf("请输入要关闭文件的索引号：o\n");
			scanf("%d", &index);
			if (close(index) == OK)
				printf("关闭操作成功\n");
			printf("----------------------------------------------\n");
			break;
		case READ:
			if (show_openlist() == 0)
			{
				printf("当前没有文件被打开\n");
				printf("----------------------------------------------\n");
				break;
			}
			printf("请输入要读取文件的索引号：o\n");
			scanf("%d", &index);
			printf("请输入想要要读取文件长度：o\n");
			scanf("%d", &count);
			if (read(index, 0, count) == OK)
				printf("读文件操作成功\n");
			printf("----------------------------------------------\n");
			break;
		case WRITE:
			if (show_openlist() == 0)
			{
				printf("当前没有文件被打开\n");
				printf("----------------------------------------------\n");
				break;
			}
			printf("请输入要写入文件的索引号：o\n");
			scanf("%d", &index);
			printf("请输入想要要写入文件长度：o\n");
			scanf("%d", &count);
			if (write(index, 0, count) == OK)
				printf("写入操作成功\n");
			printf("----------------------------------------------\n");
			break;
		case LSEEK:
			if (show_openlist() == 0)
			{
				printf("当前没有文件被打开\n");
				printf("----------------------------------------------\n");
				break;
			}
			printf("请输入要写入文件的索引号：o\n");
			scanf("%d", &index);
			printf("请输入想要要设置的文件相对位置\n");
			scanf("%d", &pos);
			lseek(index, pos);
			printf("----------------------------------------------\n");
			break;
		case OPLIST:
			if (show_openlist() == 0)
			{
				printf("\t\t\n当前没有文件被打开\n");
				printf("----------------------------------------------\n");
				break;
			}
			printf("----------------------------------------------\n");
			break;
		case HELP:
			show_help();
			break;
		case LDISK:
			show_ldisk();
			break;
		default:
			printf("指令错误\n");
			printf("----------------------------------------------\n");
			break;
		}
		fflush(stdin);
		Init_block(ch, CH_LENGTH);
	}

}

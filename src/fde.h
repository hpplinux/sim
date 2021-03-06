/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#ifndef UTIL_FDE_H
#define UTIL_FDE_H

#include <errno.h>
#include <vector>

#ifdef __linux__
	#define HAVE_EPOLL 1
#endif

#ifdef HAVE_EPOLL
	#include <sys/epoll.h>
#else
	#include <sys/select.h>
#endif

/*
#define FDEVENT_NONE	(0)
#define FDEVENT_IN		(1<<0)
#define FDEVENT_PRI		(1<<1)
#define FDEVENT_OUT		(1<<2)
#define FDEVENT_HUP		(1<<3)
#define FDEVENT_ERR		(1<<4)
*/

namespace sim{

/*
能static和const的一定要用.
这块为啥不用enum呢?
*/
const static int FDEVENT_NONE = (0);
const static int FDEVENT_IN   = (1<<0);
const static int FDEVENT_PRI  = (1<<1);
const static int FDEVENT_OUT  = (1<<2);
const static int FDEVENT_HUP  = (1<<3);
const static int FDEVENT_ERR  = (1<<4);

/** 为什么不把读写相关的处理函数也注册进来?  因为这俩函数是统一的，所以不用个性化的设置*/
struct Fdevent{
	int fd;
	int s_flags; // subscribed events
	int events;	 // ready events
	struct{//自己私有的变量，定义在自己内部，安全
		/**
		const static int DEFAULT_TYPE = 0;
		const static int HANDLER_TYPE = 1;
		num是这两个值之一
		*/
		int num;
		/**
		可以是Link、Handler 、Session的指针
		如果是HANDLER_TYPE的话，则是Handler；
		如果是DEFAULT_TYPE的话，则是Session
		*/
		void *ptr;
	}data;
	
	bool readable() const{
		return events & FDEVENT_IN;
	}
	bool writable() const{
		return events & FDEVENT_OUT;
	}
};

/**可以处理多个fd的事物，实际就是一个epoll的包装管理*/
class Fdevents{
	public:
		//可以用using来替代typedef
		typedef std::vector<struct Fdevent *> events_t;
	private:
#ifdef HAVE_EPOLL
		static const int MAX_FDS = 8 * 1024;
		int ep_fd;
		struct epoll_event ep_events[MAX_FDS];
#else
		int maxfd;
		fd_set readset;
		fd_set writeset;
#endif
		events_t events;
		events_t ready_events;//负责将某一次wait收集到的请求打包返回

		struct Fdevent *get_fde(int fd);
	public:
		Fdevents();
		~Fdevents();

		bool isset(int fd, int flag);
		int set(int fd, int flags, int data_num, void *data_ptr);
		int del(int fd);
		int clr(int fd, int flags);
		const events_t* wait(int timeout_ms=-1);
};


}; // namespace sim

#endif

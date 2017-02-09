/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include <unistd.h>
#include "fde.h"

namespace sim{

/* 返回fd对应的Fdevent ，如已经存在的话则直接返回；没有的话则一个个的创建上去，一直创建到fd这个。
这种方式会不会导致创建无用的 Fdevent 呢? 也就是说让while无效的多次执行。
考虑到在一个进程里面，文件描述符总是从最小的开始编号使用的，所以while执行的次数应该不是很大。
**/
struct Fdevent* Fdevents::get_fde(int fd){
	while((int)events.size() <= fd){
		struct Fdevent *fde = new Fdevent();
		fde->fd = events.size();
		fde->s_flags = FDEVENT_NONE;
		fde->data.num = 0;
		fde->data.ptr = NULL;//看看这个变量后面具体怎么用的
		events.push_back(fde);//emplace更高效
	}
	return events[fd];
}

}; // namespace sim


#ifdef HAVE_EPOLL
#include "fde_epoll.cpp"
#else
#include "fde_select.cpp"
#endif

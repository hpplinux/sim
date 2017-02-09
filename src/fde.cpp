/*
Copyright (c) 2012-2014 The SSDB Authors. All rights reserved.
Use of this source code is governed by a BSD-style license that can be
found in the LICENSE file.
*/
#include <unistd.h>
#include "fde.h"

namespace sim{

/* ����fd��Ӧ��Fdevent �����Ѿ����ڵĻ���ֱ�ӷ��أ�û�еĻ���һ�����Ĵ�����ȥ��һֱ������fd�����
���ַ�ʽ�᲻�ᵼ�´������õ� Fdevent ��? Ҳ����˵��while��Ч�Ķ��ִ�С�
���ǵ���һ���������棬�ļ����������Ǵ���С�Ŀ�ʼ���ʹ�õģ�����whileִ�еĴ���Ӧ�ò��Ǻܴ�
**/
struct Fdevent* Fdevents::get_fde(int fd){
	while((int)events.size() <= fd){
		struct Fdevent *fde = new Fdevent();
		fde->fd = events.size();
		fde->s_flags = FDEVENT_NONE;
		fde->data.num = 0;
		fde->data.ptr = NULL;//��������������������ô�õ�
		events.push_back(fde);//emplace����Ч
	}
	return events[fd];
}

}; // namespace sim


#ifdef HAVE_EPOLL
#include "fde_epoll.cpp"
#else
#include "fde_select.cpp"
#endif

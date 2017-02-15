#include "util/log.h"
#include "handler.h"
#include "util/thread.h"

namespace sim{

int Handler::m_init(){
	resps = new SelectableQueue<Response *>();
	return this->init();
}

int Handler::m_free(){//清理工作为什么不放在析构函数里面呢 ?
	delete resps;
	return this->free();
}

int Handler::fd(){
	return resps->fd();
}

HandlerState Handler::accept(const Session &sess){
	return HANDLE_OK;
}

HandlerState Handler::close(const Session &sess){
	return HANDLE_OK;
}

HandlerState Handler::proc(const Request &req, Response *resp){
	return HANDLE_OK;
}

/** 该函数仅仅在test中被用过*/
void Handler::async_send(Response *resp){
	this->resps->push(resp);
}

Response* Handler::handle(){
	while(this->resps->size() > 0){
		Response *resp;
		if(this->resps->pop(&resp) == 1 && resp != NULL){//取出一个item的数据
			return resp;
		}
	}
	return NULL;
}

}; // namespace sim

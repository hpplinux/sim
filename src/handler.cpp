#include "util/log.h"
#include "handler.h"
#include "util/thread.h"

namespace sim{

int Handler::m_init(){
	resps = new SelectableQueue<Response *>();
	return this->init();
}

int Handler::m_free(){//������Ϊʲô�������������������� ?
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

/** �ú���������test�б��ù�*/
void Handler::async_send(Response *resp){
	this->resps->push(resp);
}

Response* Handler::handle(){
	while(this->resps->size() > 0){
		Response *resp;
		if(this->resps->pop(&resp) == 1 && resp != NULL){//ȡ��һ��item������
			return resp;
		}
	}
	return NULL;
}

}; // namespace sim

#ifndef SIM_HANDLER_H_
#define SIM_HANDLER_H_

#include "link.h"
#include "message.h"

template <class T>
class SelectableQueue;

namespace sim{

class Session
{
public:
	int64_t id;//顺序自增变量
	Link *link;
	
	Session(){
		static int64_t inc = 0;
		this->id = inc ++;
		this->link = NULL;
	}
	~Session(){
	}
};

class Request{
public:
	Message msg;
	Session sess;

	double stime;
	double time_wait;
	double time_proc;
};

class Response{
public:
	Message msg;
	Session sess;
};


typedef enum{
	HANDLE_OK   = 0,
	HANDLE_FAIL = 1,
	HANDLE_RESP = 2,
}HandlerState;


class Handler
{
public:
	Handler(){};
	virtual ~Handler(){};

	// 褰撴湁鏂板鎴风杩涙潵鏃�, 姝ゆ柟娉曡璋冪敤. 濡傛灉杩斿洖 HANDLE_FAIL, 杩炴帴灏嗚鍏抽棴.
	virtual HandlerState accept(const Session &sess);
	// 褰撳鎴风琚叧闂椂, 姝ゆ柟娉曡璋冪敤.
	virtual HandlerState close(const Session &sess);
	
	// 褰撴敹鍒板鎴风鐨勪竴涓姹傛姤鏂囨椂, 璋冪敤姝ゆ柟娉�.
	// 濡傛灉鏈夊搷搴旈渶瑕佺珛鍗宠繑鍥炵粰瀹㈡埛绔�, 灏嗗搷搴斿姞鍒� resp 涓�, 骞惰繑鍥� HANDLE_RESP;
	virtual HandlerState proc(const Request &req, Response *resp);

	virtual int init(){ return 0; }
	virtual int free(){ return 0; }
	//virtual void thread();
	
	/***** 浠ヤ笅鏄壒娈婃柟娉�, 浣犱竴鑸笉闇�瑕佸叧蹇�. *****/
	
	// 姝ゆ柟娉曢粯璁よ繑鍥炲紓姝ュ搷搴旈槦鍒楃殑 fd, 浣犲彲浠ラ噸鍐欐鏂规硶, 杩斿洖浣犺嚜宸辩殑 fd.
	virtual int fd();
	
	// 褰� fd() 鏈夊彲璇讳簨浠舵椂, 鏈嚱鏁拌璋冪敤.
	// 濡傛灉姝ゆ柟娉曟湁鍝嶅簲闇�瑕佺珛鍗宠繑鍥�, 璇疯繑鍥� Response 瀹炰緥, 澶栭潰浼氳礋璐ｉ噴鏀惧唴瀛�.
	// 濡傛棤鍝嶅簲, 杩斿洖 NULL.
	virtual Response* handle();
	
protected:
	// 灏嗗紓姝ュ搷搴斿姞鍏ュ埌闃熷垪涓�, 璇ュ搷搴斾細琚彂閫佺粰瀹㈡埛绔�.
	// 濡傛灉 Handler 鏄绾跨▼鐨�, 鍙互浼氳皟鐢ㄦ湰鏂规硶灏嗗搷搴斿彂缁欏鎴风.
	void async_send(Response *resp);
	
	HandlerState ok(){ return HANDLE_OK; };
	HandlerState fail(){ return HANDLE_FAIL; };
	HandlerState resp(){ return HANDLE_RESP; };

private:
	SelectableQueue<Response *> *resps;
	
	int m_init();
	int m_free();
	friend class Server;
};

}; // namespace sim

#endif

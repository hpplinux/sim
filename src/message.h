#ifndef SIM_MESSAGE_H_
#define SIM_MESSAGE_H_

#include <stdint.h>
#include <inttypes.h>
#include <string>
#include <map>

namespace sim{

class Message{
public:
	Message();
	~Message();
	
	void reset();
	
	std::string type() const;
	void set_type(const std::string &type);
	
	void add(const std::string &val); // è‡ªå¢ tag, ä» 0 å¼€å§‹
	void set(int tag, int32_t val);
	void set(int tag, int64_t val);
	void set(int tag, const char *val);
	void set(int tag, const std::string &val);
	const std::string* get(int tag) const;
	
	std::string encode() const;//Ö»ÓĞencode£¬Ã»ÓĞ¶ÔÓ¦µÄdecode£¬ÈÃÈË¿´ÆğÀ´ºÜ¹îÒì°¡
	const std::map<int, std::string>* fields() const{
		return &fields_;
	}

private:
	std::string type_;
	std::map<int, std::string> fields_;
	friend class Decoder;
};

}; // namespace sim

#endif

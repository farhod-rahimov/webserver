#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>

class Client {
	public:
			Client();
			virtual ~Client();

			Client(const Client & src);
			
			Client & operator = (const Client & src);

			// REQUEST | REQUEST | REQUEST | REQUEST | REQUEST | REQUEST | REQUEST
			
			void ReqSetMethod(std::string);
			std::string & ReqGetMethod(void);
			
			void ReqSetPath(std::string);
			std::string & ReqGetPath(void);
			
			void ReqSetProtocol(std::string);
			std::string & ReqGetProtocol(void);

			void ReqSetHost(std::string);
			std::string & ReqGetHost(void);
			
			void ReqSetConnection(std::string);
			std::string & ReqGetConnection(void);

			void ReqSetContentFileName(std::string);
			std::string & ReqGetContentFileName(void);

			void ReqSetContentType(std::string);
			std::string & ReqGetContentType(void);
			
			void ReqSetContentLength(size_t);
			size_t ReqGetContentLength(void);
			
			void ReqSetContent(std::string);
			std::string & ReqGetContent(void);

			void ReqSetReqNum(size_t);
			size_t ReqGetReqNum(void);

			// RESPONSE | RESPONSE | RESPONSE | RESPONSE | RESPONSE | RESPONSE | RESPONSE

			void RespSetProtocol(std::string);
			std::string & RespGetProtocol(void);
			
			void RespSetStatusCode(std::string);
			std::string & RespGetStatusCode(void);
			
			void RespSetStatusTxt(std::string);
			std::string & RespGetStatusTxt(void);
			
			void RespSetContentType(std::string);
			std::string & RespGetContentType(void);
			
			void RespSetContentLength(size_t);
			std::string & RespGetContentLength(void);
			
			void RespSetContent(std::string);
			std::string & RespGetContent(void);

			void RespSetRespNum(size_t);
			size_t RespGetRespNum(void);

			std::string & RespCreateFullRespTxt(void);
			std::string & RespGetFullRespTxt(void);
			
			int RespGetRemainedToSent(void);
			void RespSetRemainedToSent(int);

			void RespSetConnection(std::string);
			std::string & RespGetConnection(void);

			void RespSetLocation(std::string);
			std::string & RespGetLocation(void);

			// COMMON	|	COMMON	|	COMMON	|	COMMON	|	COMMON	|	COMMON
			
			std::string & getBuff(void);

	private:

			// COMMON	|	COMMON	|	COMMON	|	COMMON	|	COMMON	|	COMMON

			std::string	_buff;

			// REQUEST	|	REQUEST	|	REQUEST	|	REQUEST	|	REQUEST	|	REQUEST
			
			std::string _req_method;//
			
			std::string _req_path;//
			std::string _req_protocol;//
			std::string _req_host;//
			std::string _req_connection;//
			
			std::string _req_content_file_name;//
			std::string _req_content_type;//
			size_t		_req_content_length;//
			std::string _req_content;//

			// RESPONSE	|	RESPONSE	|	RESPONSE	|	RESPONSE	|	RESPONSE

			std::string _resp_protocol;//
			std::string _resp_status_code;//
			std::string _resp_status_txt;//
			
			std::string _resp_connection;//
			std::string _resp_location;//
			
			std::string _resp_content_type;//
			std::string	_resp_content_length;//
			std::string _resp_content;//

			std::string _resp_full_resp_txt;//
			int			_resp_remained_to_sent;//
};

#endif
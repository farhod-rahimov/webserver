#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <vector>
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
			
			void ReqSetConnection(std::string);
			std::string & ReqGetConnection(void);
			
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
			
			// void RespSetContentLength(size_t);
			// size_t RespGetContentLength(void);

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


	private:

			// COMMON	|	COMMON	|	COMMON	|	COMMON	|	COMMON	|	COMMON



			// REQUEST	|	REQUEST	|	REQUEST	|	REQUEST	|	REQUEST	|	REQUEST
			
			std::string _req_method;
			
			std::string _req_path;
			std::string _req_protocol;
			std::string _req_connection;
			
			std::string _req_content_type;
			size_t		_req_content_length;
			std::string _req_content;

			size_t		_req_num;
			
			// RESPONSE	|	RESPONSE	|	RESPONSE	|	RESPONSE	|	RESPONSE

			std::string _resp_protocol;
			std::string _resp_status_code;
			std::string _resp_status_txt;
			
			std::string _resp_content_type;
			// size_t		_resp_content_length;
			std::string	_resp_content_length;
			std::string _resp_content;

			std::string _resp_full_resp_txt;
			// int			_resp_full_resp_length;
			int			_resp_remained_to_sent;
			
			size_t		_resp_num;
};

// std::string & operator << (std::string & full_response, Response & response) {
// 	full_response.clear();
// 	full_response.append(response.getProtoVer()); full_response.append(" ");
// 	full_response.append(response.getStatusCode()); full_response.append(" ");
// 	full_response.append(response.getStatusText()); full_response.append("\n");
// 	full_response.append(response.getConnection()); full_response.append("\n");
// 	full_response.append(response.getContentType()); full_response.append("\n");
// 	full_response.append(response.getContentLength()); full_response.append("\n\n");
	
// 	full_response.append(response.getContent()); full_response.append("\n");
// 	return (full_response);
// }

#endif
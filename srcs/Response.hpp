#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <string>

class Response {
	public:
		Response();
		virtual ~Response();

		Response(const Response & src);
		
		Response & operator = (const Response & src);

		void setProtoVer(std::string);
		void setStatusCode(std::string);
		void setStatusText(std::string);
		void setConnection(std::string);
		void setContentType(std::string);
		void setContentLength(std::string);
		void setContent(std::string);

		std::string & getProtoVer(void);
		std::string & getStatusCode(void);
		std::string & getStatusText(void);
		std::string & getConnection(void);
		std::string & getContentType(void);
		std::string & getContentLength(void);
		std::string & getContent(void);

	private:
		std::string _protocol_version;
		
		std::string _status_code;
		std::string _status_text;
		
		std::string _connection;
		
		std::string _content_type;
		std::string _content_length;
		std::string _content;
};

#endif
#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>

class Request {
	public:
		Request();
		Request(std::string method, std::string path, std::string proto_v);
		virtual ~Request();
		
		Request(const Request & src);
		
		Request & operator = (const Request & src);

		void setMethod(std::string);
		void setPath(std::string);
		void setProtoVer(std::string);
		void setConnection(std::string);
		void setContentType(std::string);
		void setContentLength(std::string);
		void setContent(std::string);
		
		std::string & getMethod(void);
		std::string & getPath(void);
		std::string & getProtoVer(void);
		std::string & getConnection(void);
		std::string & getContentType(void);
		std::string & getContentLength(void);
		std::string & getContent(void);

	private:
		std::string _method;				// GET POST DELETE
		std::string _path;					// /about or somethig else
		std::string _protocol_version;		// ONLY HTTP/1.1

		std::string _connection;
		
		std::string _content_type;
		std::string _content_length;
		std::string _content;
};

#endif
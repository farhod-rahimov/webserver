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

		void setRequest(Request);
		void setRequestNum(size_t);
		void setResponse(Response);
		void setResponseNum(size_t);


		std::vector<Request> &	getRequest(void);
		size_t					getRequestNum(void);
		std::vector<Response> & getResponse(void);
		size_t					getResponseNum(void);

	private:
		std::vector<Request>		_request;
		size_t                      _request_num;
		
		std::vector<Response>		_response;
		size_t                      _response_num;
};

std::string & operator << (std::string full_response, Response & response) {
	full_response.clear();
	full_response.append(response.getProtoVer()); full_response.append(" ");
	full_response.append(response.getStatusCode()); full_response.append(" ");
	full_response.append(response.getStatusText()); full_response.append("\n");
	full_response.append(response.getConnection()); full_response.append("\n");
	full_response.append(response.getContentType()); full_response.append("\n");
	full_response.append(response.getContentLength()); full_response.append("\n\n");
	
	full_response.append(response.getContent()); full_response.append("\n");
	return (full_response);
}

#endif
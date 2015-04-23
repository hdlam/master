/*
 * communicator.h
 *
 *  Created on: 8 Apr 2014
 *      Author: erik
 */

#ifndef COMMUNICATOR_H_
#define COMMUNICATOR_H_

#include <boost/asio.hpp>
#include <vector>

namespace network {

template <typename T>
class Communicator {
public:
	Communicator() {
		socket = NULL;
	}
	virtual ~Communicator() {

	}

	void connect(std::string host, std::string port) {
		using boost::asio::ip::udp;
			try
			{
				boost::asio::io_service io_service;

				udp::resolver resolver(io_service);
				udp::resolver::query query(udp::v4(), host, port);
				receiver_endpoint = *resolver.resolve(query);

				socket = new udp::socket(io_service);
				socket->open(udp::v4());
			}
			catch (std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
	};

	void send(std::vector<T> data) {
		using boost::asio::ip::udp;

			try {
				socket->send_to(boost::asio::buffer(data), receiver_endpoint);

			} catch (std::exception &e)  {
				std::cerr << e.what() << std::endl;
			}
	};
	
	void sendAscii(std::vector<T> data) {
		using boost::asio::ip::udp;
			std::string string = ""; 
			for ( T element : data)
			{
				std::stringstream tmpId ; 
				std::stringstream tmpX ; 
				std::stringstream tmpY ; 
				std::stringstream tmpR ; 
				tmpId << element.getId();
				tmpX << element.getPosition().x;
				tmpY << element.getPosition().y;
				tmpR << element.getRotation();
				string += "X" + tmpId.str() + ":" + tmpX.str() + ";Y" + tmpId.str() + ":" + tmpY.str() + ";R" + tmpId.str() + ":" + tmpR.str()+ "|" ;
			}

			try {
				socket->send_to(boost::asio::buffer(string), receiver_endpoint);
			} catch (std::exception &e)  {
				std::cerr << e.what() << std::endl;
			}
	};

	void disconnect() {
		// todo: figure out why this doesn't work (call never returns)
		//socket->close();
		//delete socket;
	}

	void listen(void (*messageHandler)(std::vector<T> data), short port) {
		using boost::asio::ip::udp;
			try
			{
				boost::asio::io_service io_service;

				udp::socket socket(io_service, udp::endpoint(udp::v4(), port));

				for (;;)
				{
					std::array<T, 1024> recv_buf;
					udp::endpoint remote_endpoint;
					boost::system::error_code error;
					size_t len = socket.receive_from(boost::asio::buffer(recv_buf),
							remote_endpoint, 0, error);

					if (error && error != boost::asio::error::message_size)
						throw boost::system::system_error(error);

					std::vector<T> data(recv_buf.begin(), recv_buf.begin()+len/sizeof(T));
					messageHandler(data);
				}
			}
			catch (std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
	};

	void listenAscii(void (*messageHandler)(std::string received), short port) {
		using boost::asio::ip::udp;
			try
			{
				boost::asio::io_service io_service;

				udp::socket socket(io_service, udp::endpoint(udp::v4(), port));

				for (;;)
				{
					std::array<char, 2048> recv_buf;
					udp::endpoint remote_endpoint;
					boost::system::error_code error;
					size_t len = socket.receive_from(boost::asio::buffer(recv_buf),
							remote_endpoint, 0, error);

					if (error && error != boost::asio::error::message_size)
						throw boost::system::system_error(error);

					std::string received(recv_buf.begin(), recv_buf.begin()+len);
					messageHandler(received);
				}
			}
			catch (std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
	};

private:
	boost::asio::ip::udp::socket* socket;
	boost::asio::ip::udp::endpoint receiver_endpoint;
};

} /* namespace network */


#endif /* COMMUNICATOR_H_ */

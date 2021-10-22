#define _CRT_SECURE_NO_WARNINGS
#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp; //라이브러리를 그대로 사용
using namespace std;

//서버 컴퓨터의 날짜 및 시간 정보를 반환
string make_daytime_string() {
	time_t now = time(0); //초기화
	return ctime(&now);
}

int main() {
	//80번:http 13번:date
	try {
		//Boost.Asio 프로그램은 하나의 IO Service 객체를 가짐
		boost::asio::io_service io_service; 
		// TCP 프로토콜의 13번 포트로 연결을 받는 수동소켓을 생성
		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 13)); //주소 객체 사용
		//모든 클라이언트에 대해 무한정 반복 수행
		while (1) {
			//소켓 객체를 생성해 연결을 대기
			tcp::socket socket(io_service);
			acceptor.accept(socket);
			//연결이 완료되면 해당되는 클라이언트에게 메세지를 생성
			string message = make_daytime_string();
			//해당 클라이언트에게 메시지를 담아 전송
			boost::system::error_code ignored_error;
			boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
		}
	} catch (exception& e) {
			cerr << e.what() << '\n';
	}
	return 0;
}

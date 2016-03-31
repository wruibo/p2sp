#include "cube/service/tcp/tcp_handler.h"

BEGIN_SERVICE_NS
tcp_handler::tcp_handler(){

}

tcp_handler::~tcp_handler(){

}

int tcp_handler::on_open(void* arg) {
	return -1;
}

int tcp_handler::on_send(unsigned int sz) {
	return -1;
}

int tcp_handler::on_recv(const void* data, unsigned int sz) {
	return -1;
}

int tcp_handler::on_timeout(time_t tm) {
	return -1;
}

int tcp_handler::on_running(time_t tm) {
	return -1;
}

int tcp_handler::on_close(int err) {
	return -1;
}

void tcp_handler::sock(socket_t s){
	_session.sock(s);
}

socket_t tcp_handler::sock(){
	return _session.sock();
}

void tcp_handler::remote_ip(unsigned int ip){
	_session.remote_ip(ip);
}

unsigned int tcp_handler::remote_ip(){
	return _session.remote_ip();
}

void tcp_handler::remote_port(unsigned short port){
	_session.remote_port(port);
}

unsigned short tcp_handler::remote_port(){
	return _session.remote_port();
}

void tcp_handler::set_timer(int delay_seconds){
	_timer.set(delay_seconds);
}

bool tcp_handler::is_timeout(time_t now){
	return _timer.is_timeout(now);
}

int tcp_handler::send(const void* data, int sz){
	return _session.send(data, sz);
}

int tcp_handler::recv(int sz){
	return _session.recv(sz);
}

bool tcp_handler::redo_send(unsigned int *send_sz) {
	return _session.redo_send(send_sz);
}

bool tcp_handler::redo_recv(void** data, unsigned int *recv_sz) {
	return _session.redo_recv(data, recv_sz);
}
END_SERVICE_NS

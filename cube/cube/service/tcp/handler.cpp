#include "cube/service/tcp/handler.h"

BEGIN_SERVICE_TCP_NS
handler::handler(){

}

handler::~handler(){

}

int handler::on_open(void* arg) {
	return -1;
}

int handler::on_send(unsigned int sz) {
	return -1;
}

int handler::on_recv(const void* data, unsigned int sz) {
	return -1;
}

int handler::on_timeout(time_t tm) {
	return -1;
}

int handler::on_running(time_t tm) {
	return -1;
}

int handler::on_close(int err) {
	return -1;
}

void handler::sock(SOCKET s){
	_session.sock(s);
}

SOCKET handler::sock(){
	return _session.sock();
}

void handler::remote_ip(unsigned int ip){
	_session.remote_ip(ip);
}

unsigned int handler::remote_ip(){
	return _session.remote_ip();
}

void handler::remote_port(unsigned short port){
	_session.remote_port(port);
}

unsigned short handler::remote_port(){
	return _session.remote_port();
}

void handler::set_timer(int delay_seconds){
	_timer.set(delay_seconds);
}

bool handler::is_timeout(time_t now){
	return _timer.is_timeout(now);
}

int handler::send(const void* data, int sz){
	return _session.send(data, sz);
}

int handler::recv(int sz){
	return _session.recv(sz);
}

bool handler::redo_send(unsigned int *send_sz) {
	return _session.redo_send(send_sz);
}

bool handler::redo_recv(void** data, unsigned int *recv_sz) {
	return _session.redo_recv(data, recv_sz);
}
END_SERVICE_TCP_NS
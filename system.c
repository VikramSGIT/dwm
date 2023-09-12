#include "system.h"

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

int clientSocket;
char buffer[64];

void init() {
	batcap_file = fopen(batcap, "r");
	batstate_file = fopen(batstate, "r");
	mem_file = fopen(mem, "r");

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket == -1) return;

	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	if(inet_pton(AF_INET, SERVER_IP, &serverAddress.sin_addr) <= 0) {
		close(clientSocket);
		clientSocket = -1;
		return;
	}

	if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
		close(clientSocket);
		clientSocket = -1;
		return;
	}
}

int cmd_server() {
	if(clientSocket == -1) {
	       	buffer[0] = '\0';
		return 0;
	}
	send(clientSocket, buffer, sizeof(buffer), 0);
	int bytesRead = recv(clientSocket, buffer, sizeof(client_buffer), 0);
	if(bytesRead > 0) {
		buffer[bytesRead] = '\0';
	} else {
		buffer[0] = '\0';
	}
	return bytesRead;
}

int toggle_cb() {
	strcpy(buffer, "toggle-cooler-boost");
	int len = cmd_server();
	if(len <= 0) return -1; 
	return atoi(buffer);
}

int get_cb() {
	strcpy(buffer, "get-cooler-boost");
	int len = cmd_server();
	if(len <= 0) return -1;
	return atoi(buffer);
}

int get_vol() {
	int res;
	FILE* pipe = popen(vol, "r");
	char buffer[8];
	if(pipe && fgets(buffer, sizeof(buffer), pipe) != NULL) {
		size_t len = strlen(buffer);
		buffer[len - 1] = '\0';
		res = atoi(buffer);
		pclose(pipe);
	}
	return res;
}

int inc_vol() {
	int res;
	FILE* pipe = popen(incvol, "r");
	char buffer[8];
	if(pipe && fgets(buffer, sizeof(buffer), pipe) != NULL) {
		size_t len = strlen(buffer);
		buffer[len - 1] = '\0';
		res = atoi(buffer);
		pclose(pipe);
	}
	return res;
}

int dec_vol() {
	int res;
	FILE* pipe = popen(decvol, "r");
	char buffer[8];
	if(pipe && fgets(buffer, sizeof(buffer), pipe) != NULL) {
		size_t len = strlen(buffer);
		buffer[len - 1] = '\0';
		res = atoi(buffer);
		pclose(pipe);
	}
	return res;
}

int toggle_mute() {
	int res;
	FILE* pipe = popen(togglemute, "r");
	char buffer[8];
	if(pipe && fgets(buffer, sizeof(buffer), pipe) != NULL) {
		size_t len = strlen(buffer);
		buffer[len - 1] = '\0';
		res = atoi(buffer);
		pclose(pipe);
	}
	return res;
}
int get_mute() {
	int res;
	FILE* pipe = popen(getmute, "r");
	char buffer[8];
	if(pipe && fgets(buffer, sizeof(buffer), pipe) != NULL) {
		size_t len = strlen(buffer);
		buffer[len - 1] = '\0';
		res = atoi(buffer);
		pclose(pipe);
	}
	return res;
}
int get_bright() {
	int res;
	FILE* pipe = popen(bright, "r");
	char buffer[8];
	if(pipe && fgets(buffer, sizeof(buffer), pipe) != NULL) {
		size_t len = strlen(buffer);
		buffer[len - 1] = '\0';
		res = atoi(buffer);
		pclose(pipe);
	}
	return res;
}

int inc_bright() {
	int res;
	FILE* pipe = popen(incbright, "r");
	char buffer[8];
	if(pipe && fgets(buffer, sizeof(buffer), pipe) != NULL) {
		size_t len = strlen(buffer);
		buffer[len - 1] = '\0';
		res = atoi(buffer);
		pclose(pipe);
	}
	return res;
}

int dec_bright() {
	int res;
	FILE* pipe = popen(decbright, "r");
	char buffer[8];
	if(pipe && fgets(buffer, sizeof(buffer), pipe) != NULL) {
		size_t len = strlen(buffer);
		buffer[len - 1] = '\0';
		res = atoi(buffer);
		pclose(pipe);
	}
	return res;
}

void get_status() {
	int cur = 0;
// CPU GPU
	{
		strcpy(buffer, "get-stats");
		int len = cmd_server(), count = 0, cur_pos = 0;
		char temp[8];
		for(int i = 0; i < len; i++){
			if(buffer[i] == ' '){
				count++;
				if(count == 1){ // CPU
					temp[cur_pos] = '\0';
					cur += sprintf(client_buffer + cur, " | CPU: %s °C", temp);
					cur_pos = 0;
				}
				else if(count == 2){ // GPU
					temp[cur_pos] = '\0';
					cur += sprintf(client_buffer + cur, " | GPU: %s °C", temp);
					cur_pos = 0;
					break;
				}
			}
			temp[cur_pos++] = buffer[i];
		}
	}
// MEMORY
	{
		size_t line_count = 0;
		float total, avail;
		char line[64];
		fseek(mem_file, 0, SEEK_SET);
		while(fgets(line, sizeof(line), mem_file) != NULL) {
			line_count++;
			if(line_count == 1) {
				size_t line_size = strlen(line);
				const char startstr[]= "MemTotal:      ";
				const char endstr[]=" kB";
				line[line_size - (sizeof(endstr) - 1)] = '\0';
				strcpy(line, line + sizeof(startstr));
				total = atof(line);
			}
			if(line_count == 3) {
				size_t line_size = strlen(line);
				const char startstr[]= "MemAvailable:  ";
				const char endstr[]=" kB";
				line[line_size - (sizeof(endstr) - 1)] = '\0';
				strcpy(line, line + sizeof(startstr));
				avail = atof(line);
				break;
			}
		}
		cur += sprintf(client_buffer + cur, " | MEM: %.2f/%.2f", (total - avail)/1048576,total/1048576);
	}
// NETWORK
	{
		FILE* pipe = popen(netstatus, "r");
		char buffer[16];
		if(pipe && fgets(buffer, sizeof(buffer), pipe) != NULL) {
			size_t len = strlen(buffer);
			buffer[len - 1] = '\0';
			cur += sprintf(client_buffer + cur, " | NET: %s", buffer);
			pclose(pipe);
		}
	}
// BATTERY
	{
		char line[8];
		fseek(batcap_file, 0, SEEK_SET);
		while(fgets(line, sizeof(line), batcap_file )){
			size_t len = strlen(line);
			line[len - 1] = '\0';
			cur += sprintf(client_buffer + cur, " | BAT: %s", line);
		}
		fseek(batstate_file, 0,SEEK_SET);
		while(fgets(line, sizeof(line), batstate_file)){
			size_t len = strlen(line);
			line[len - 1] = '\0';
			cur += sprintf(client_buffer + cur, " %s", line);
		}
	}
// TIME
	
	time_t now = time(NULL);
	struct tm *ltm = localtime(&now);

        char date[18];
        strftime(date, 18, "%H:%M %a, %b %e", ltm);
	cur += sprintf(client_buffer + cur, " | %s \"", date);
}

void deinit() {
	if(clientSocket != -1) {
		close(clientSocket);
		clientSocket = -1;
	}
	if(batcap_file != NULL)	fclose(batcap_file);
	if(batstate_file != NULL) fclose(batstate_file);
	if(mem_file != NULL) fclose(mem_file);
}

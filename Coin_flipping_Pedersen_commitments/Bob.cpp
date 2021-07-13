#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <gmp.h>
#include <fstream>
#include "pedersen.h"

#define PORT 8080
#define MAX_SIZE 500000

int sock = 0, valread;
struct sockaddr_in serv_addr;
char output[MAX_SIZE]{0};
char inp[MAX_SIZE]{0};

int connection(){
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		printf("\nConnection Failed \n");
		return -1;
	}

	return 0;
}

int main(int argc, char const *argv[]){

	mpz_t p, q, g, h, A;
    mpz_init(p);
    mpz_init(q);
    mpz_init(g);
    mpz_init(h);
    mpz_init(A);

	char *ans = "1";
	char *mess = "1";
	char lis[10] = {0};
	fstream fin, fout;
	fin.open("Bob.txt", ios::in);
	fin.close();

	int err = connection();
	if(err == -1){
        return -1;
	}

	setup(p, q, g, h, A);

	cout<<"\n================================ PUBLIC ================================\n";
	fout.open("Alice.txt", ios::out | ios:: trunc);

	mpz_get_str(output, 10, p);
	fout<<output;
	fout<<"\n";

	mpz_get_str(output, 10, q);
	fout<<output;
	fout<<"\n";

	mpz_get_str(output, 10, g);
	fout<<output;
	fout<<"\n";

	mpz_get_str(output, 10, h);
	fout<<output;
	fout<<"\n";

	fout.close();
	send(sock , mess , strlen(mess), 0);
	printf("Message sent!\n");

	cout<<"\n================================ COM ================================\n";
	mpz_t com, r, x;
    mpz_init(com);
    mpz_init(r);
    mpz_init(x);
    bzero(lis, 10);
	valread = read(sock , lis, 10);

	fin.open("Bob.txt", ios::in);
	fin.getline(inp, MAX_SIZE);
	mpz_set_str(com, inp, 10);
	gmp_printf("com = %Zd\n", com);
	fin.close();

	cout<<"\n================================ ANSWER ================================\n";
	cout<<"Answer: "<<ans<<"\n";
	send(sock, ans, strlen(ans), 0);
	printf("Answer sent!\n");

	cout<<"\n================================ VERIFY ================================\n";
	bzero(lis, 10);
	valread = read(sock , lis, 10);

	fin.open("Bob.txt", ios::in);

	fin.getline(inp, MAX_SIZE);
	mpz_set_str(x, inp, 10);

	fin.getline(inp, MAX_SIZE);
	mpz_set_str(r, inp, 10);

	gmp_printf("\nx = %Zd\n\n", x);
    gmp_printf("r = %Zd\n\n", r);
    fin.close();

    bool res = verify(com, p, g, h, x, r);

    if(res){
        cout<<"Result: True\n\n";
    }
    else{
        cout<<"Result: False\n\n";
    }

	mpz_clears(p, q, g, h, A, com, r, x, NULL);

	return 0;
}


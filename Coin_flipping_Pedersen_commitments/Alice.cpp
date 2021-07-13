#include <unistd.h>
#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <gmp.h>
#include "pedersen.h"

#define PORT 8080
#define MAX_SIZE 500000

int server_fd, new_socket, valread;
struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);
char output[MAX_SIZE]{0};
char inp[MAX_SIZE]{0};

void connection(){
    // Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char const *argv[]){

	mpz_t p, q, g, h;
    mpz_init(p);
    mpz_init(q);
    mpz_init(g);
    mpz_init(h);

	char ans[10] ={0};
	char *mess = "1";
	fstream fin, fout;
	fin.open("Alice.txt", ios::in);
	fin.close();

	connection();

	bzero(inp, MAX_SIZE);
	valread = read(new_socket , ans, 10);

	fin.open("Alice.txt", ios::in);

	fin.getline(inp, MAX_SIZE);
	mpz_set_str(p, inp, 10);

	fin.getline(inp, MAX_SIZE);
	mpz_set_str(q, inp, 10);

	fin.getline(inp, MAX_SIZE);
	mpz_set_str(g, inp, 10);

	fin.getline(inp, MAX_SIZE);
	mpz_set_str(h, inp, 10);

	fin.close();

	cout<<"\n================================ PUBLIC ================================\n";
	cout<<"p = ";
    gmp_printf("%Zd\n\n", p);
    cout<<"q = ";
    gmp_printf("%Zd\n\n", q);
    cout<<"g = ";
    gmp_printf("%Zd\n\n", g);
    cout<<"h = ";
    gmp_printf("%Zd\n", h);


	mpz_t com, r, x;
    mpz_init(com);
    mpz_init(r);
    mpz_init(x);

    cout<<"\n================================ COMMIT ================================\n";
    cout<<"Enter value of x (belong to Zq): ";
    //cin.ignore();
    cin.getline(inp, MAX_SIZE);
    mpz_set_str(x, inp, 10);

    commit(com, p, q, g, h, x, r);
    gmp_printf("\nr = %Zd\n\n", r);
    gmp_printf("com = %Zd\n\n", com);

    mpz_get_str(output, 10, com);
    fout.open("Bob.txt", ios::out | ios:: trunc);
    fout<<output;
    fout<<"\n";
    fout.close();

	send(new_socket, mess, strlen(mess), 0);
	printf("Message sent!\n");

	cout<<"\n================================ Bob ================================\n";
	bzero(ans, 10);
	valread = read(new_socket , ans, 10);
	cout<<"Answer from Bob: "<<ans<<"\n";

	cout<<"\n================================ OPEN ================================\n";
	fout.open("Bob.txt", ios::out | ios:: trunc);
	mpz_get_str(output, 10, x);
	fout<<output;
    fout<<"\n";

	mpz_get_str(output, 10, r);
	fout<<output;
    fout<<"\n";

	fout.close();
	send(new_socket, mess, strlen(mess), 0);
	printf("Message sent!\n\n");

    mpz_clears(p, q, g, h, com, r, x, NULL);

	return 0;
}


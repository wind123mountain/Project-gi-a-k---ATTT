#include <bits/stdc++.h>
#include <gmp.h>

using namespace std;

#define MAX 1000005
#define nbyte 384
#define nbyte_720 90

unsigned char input[MAX];
unsigned char Hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
int temp = 0;
char prime[2*nbyte+1];
char prime_720[2*nbyte_720+1];

unsigned long int primes_1e4[1230];
int size_primes_1e4 = 0;

void setup_primes_1e4(){
    primes_1e4[0] = 3;
	primes_1e4[1] = 5;
	primes_1e4[2] = 7;
	size_primes_1e4 = 3;
	for(int i = 2; i != 1667; i++){
		int temp1 = 6*i - 1;
		int temp2 = 6*i +1;
		bool check = true;
		for(int j = 0; j != size_primes_1e4; j++){
			if(temp1%primes_1e4[j] == 0){
				check = false;
				break;
			}
		}
		if(check){
			primes_1e4[size_primes_1e4] = temp1;
			size_primes_1e4++;
		}

		check = true;
		for(int j = 0; j != size_primes_1e4; j++){
			if(temp2%primes_1e4[j] == 0){
				check = false;
				break;
			}
		}
		if(check){
			primes_1e4[size_primes_1e4] = temp2;
			size_primes_1e4++;
		}
	}
}

 void genInt_720(mpz_t &x){
    int n = nbyte_720 - 1;
    char c = Hex[(input[temp] >> 4) | 8];
    prime_720[0] = c;
    c = Hex[input[temp] & 15];
    prime_720[1] = c;
    for(int i = 1; i != n; i++){
        prime_720[2*i] = Hex[(int)input[i+temp] >> 4];
        prime_720[2*i + 1] = Hex[(int)input[i+temp] & 15];
    }
    prime_720[2*nbyte_720 - 2] = Hex[(int)input[n+temp] >> 4];
    prime_720[2*nbyte_720 - 1] = Hex[((int)input[n+temp] & 15) | 1];
    temp += nbyte_720;

    mpz_set_str(x, prime_720, 16);
}

void genInt_3072(mpz_t &x){
    int n = nbyte - 1;
    char c = Hex[(input[temp] >> 4) | 8];
    prime[0] = c;
    c = Hex[input[temp] & 15];
    prime[1] = c;
    for(int i = 1; i != n; i++){
        prime[2*i] = Hex[(int)input[i+temp] >> 4];
        prime[2*i + 1] = Hex[(int)input[i+temp] & 15];
    }
    prime[2*nbyte - 2] = Hex[(int)input[n+temp] >> 4];
    prime[2*nbyte - 1] = Hex[((int)input[n+temp] & 15) | 1];
    temp += nbyte;

    mpz_set_str(x, prime, 16);
}


mpz_t res, x_1, a, u, x_i, x_i_1, res_check, check_odd;

bool checkPrime(mpz_t &x) {
    mpz_sub_ui(x_1, x, 1U);
    mpz_set_ui(a, 2U);

    for(int i = 0; i != size_primes_1e4; i++){
        mpz_mod_ui(res, x, primes_1e4[i]);
        if(mpz_get_ui(res) == 0){
            return false;
        }
    }

    //checkFermat
    mpz_powm(res, a, x_1, x);
    if(mpz_get_ui(res) != 1){
        return false;
    }

    //Check Rabin-Miller

    //tim u, t
    int t = 0;
    mpz_set_ui(check_odd, 1U);
    mpz_set(u, x_1);
    mpz_and(res_check, x_1, check_odd);
    while(mpz_get_ui(res_check) == 0){
        t++;
        mpz_div_2exp(u, u, 1U);
        mpz_and(res_check, u, check_odd);
    }

    //Rabin-Miller
    srand(time(NULL));
    unsigned long long a_u;
    for(int i = 1; i != 65; i++){
        if(i < 17){
            mpz_set_ui(a, primes_1e4[i]);
        }
        else{
            a_u = rand() | 64;
            mpz_set_ui(a, a_u);
        }

        mpz_powm(x_i_1, a, u, x);
        for(int j = 0; j != t; j++){
            mpz_powm_ui(x_i, x_i_1, 2U, x);
            if(mpz_get_ui(x_i) == 1 && mpz_get_ui(x_i_1) != 1 && mpz_cmp(x_i_1, x_1) != 0){
                return false;
            }
            mpz_set(x_i_1, x_i);
        }
        if(mpz_get_ui(x_i) != 1){
            return false;
        }
    }
    return true;
}

void genPrimes_p_q(mpz_t &p, mpz_t &q){

    setup_primes_1e4();

    fstream f;
    f.open("/dev/urandom", ios::in|ios::binary);
    f.read((char*)input, MAX);
    f.close();
    temp = 0;

    mpz_init(res_check);
    mpz_init(check_odd);
    mpz_init(res);
    mpz_init(x_1);
    mpz_init(a);
    mpz_init(u);
    mpz_init(x_i);
    mpz_init(x_i_1);

    int couter1 = 0;
    while(couter1 < 1000000){
        couter1++;

        int counter2 = 0;
        while(counter2 < 500){
            counter2++;

            if(temp > 999910){
                fstream f;
                f.open("/dev/urandom", ios::in|ios::binary);
                f.read((char*)input, MAX);
                f.close();
                temp = 0;
            }

            genInt_720(q);
            if(checkPrime(q)){
                break;
            }
        }

        mpz_mul_2exp(p, q, 1U);
        mpz_add_ui(p, p, 1U);
        if(checkPrime(p)){
            break;
        }
    }
}

int main(){
    mpz_t p, q;
    mpz_init(p);
    mpz_init(q);
    
    double TOTAL_TIME = 0;
    
    for(int i = 0; i != 100; i++){
    	clock_t s = clock();
    	genPrimes_p_q(p, q);
    	clock_t e = clock();
    	double RES_TIME = (double)(e-s)/CLOCKS_PER_SEC;
    	cout<<"#"<<i+1<<'\n';
    	cout<<"p = ";
    	gmp_printf("%Zd\n", p);
    	cout<<"q = ";
    	gmp_printf("%Zd\n", q);
    	cout<<"Thoi gian: "<<RES_TIME<< " second (s)\n\n"<<endl;
    	TOTAL_TIME += RES_TIME;
    }
    
    cout<<"\nn = 100\nTOTAL_TIME = "<<TOTAL_TIME;
    cout<<"\nAVERAGE TIME: "<<TOTAL_TIME/100<<"\n\n";
    
    mpz_clears(p, q, NULL);

    return 0;
}

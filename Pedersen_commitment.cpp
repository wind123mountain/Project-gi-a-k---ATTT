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


int setup(mpz_t &p, mpz_t &q, mpz_t &g, mpz_t &h, mpz_t &A) {

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

    int choose_prime720 = 1;
    cout<<"Choose size of prime:{1, 0}\n1: 720 bit\n0: 3072 bit\nChoose: ";
    cin>>choose_prime720;

    cout<<"\n================================ SETUP ================================\n";

    if(choose_prime720 != 1){
        choose_prime720 = 0;
    }

    if(choose_prime720 == 1){
        genPrimes_p_q(p, q);
    }
    else{
        int couter1 = 0;
        while(couter1 < 1){
            couter1++;

            // genarate p
            int counter2 = 0;
            while(counter2 < 2500){
                counter2++;
                if(temp > 999600){
                    fstream f;
                    f.open("/dev/urandom", ios::in|ios::binary);
                    f.read((char*)input, MAX);
                    f.close();
                    temp = 0;
                }

                genInt_3072(p);
                if(checkPrime(p)){
                    break;
                }
            }

            // genarate q
            mpz_sub_ui(q, p, 1U);
            int t = 0;
            mpz_set_ui(check_odd, 1U);
            mpz_set_ui(res_check, 0U);
            while(mpz_get_ui(res_check) == 0){
                t++;
                mpz_div_2exp(q, q, 1U);
                mpz_and(res_check, q, check_odd);
            }

            for(int i = 0; i != size_primes_1e4; i++){
                mpz_mod_ui(res, q, primes_1e4[i]);
                if(mpz_get_ui(res) == 0){
                    mpz_div_ui(q, q, primes_1e4[i]);
                }
            }

        }
    }

    gmp_randstate_t state ;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, 1000000U);

    mpz_t mem;
    mpz_init(mem);
    mpz_sub_ui(mem, p, 1);
    mpz_div(mem, mem, q);
    mpz_urandomm(g, state, p);
    mpz_add_ui(g, g, 536870913U);
    mpz_powm(g, g, mem, p);

    mpz_sub_ui(mem, q, 1000U);
    mpz_urandomm(A, state, mem);
    mpz_add_ui(A, A, 1000U);

    mpz_powm(h, g, A, p);

    cout<<"p = ";
    gmp_printf("%Zd\n\n", p);
    cout<<"q = ";
    gmp_printf("%Zd\n\n", q);
    cout<<"g = ";
    gmp_printf("%Zd\n\n", g);
    cout<<"h = ";
    gmp_printf("%Zd\n", h);

    gmp_randclear(state);
    mpz_clears(x_1, x_i, x_i_1, a, u, res, res_check, check_odd, mem, NULL);

    return 0;
}

void commit(mpz_t &com, mpz_t &p, mpz_t &q, mpz_t &g, mpz_t &h, mpz_t &x, mpz_t &r){
    mpz_t mem;
    mpz_init(mem);

    gmp_randstate_t state ;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, 1000000U);
    mpz_sub_ui(mem, q, 1000U);
    mpz_urandomm(r, state, mem);
    mpz_add_ui(r, r, 1000U);

    mpz_powm(mem, g, x, p);
    mpz_powm(com, h, r, p);
    mpz_mul(com, com, mem);
    mpz_mod(com, com, p);

    gmp_randclear(state);
    mpz_clear(mem);
}

bool verify(mpz_t &com, mpz_t &p, mpz_t &g, mpz_t &h, mpz_t &x, mpz_t &r){
    mpz_t res, mem;
    mpz_init(mem);
    mpz_init(res);

    mpz_powm(mem, g, x, p);
    mpz_powm(res, h, r, p);
    mpz_mul(res, res, mem);
    mpz_mod(res, res, p);

    int ans = mpz_cmp(res, com);

    if(ans == 0){
        return true;
    }

    return false;
}

void commitment(mpz_t &p, mpz_t &q, mpz_t &g, mpz_t &h){

    mpz_t com, r, x;
    mpz_init(com);
    mpz_init(r);
    mpz_init(x);

    cout<<"\n================================ COMMIT ================================\n";
    cout<<"Enter value of x (belong to Zq): ";
    char *input_x = new char[MAX];
    cin.ignore();
    cin.getline(input_x, MAX);
    mpz_set_str(x, input_x, 10);

    commit(com, p, q, g, h, x, r);
    gmp_printf("\nr = %Zd\n\n", r);
    gmp_printf("com = %Zd\n\n", com);

    mpz_clears(com, x, r, NULL);
    delete input_x;
}

void verifier(mpz_t &p, mpz_t &g, mpz_t &h){
    mpz_t com, r, x;
    mpz_init(com);
    mpz_init(r);
    mpz_init(x);

    cout<<"\n================================ VERIFY ================================\n";
    cout<<"Enter value of x: ";
    char *input_data = new char[MAX];
    cin.getline(input_data, MAX);
    mpz_set_str(x, input_data, 10);

    cout<<"\nEnter value of r: ";
    cin.getline(input_data, MAX);
    mpz_set_str(r, input_data, 10);

    cout<<"\nEnter value of com: ";
    cin.getline(input_data, MAX);
    mpz_set_str(com, input_data, 10);

    bool res = verify(com, p, g, h, x, r);

    if(res){
        cout<<"\nResult: True\n";
    }
    else{
        cout<<"\nResult: False\n";
    }

    mpz_clears(com, x, r, NULL);
    delete input_data;
}

int main(){
    mpz_t p, q, g, h, A;
    mpz_init(p);
    mpz_init(q);
    mpz_init(g);
    mpz_init(h);
    mpz_init(A);

    setup(p, q, g, h, A);
    commitment(p, q, g, h);
    verifier(p, g, h);

    mpz_clears(p, q, g, h, A, NULL);

    return 0;
}

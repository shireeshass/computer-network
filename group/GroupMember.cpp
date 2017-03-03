#include <bits/stdc++.h>
#include <gmp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <poll.h>


#define ll long long

int port_no;

using namespace std;

struct table{
	//vector<pair<string,string> > publicKey;
	int size;
	char publicKey[100][2][100];
};

struct message{
	int key_id;
	ll text;
};

class Key_Generator{

public:
	mpz_t p,q;
	
	void gen_pqn(mpz_t e,mpz_t n,gmp_randstate_t state)
	{
		mpz_t base_key;
		mpz_init(base_key);
		mpz_ui_pow_ui(base_key,2,512);

		mpz_t rand_no;
		mpz_init(rand_no);

		mpz_t one,P_less_1,Q_less_1;
	
		mpz_init(P_less_1);
		mpz_init(Q_less_1);


		mpz_init_set_str(one,"1",10);

		//generate p
		mpz_urandomb(rand_no,state,512);
		mpz_add(p,base_key,rand_no);
		
		mpz_t gcd;
		mpz_init(gcd);

		while(1)
		{
			mpz_nextprime(p,p);
			
			mpz_sub(P_less_1,p,one);
			mpz_gcd(gcd,P_less_1,e);
			
			if(mpz_cmp(gcd,one)==0)
			break;
		}

		//generate q (!=p)
		mpz_urandomb(rand_no,state,512);
		mpz_add(q,base_key,rand_no);

		while(1)
		{
			mpz_nextprime(q,q);

			mpz_sub(Q_less_1,q,one);
			mpz_gcd(gcd,Q_less_1,e);

			//if p and q are not equal break
			if(mpz_cmp(p,q) && mpz_cmp(gcd,one)==0)
			break;
		}

		//generate n
		mpz_mul(n,p,q);
		
	}

	Key_Generator()
	{
		mpz_init(p);
		mpz_init(q);
	}

	void key_gen(mpz_t e,mpz_t n,gmp_randstate_t state)
	{
		gen_pqn(e,n,state);
	}

};

class RSA{
		
	void calculate_phi_n()
	{
		//phi_n = n-p-q+1
		mpz_t one;
		mpz_init_set_str(one,"1",10);

		mpz_set(phi_n,n);
		mpz_sub(phi_n,phi_n,key_generator.p);
		mpz_sub(phi_n,phi_n,key_generator.q);
		mpz_add(phi_n,phi_n,one);

		//gmp_printf("n= %Zd\n\n",n);
	}

	void generate_e()
	{
		mpz_set_si(e,65537);
	}
	
	void generate_d()
	{
		mpz_invert(d,e,phi_n);
	}

public:
	Key_Generator key_generator;
	mpz_t n,phi_n,d,e;
	gmp_randstate_t state;

	RSA()
	{
		mpz_init(e);
		mpz_init(d);
		mpz_init(n);
		mpz_init(phi_n);
		gmp_randinit_default (state);
	}

	void generate_keypairs()
	{
		generate_e();
		key_generator.key_gen(e,n,state);

		calculate_phi_n();
		generate_d();
	}

	void encrypt(mpz_t cipher_text,mpz_t plain_text)
	{
		mpz_init(cipher_text);
		
		mpz_powm_sec(cipher_text,plain_text,e,n);
	}

	void encrypt(mpz_t cipher_text,mpz_t plain_text,mpz_t e_val,mpz_t n_val)
	{
		mpz_init(cipher_text);
		
		mpz_powm_sec(cipher_text,plain_text,e_val,n_val);
	}
	
	void decrypt(mpz_t plain_text,mpz_t cipher_text)
	{
		mpz_init(plain_text);
		mpz_powm_sec(plain_text,cipher_text,d,n);
	}

	void decrypt(mpz_t plain_text,mpz_t cipher_text,mpz_t d_val,mpz_t n_val)
	{
		mpz_init(plain_text);
		mpz_powm_sec(plain_text,cipher_text,d_val,n_val);
	}

};

class Member{

	mpz_t password;
	string name;

	void generate_keypairs()
	{
		int no_of_key_pairs=min(10,rand()%10);

		public_key->size=0;

		for(int i=0;i<no_of_key_pairs;i++)
		{
			RSA rsa;

			rsa.generate_keypairs();

			//pair<string,string> publicKey;
			pair<mpz_t,mpz_t> privateKey;

			mpz_inits(privateKey.first,privateKey.second,NULL);

			//string str1(mpz_get_str(NULL,10,rsa.d));
			//string str2(mpz_get_str(NULL,10,rsa.n));

			strcpy(public_key->publicKey[i][0],mpz_get_str(NULL,10,rsa.d));
			strcpy(public_key->publicKey[i][1],mpz_get_str(NULL,10,rsa.n));
			public_key->size++;
			
			//private key kept in encrypted form
			encrypter.encrypt(privateKey.first,rsa.e);
			encrypter.encrypt(privateKey.second,rsa.n);

			private_key.push_back(privateKey);

		}
	}

public:
	RSA encrypter;
	struct table* public_key;
	vector<pair<mpz_t,mpz_t> > private_key;

	Member()
	{
		public_key=new table;
		encrypter.generate_keypairs();
		generate_keypairs();
	}

};


void* sendt(void* arg)
{	
	cout<<"Send Started\n";

	Member helper=*(Member*)arg;
	//struct table* ptable=(struct table*)malloc(sizeof(struct table));
	//ptable->publicKey=helper.public_key;
	
	char* IP=(char*)malloc(100);
	int port,csfd;

	while(1)
	{
		memset(IP,0,sizeof(IP));
		cout<<"Enter IP to establish a connection!"<<endl;
		cin>>IP;
		cout<<"Enter the port\n";
		cin>>port;

		cout<<"IP "<<IP<<" Port "<<port<<endl;

		//connect to the friend
		assert((csfd=socket(AF_INET,SOCK_STREAM,0))!=-1);

		struct sockaddr_in serv_addr;
		serv_addr.sin_family=AF_INET;
		serv_addr.sin_port=htons(port);
		serv_addr.sin_addr.s_addr=INADDR_ANY;

		assert(connect(csfd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr_in))!=-1);
			
		printf("Connected to server.\n");

		//send your table
		if(send(csfd,helper.public_key,sizeof(struct table),0)!=-1)
		{
			cout<<"Table "<<helper.public_key->publicKey[0][0]<<" "<<helper.public_key->publicKey[0][1]<<endl;

			int id;
			ll msg_text;

			struct message* msg=(struct message*)malloc(sizeof(struct message));

			while(1)
			{
				cout<<"Chose a public_key id:\n";
				cin>>msg->key_id;
				cout<<"Enter the message:\n";
				cin>>msg_text;

				cout<<"Sent data: Key_ID: "<<msg->key_id<<"\nText: "<<msg_text<<"."<<endl;
				mpz_t plain_text,cipher_text;
				mpz_init_set_si(plain_text,msg_text);

				mpz_t e,n;

				helper.encrypter.decrypt(e,helper.private_key[msg->key_id].first);
				helper.encrypter.decrypt(n,helper.private_key[msg->key_id].second);

				helper.encrypter.encrypt(cipher_text,plain_text,e,n);

				msg->text=mpz_get_si(cipher_text);

				send(csfd,msg,sizeof(msg),0);

			}
		}
		else
		{
			cout<<"Connection Error!\n";
		}
	}
}


void* receivet(void* arg)
{

	cout<<"Receive Started\n";

	struct table* ptable=(struct table*)malloc(sizeof(struct table));
	Member helper=*(Member*)arg;
	struct sockaddr peer_addr;
	socklen_t addr_len;
	int sfd;

	assert((sfd=socket(AF_INET,SOCK_STREAM,0))!=-1);
		
	struct sockaddr_in my_addr;
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(port_no);
	my_addr.sin_addr.s_addr=INADDR_ANY;

	assert(bind(sfd,(struct sockaddr *)&my_addr,sizeof(my_addr))!=-1);

	assert(listen(sfd,50)!=-1);

	cout<<"Bind and Listen Done! Port "<<port_no<<".\n";

	while(1)
	{
		int nsfd;
		assert((nsfd=accept(sfd,&peer_addr,&addr_len))!=-1);

		cout<<"Connection Accepted!\n";

		// a new chat member came and sent the table
		memset(ptable,0,sizeof(struct table));
		recv(nsfd,ptable,sizeof(struct table),0);

		cout<<"Table Received!\n";
		cout<<"Table "<<" "<<ptable->publicKey[0][0]<<" "<<ptable->publicKey[0][1]<<endl;

		struct message* msg=(struct message*)malloc(sizeof(struct message));

		while(1)
		{
			memset(msg,0,sizeof(message));
			recv(nsfd,msg,sizeof(message),0);

			cout<<"Message received! Key_ID is "<<msg->key_id<<".\n";

			int keyid=msg->key_id;
			mpz_t d,n;

			string dstr=ptable->publicKey[keyid][0];
			string nstr=ptable->publicKey[keyid][1];

			mpz_init_set_str(d,dstr.c_str(),10);
			mpz_init_set_str(n,nstr.c_str(),10);

			mpz_t plain_text,cipher_text;
			mpz_init_set_si(cipher_text,msg->text);

			helper.encrypter.decrypt(plain_text,cipher_text,d,n);

			if(mpz_cmp_si(plain_text,0))
			{
				cout<<"This friend went away! Connection closed.\n";
				close(nsfd);

				break;
			}
			else
			{
				cout<<"A new message arrived.It is: ";
				gmp_printf("Zd\n",plain_text);
			}
			
		}

	}
}


int main(int argc,char** argv)
{
	port_no=atoi(argv[1]);

	Member m;

	pthread_t SEND,RCV;

	pthread_create(&SEND,NULL,sendt,(void*)&m);
	pthread_create(&RCV,NULL,receivet,(void*)&m);

	pthread_join(SEND,NULL);
	pthread_join(RCV,NULL);

}
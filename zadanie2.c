/*
 ============================================================================
 Name        : EthRecv.c
 Author      : Krzysztof Maslanka
 Version     :
 Copyright   :
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <errno.h>
#include <string.h>

#include <sys/socket.h>
#include <linux/if_ether.h>
//dla rozpoznania typu w ethernecie
struct typether{
				unsigned char t1;
				unsigned char t2;
			}typethernetu;

int main(void) {
	printf("Uruchamiam odbieranie ramek Ethernet.\n"); /* prints  */

	//Utworzenie bufora dla odbieranych ramek Ethernet
	char* buffer = (void*) malloc(ETH_FRAME_LEN);

	//Otwarcie gniazda pozwalającego na odbiór wszystkich ramek Ethernet
	int iEthSockHandl = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	//Kontrola czy gniazdo zostało otwarte poprawnie, w przypadku bledu wyświetlenie komunikatu.
	if (iEthSockHandl<0)
			printf("Problem z otwarciem gniazda : %s!\n", strerror(errno));

	//Zmienna do przechowywania rozmiaru odebranych danych
	int iDataLen = 0;

	//Pętla nieskończona do odbierania ramek Ethernet
	int i=0;
	while (i<3) {

		//Odebranie ramki z utworzonego wcześniej gniazda i zapisanie jej do bufora
		iDataLen = recvfrom(iEthSockHandl, buffer, ETH_FRAME_LEN, 0, NULL, NULL);

		//Kontrola czy nie było bledu podczas odbierania ramki
		if (iDataLen == -1)
			printf("Nie moge odebrac ramki: %s! \n", strerror(errno));
		else { //jeśli ramka odebrana poprawnie wyświetlenie jej zawartości------------tutaj glowna czesc programu
			
			//rozpoznanie typu w ramce ethernet
			for(int k=0;k<iDataLen;k++)
				{
					if(k==12)
						{
							typethernetu.t1=buffer[k];
						}
					if(k==13)
						{
							typethernetu.t2=buffer[k];
						}
				}
			printf("Typ danych w ethernecie : 0x%x%x\n",typethernetu.t1,typethernetu.t2);

				i++;
		}
	}

	return EXIT_SUCCESS;
}
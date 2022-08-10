/*
 * ESP8266_HAL.c
 *
 *  Created on: Apr 14, 2020
 *      Author: Controllerstech
 */


#include "UartRingbuffer_multi.h"
#include "ESP8266_HAL.h"
#include "stdio.h"
#include "string.h"

#define MYWAIT 500

int Server_Send (char *str, int Link_ID);
void temporaryFunc(void);
int findString(char *dataString, char *findString);
void getToken(char *dataString);
void readNwrite();
void readNsave();
void scanIF(int hello);


void login(char name[8][20], int ledNumber);

void bunchOfLogin();

extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart2;

#define wifi_uart &huart3
#define pc_uart &huart2


char buffer[20];
char kingfer[50];
GPIO_PinState theta[8] = {1,1,1,1,1,1,1,1};
GPIO_PinState delta[8] = {1,1,1,1,1,1,1,1};
char ukLoc[32];

HAL_StatusTypeDef _mystat;
HAL_StatusTypeDef uartstat;
uint8_t popcorn[10];
char sendBuffer[50];
char resultBuf[128];

char tokenString[8];
char ledToken[8][9];

char *testTe = "helloworld";

char *Basic_inclusion = "<!DOCTYPE html> <html>\n<head><meta name=\"viewport\"\
		content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n\
		<title>LED CONTROL</title>\n<style>html { font-family: Helvetica; \
		display: inline-block; margin: 0px auto; text-align: center;}\n\
		body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\
		h3 {color: #444444;margin-bottom: 50px;}\n.button {display: block;\
		width: 80px;background-color: #1abc9c;border: none;color: white;\
		padding: 13px 30px;text-decoration: none;font-size: 25px;\
		margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n\
		.button-on {background-color: #1abc9c;}\n.button-on:active \
		{background-color: #16a085;}\n.button-off {background-color: #34495e;}\n\
		.button-off:active {background-color: #2c3e50;}\np {font-size: 14px;color: #888;margin-bottom: 10px;}\n\
		</style>\n</head>\n<body>\n<h1>ESP8266 LED CONTROL</h1>\n";

char *LED_ON = "<p>LED Status: ON</p><a class=\"button button-off\" href=\"/ledoff\">OFF</a>";
char *LED_OFF = "<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/ledon\">ON</a>";
char *Terminate = "</body></html>";






//char *Basic_inclusion = "<!DOCTYPE html><html>\n\<head></head>\n\<body>";
//char *LED_ON = "<p>LED STA : ON</p><a href=\"/ledoff\">OFF</a>";
//char *LED_OFF = "<p>LED STA : OFF</p><a href=\"/ledon\">ON</a>";
//char *Terminate = "</body></html>";





/*****************************************************************************************************************************************/
//✨사용하지 않음
void bunchOfLogin(){ 

	char name[8][20];

	for(int nameLength = 0; nameLength < 8; nameLength++){
		for(int nameCharNum = 0; nameCharNum < 20; nameCharNum++){
			name[nameLength][nameCharNum] = 0;
		}
	}//이름 초기화


	strcpy(name[0], "strangeSTM");
	strcpy(name[1], "QWETTY");
	strcpy(name[2], "strangeStrashcan");
	strcpy(name[3], "KINGGODWORLD");
	strcpy(name[4], "UNKWON");
	strcpy(name[5], "A32bC45");
	strcpy(name[6], "HotSummer");
	strcpy(name[7], "ColdSummer");
	//이름 지정

	//Server_Send("app 1q2w3e4r!@ strangeSTMEOF\r\n", 2);


	for(int ledNumber = 0; ledNumber < 8; ledNumber++){

		login(name, ledNumber);


	}

	printf("All login done!\r\n");
	//8번의 로그인
}

void login(char name[8][20], int ledNumber){
	/*
	int numericgod = 0;
logingod:

	if(numericgod > 10){

	}else{
		numericgod++;
		printf("%d,", numericgod);
		goto logingod;
	}
*/
	char data[80];
logingod:
	for(int ff = 0; ff < 80; ff++){
		data[ff] = 0;
	}

	sprintf(data, "app 1q2w3e4r!@ %sEOF\r\n", name[ledNumber]);
	Server_Send(data, 2);
	HAL_Delay(5000);
	printf("%s", data);


	//readNsave();
	//HAL_Delay(500);


	//printf("sibal %s", resultBuf);
	char isSuc = findString(resultBuf, "busy");
	if(isSuc){
		//error
		goto logingod;
	}else{
		//ok
		char isTok = findString(resultBuf, "TOK");
		if(isTok){
			//ok
			for(int ss = 14; ss < 22; ss++){
				printf("%c", resultBuf[ss]);
				ledToken[ledNumber][(ss-14)] = resultBuf[ss];
			}
			printf("{{{token found\r\n");

		}else{

			printf("token is missing!\r\n");
		}
	}
	/*

	char data[80];
hellogodplzhelp:

	sprintf(data, "app 1q2w3e4r!@ %sEOF\r\n", name[ledNumber]);
	Server_Send(data, 2);
	printf("%s\r\n", resultBuf);
	HAL_Delay(7000);
	goto hellogodplzhelp;
	*/

/*
	char isSuc = findString(resultBuf, "busy");

	if(isSuc){
		//1이 반환됨... 바쁨 -> 오류.ㅣ
		readNsave();
		HAL_Delay(500);
		goto hellogodplzhelp;
	}else{
		//0이 반환됨... 바쁘지 않음 -> 정상 전송
		char isTok = findString(resultBuf, "TOK");
		if(isTok){
			printf("SUCCESSQ!\r\n");
			readNsave();
			HAL_Delay(500);
		}else{
			//토큰이 아님. 오류
			readNsave();
			HAL_Delay(500);
			goto hellogodplzhelp;
		}
	}
	HAL_Delay(500);
*/
}


int findString(char *dataString, char *findString){
	int kStringLength = strlen(dataString);
	int fStringLength = strlen(findString);

	int rightScore = 0;
	for(int keepString = 0; keepString < kStringLength; keepString++){
		if(dataString[keepString] == findString[rightScore]){
			if(rightScore >= (fStringLength-1)){
				printf("%s FOUND!!\r\n", findString);
				return 1; //찾았다
			}
			rightScore++;
		}else{
			if(rightScore > 0){
				rightScore = 0;
			}
		}
	}
	return 0; //못찾았다
}


void getToken(char *dataString){

	int indexReader = 0;
	int lengthofstring = 0;

	while(1){
		printf("HELLO WORLD! : %c\r\n", dataString[0]);
		printf("LENGTH : %d\r\n", strlen(dataString));

		lengthofstring = strlen(dataString);


		findString(dataString, "EOF");
		findString(dataString, "busy");


/*
		if(dataString[(lengthofstring-1)] == 'F'){
			if(dataString[(lengthofstring-2)] == 'O'){
				if(dataString[(lengthofstring-3)] == 'E'){
					printf("EOF found\r\n"); //정상적인 데이터셋
				}
			}
		}
		*/
		break;
	}

	//tok => 14..21 (8)

}



void readNwrite(){
	printf("READ-WRITE activated\r\n");
	while(1){

		int isDataAva = IsDataAvailable(wifi_uart);

		if(isDataAva != 63){
			int popcornking = Uart_read(wifi_uart);

			if(popcornking == 13){
				printf("\r");
			}else if(popcornking == 10){
				printf("\n");
			}else if(popcornking == -1){
				break;
			}else{
				printf("%c", popcornking);
			}

			HAL_Delay(100);
		}else{
			//printf("fuck you. still stay.\r\n");
			//printf("%d\r\n", Uart_read(wifi_uart));
			printf("%c", Uart_read(wifi_uart));
			HAL_Delay(100);
		}
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}

	printf("escape\r\n");
}

void readNsave(){
	printf("Hello.\r\n");
	int tryNumeric = 0;
	char recvBuffer[128];
	for(int ff = 0; ff < 128; ff++){
		recvBuffer[ff] = 0;
		resultBuf[ff] = 0;
	}

	int tryNN = 0;

	while(1){

		int isDataAva = IsDataAvailable(wifi_uart);

		if(isDataAva != 63){
			int popcornking = Uart_read(wifi_uart);

			if(popcornking == 13){
				//printf("\r");
			}else if(popcornking == 10){
				//printf("\n");
			}else if(popcornking == -1){
				break;
			}else{
				//printf("[%c]\r\n", popcornking);
				recvBuffer[tryNumeric] = (char)(popcornking);
			}
			tryNumeric++;
			HAL_Delay(10);
		}else{

			//printf("[%c]\r\n", Uart_read(wifi_uart));
			Uart_read(wifi_uart);
			//printf("", Uart_read(wifi_uart));
			HAL_Delay(10);
		}
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
	for(int trN = 0; trN < 128; trN++){
		if(recvBuffer[trN] == 0){

			continue;
		}else{
			resultBuf[tryNN] = recvBuffer[trN];
			tryNN++;
			//printf("%c\r\n", recvBuffer[trN]);
		}
	}
	printf("loginas : %s", resultBuf);
	printf("saved\r\n");
	//printf("%c\r\n", resultBuf[0]);


	//data inspection

	//getToken(resultBuf);
}

void ESP_Init (char *SSID, char *PASSWD)
{
	for(int i = 0; i < 8; i++){
		theta[i] = GPIO_PIN_RESET;
		delta[i] = GPIO_PIN_RESET;
	}
	char data[80];

	Ringbuf_init();

	Uart_sendstring("AT+RST\r\n", wifi_uart);
	Uart_sendstring("RESETTING.", pc_uart);
	for (int i=0; i<5; i++)
	{
		Uart_sendstring(".", pc_uart);
		HAL_Delay(1000);
	}

	/********* AT **********/
	Uart_sendstring("AT\r\n", wifi_uart);
	while(!(Wait_for("AT\r\r\n\r\nOK\r\n", wifi_uart)));
	Uart_sendstring("AT---->OK\r\n", pc_uart);


	/********* AT+CWMODE=1 **********/
	Uart_sendstring("AT+CWMODE=1\r\n", wifi_uart);
	while (!(Wait_for("AT+CWMODE=1\r\r\n\r\nOK\r\n", wifi_uart)));
	Uart_sendstring("CW MODE---->1\r\n", pc_uart);


	/********* AT+CWJAP="SSID","PASSWD" **********/
	Uart_sendstring("connecting... to the provided AP\r\n", pc_uart);
	sprintf (data, "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PASSWD);
	Uart_sendstring(data, wifi_uart);
	while (!(Wait_for("WIFI GOT IP\r\n\r\nOK\r\n", wifi_uart)));
	sprintf (data, "Connected to,\"%s\"\r\n", SSID);
	Uart_sendstring(data,pc_uart);


	/********* AT+CIFSR **********/
	Uart_sendstring("AT+CIFSR\r\n", wifi_uart);
	while (!(Wait_for("CIFSR:STAIP,\"", wifi_uart)));
	while (!(Copy_upto("\"",buffer, wifi_uart)));
	while (!(Wait_for("OK\r\n", wifi_uart)));
	int len = strlen (buffer);
	buffer[len-1] = '\0';
	sprintf (data, "IP ADDR: %s\r\n", buffer);
	Uart_sendstring(data, pc_uart);


	Uart_sendstring("AT+CIPMUX=1\r\n", wifi_uart);
	while (!(Wait_for("AT+CIPMUX=1\r\r\n\r\nOK\r\n", wifi_uart)));
	Uart_sendstring("CIPMUX---->OK\r\n", pc_uart);

	Uart_sendstring("AT+CIPSERVER=1,80\r\n", wifi_uart);
	while (!(Wait_for("OK\r\n", wifi_uart)));
	Uart_sendstring("CIPSERVER---->OK\r\n", pc_uart);

	Uart_sendstring("Now Connect to the IP ADRESS\r\n", pc_uart);

	/****ATE0 -> 0 echo off, 1 echo on****/
	Uart_sendstring("ATE0\r\n", wifi_uart);
	while (!(Wait_for("OK\r\n", wifi_uart)));

	HAL_Delay(MYWAIT);

	Uart_sendstring("AT+CIPSTART=2,\"TCP\",\"<📝목적지 IP>\",<📝목적지 PORT>\r\n", wifi_uart);
//13.125.252.196:58031
	while(!(Wait_for("OK\r\n", wifi_uart)));

	readNwrite();

	//login("KINGGOD", 911);

	//bunchOfLogin(); ✨bunchOfLogin 사용 금지

	/* ✨ bunchOfLogin이 사용 금지이므로 ledToken을 여러개 받을 필요가 없음
	for(int kk = 0; kk < 8; kk++){
		printf("[LED TOK] : %s\r\n", ledToken[kk]);
	}
	*/
	
	
	/*🎈단일 쓰레기통용 코드. 로그인을 한번만 수행함*/
	
	login("<📝쓰레기통 ID>", 0); //✨두번째 파라미터에 있는 0은 login 배열 번호. 변경 금지
	
	/******************************************/


//✨해당 for문은 건물 모형 전용 구문
	for(int ppo = 0; ppo < 8; ppo++){
		theta[ppo] = 1;
		delta[ppo] = 1;
	}






	printf("Initial Login End\r\n");

	HAL_Delay(1500);

/*
	char kinggodString[80];

	sprintf(kinggodString, "cap %s 100EOF\r\n", ledToken[5]);
	printf("%s", kinggodString);

	Server_Send(kinggodString, 2);
	HAL_Delay(1500);


	sprintf(kinggodString, "cap %s 100EOF\r\n", ledToken[6]);
	printf("%s", kinggodString);

	Server_Send(kinggodString, 2);
*/


}




int Server_Send (char *str, int Link_ID)
{
	int len = strlen (str);
	char data[80];
	sprintf (data, "AT+CIPSEND=%d,%d\r\n", Link_ID, (len-2));

	Uart_sendstring(data, pc_uart); //transfer data to pc
	Uart_sendstring(data, wifi_uart); //AT+CIPSEND=%d,%d\r\n을 wifi로 전송
	while (!(Wait_for(">", wifi_uart))); //">"을 기다림
	HAL_Delay(MYWAIT);

	//sprintf (data, "%s", str);
	Uart_sendstring (str, wifi_uart); //입력된 데이터를 보냄
	Uart_sendstring(str, pc_uart); //transfer string to pc
	while (!(Wait_for("SEND", wifi_uart)));
	HAL_Delay(MYWAIT);

	Uart_sendstring("+++", pc_uart);
	Uart_sendstring("+++", wifi_uart);

	while (!(Wait_for("OK\r\n", wifi_uart)));
	readNsave();
	//Server_Start();
	HAL_Delay(1000);








	return 1;
}

void Server_Handle (char *str, int Link_ID)
{
	char datatosend[1024] = {0};
	if (!(strcmp (str, "/ledon")))
	{
		sprintf (datatosend, Basic_inclusion);
		strcat(datatosend, LED_ON);
		strcat(datatosend, Terminate);
		Server_Send(datatosend, Link_ID);
	}

	else if (!(strcmp (str, "/ledoff")))
	{
		sprintf (datatosend, Basic_inclusion);
		strcat(datatosend, LED_OFF);
		strcat(datatosend, Terminate);
		Server_Send(datatosend, Link_ID);
	}

	else
	{
		sprintf (datatosend, Basic_inclusion);
		strcat(datatosend, LED_OFF);
		strcat(datatosend, Terminate);
		Server_Send(datatosend, Link_ID);
	}

}

//✨서버 스타트를 더이상 사용하지 않음. 다중로그인용
void Server_Start (void)
{



	while(1){

		int isDataAva = IsDataAvailable(wifi_uart);

		if(isDataAva != 63){
			//printf("someone detected.\r\n");
			int popcorn = Uart_read(wifi_uart);

			if(popcorn == 13){
				printf("\r");
			}else if(popcorn == 10){
				printf("\n");
			}else if(popcorn == -1){
				break;
			}else{
				printf("%c", popcorn);
			}

			//printf("char : %d | %c\r\n", popcorn, popcorn);
			HAL_Delay(100);
		}else{
			printf("still stay.\r\n");
			printf("%d\r\n", Uart_read(wifi_uart));
			HAL_Delay(100);
		}
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}

	printf("escape\r\n");

	Uart_sendstring("AT+CIPRECVDATA=2,100\r\n", wifi_uart);
	HAL_Delay(MYWAIT);


	while(1){

		int isDataAva = IsDataAvailable(wifi_uart);

		if(isDataAva != 63){
			//printf("someone detected.\r\n");
			int popcorn = Uart_read(wifi_uart);

			if(popcorn == 13){
				printf("\r");
			}else if(popcorn == 10){
				printf("\n");
			}else if(popcorn == -1){
				break;
			}else{
				printf("[%d]%c", popcorn, popcorn);
			}

			//printf("char : %d | %c\r\n", popcorn, popcorn);
			HAL_Delay(100);
		}else{
			printf("still stay.\r\n");
			printf("%d\r\n", Uart_read(wifi_uart));
			HAL_Delay(100);
		}
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
	printf("escape\r\n");

}

/*✨2022-07-23 : ESP8266 업데이트*/

void scanIF(int hello){
	char kinggodString[80];

	char percentage = 0;

	if(hello > 400){
		percentage = 0;
	}else{
		percentage = ((400 - hello) / 400);
	}

	sprintf(kinggodString, "cap %s %dEOF\r\n", ledToken[0]);

	/*
	if(hello){
		sprintf(kinggodString, "cap %s 100EOF\r\n", ledToken[0]);
	}else{
		sprintf(kinggodString, "cap %s 3EOF\r\n", ledToken[0]);
	}
	*/
	
	Server_Send(kinggodString, 2);
}

/*🎉2022-07-23 : ESP8266 업데이트*/

void scanTrashcan(void){


	//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); ✨LED 깜빡이기... 레거시 코드이므로 사용하지 않음
	
	
	/*해당 FOR문은 더이상 사용하지 않음*/
	
	/*
	for(int i = 0; i < 8; i++){
		delta[i] = HAL_GPIO_ReadPin(GPIOC, (uint16_t)(1 << i));

		if(theta[i] != delta[i]){ //✨적외선 읽기용 조건문. 더이상 사용하지 않음
			//sprintf(ukLoc, "Loc%d : %dEOF\r\n", i, delta[i]);

			//Server_Send(ukLoc, 2);
			//printf("%s", ukLoc);

			for(int ww = 0; ww < 32; ww++){
				ukLoc[ww] = 0;
			}

			if(delta[i]){
				//empty
				sprintf(ukLoc, "cap %s 3EOF\r\n", ledToken[i]);
				printf("%s", ukLoc);

				Server_Send(ukLoc, 2);


			}else{
				//full
				sprintf(ukLoc, "cap %s 100EOF\r\n", ledToken[i]);
				printf("%s", ukLoc);

				Server_Send(ukLoc, 2);
			}

			theta[i] = delta[i];

			break;
		}

	}
	*/
	
	/*해당 FOR문은 더이상 사용하지 않음*/


	//✨디버깅 전용... PCOM 입력으로 ESP동작
	uartstat = HAL_UART_Receive(&huart2, popcorn, 1, 1);
	if(uartstat == HAL_OK){
		printf("%c\r\n", popcorn[0]);

		//sprintf(sendBuffer, "app 1q2w3e4r!@ strangeSTMEOF\r\n");
		sprintf(sendBuffer, "cap L*VdxW6Z 66EOF\r\n");
		Server_Send(sendBuffer, 2);

	}






}

void temporaryFunc(void){
	//Get_after();
}

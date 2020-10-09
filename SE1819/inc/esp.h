/**
* @file		esp.h
* @brief	Contains the API to the ESP wifi module.
* @version	1.1
* @date		25 Jun 2019
* @author	Ivo Pereira
*/

#ifndef ESP_H_
#define ESP_H_

/** @defgroup ESP ESP
 * This package provides a set of AT commands to communicate with the esp module.
 * @{
 */

/** @defgroup ESP_Public_Functions ESP Public Functions
 * @{
 */

/**
 * @brief	Initializes the ESP controller API.
 * @return	Void.
 */
void ESP_Init();

/**
 * @brief	Sets the operation mode os the esp.
 * @param 	mode: operation mode set.
 * @param	response: Pointer to position in memory where the response is going to be written.
 * @param 	length: Pointer to a variable where the length of the response is going to be written.
 * @return 	int.
 */
int ESP_SetOperationMode(int mode, unsigned char* response, unsigned int *length);

/**
 * @brief	Disconects the esp from a network if it is connected.
 * @param	response: Pointer to position in memory where the response is going to be written.
 * @param 	length: Pointer to a variable where the length of the response is going to be written.
 * @return 	int.
 */
int ESP_QuitNetwork(unsigned char* response, unsigned int *length);

/**
 * @brief	Lists available networks that the esp detects through wifi.
 * @param	response: Pointer to position in memory where the response is going to be written.
 * @param 	length: Pointer to a variable where the length of the response is going to be written.
 * @return 	int.
 */
int ESP_ListAvailableNetworks(unsigned char* response, unsigned int *length);

/**
 * @brief	Connects the esp to a specific network.
 * @param 	ssid: ssid of network to connect.
 * @param 	pass: password of network to connect.
 * @param	response: Pointer to position in memory where the response is going to be written.
 * @param 	length: Pointer to a variable where the length of the response is going to be written.
 * @return 	int.
 */
int ESP_ConnectToNetwork(unsigned char* ssid, unsigned char* pass,unsigned char* response, unsigned int *length);

/**
 * @brief	Gets the IP of the esp in the network.
 * @param	response: Pointer to position in memory where the response is going to be written.
 * @param 	length: Pointer to a variable where the length of the response is going to be written.
 * @return 	int.
 */
int ESP_GetIP(unsigned char* response, unsigned int *length);

/**
 * @brief	Gets the IP, gateway and netmask of the esp in the network.
 * @param	response: Pointer to position in memory where the response is going to be written.
 * @param 	length: Pointer to a variable where the length of the response is going to be written.
 * @return 	int.
 */
int ESP_CheckIP(unsigned char* response, unsigned int *length);

/**
 * @brief	Pings an address.
 * @param 	addr: address to ping.
 * @param	response: Pointer to position in memory where the response is going to be written.
 * @param 	length: Pointer to a variable where the length of the response is going to be written.
 * @return 	int.
 */
int ESP_Ping(unsigned char* addr, unsigned char* response, unsigned int *length);

/**
 * @brief	Sets a DNS to the esp.
 * @param 	domain: DNS to be set.
 * @param	response: Pointer to position in memory where the response is going to be written.
 * @param 	length: Pointer to a variable where the length of the response is going to be written.
 * @return 	int.
 */
int ESP_SetDNS(unsigned char* domain, unsigned char* response, unsigned int *length);

/**
 * @brief	Starts a transference through the network.
 * @param	type: type of the transference
 * @param 	addr: address to connect to.
 * @param 	port: port to be used.
 * @param	response: Pointer to position in memory where the response is going to be written.
 * @param 	length: Pointer to a variable where the length of the response is going to be written.
 * @return 	int.
 */
int ESP_StartTransfer(unsigned char* type, unsigned char* addr, unsigned int port, unsigned char* response, unsigned int *length);

/**
 * @brief	Sends and receives data in a transference through the network.
 * @param 	data: data to be sent.
 * @param 	size: number of bytes to be sent.
 * @param	response: Pointer to position in memory where the response is going to be written.
 * @param 	length: Pointer to a variable where the length of the response is going to be written.
 * @return 	int.
 */
int ESP_Transfer(unsigned char* data, unsigned int size, unsigned char* response, unsigned int *length);

/**
 * @brief	Closes a currently opened transference.
 * @param	response: Pointer to position in memory where the response is going to be written.
 * @param 	length: Pointer to a variable where the length of the response is going to be written.
 * @return 	int.
 */
int ESP_CloseTransfer(unsigned char* response, unsigned int *length);

/**
 * @brief	Restarts the esp module.
 * @param	response: Pointer to position in memory where the response is going to be written.
 * @param 	length: Pointer to a variable where the length of the response is going to be written.
 * @return 	int.
 */
int ESP_Restart(unsigned char* response, unsigned int *length);

/**
 * @}
 */


/**
 * @}
 */

#endif /* ESP_H_ */

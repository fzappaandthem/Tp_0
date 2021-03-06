/*
 * main.c
 *
 *  Created on: 28 feb. 2019
 *      Author: utnso
 */

#include "tp0.h"


int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	// int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	//Loggear "soy un log"
	log_info(logger, "soy un log");
	
	leer_consola(logger);

	config = leer_config();

	//asignar valor de config a la variable valor
	valor = config_get_string_value(config, "CLAVE");
	
	//Loggear valor de config
	log_info(logger, valor);
	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	//antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.
	//crear conexion
	int socket_cliente = crear_conexion("127.0.0.1", "4444");
	//enviar CLAVE al servidor
	char *mensaje = config_get_string_value(config, "CLAVE");
	enviar_mensaje(mensaje, socket_cliente);

	paquete(socket_cliente);

	liberar_conexion(socket_cliente);

	terminar_programa(socket_cliente, logger, config);
	
}

t_log* iniciar_logger(void)
{
	log_create("tp0.log", "tp0",true, LOG_LEVEL_INFO);
}

t_config* leer_config(void)
{
	config_create("tp0.config");
}

void leer_consola(t_log* logger)
{
	char* leido;

	//El primero te lo dejo de yapa

	while (strcmp((leido = readline(">")), "") != 0)
	{
		log_info(logger, leido);
		free(leido);
	}
	

	free(leido);

}

void paquete(int socket_cliente)
{
	//Ahora toca lo divertido!

	char* leido;

	t_paquete *paquete = crear_paquete();

	while (strcmp((leido = readline(">")), "") != 0)
	{
		agregar_a_paquete(paquete, leido, (strlen(leido) +1 * sizeof(char)));
		free(leido);
	}

	enviar_paquete(paquete, socket_cliente);

	eliminar_paquete(paquete);

	free(leido);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}

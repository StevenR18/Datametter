#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\..\Vt\includes\memory_traker.h"
typedef struct Nodo {
  void *data;
  struct Nodo *next;
} Nodo;

// Función para crear un nodo
Nodo *create_nodo(void *data, size_t data_size) {
  Nodo *newNodo = (Nodo *)mymalloc(sizeof(Nodo),__func__,__FILE__);
  if (newNodo == NULL) {
    perror("Error al asignar memoria para el nodo");
    return NULL;
  }

  newNodo->data = mymalloc(data_size,__func__,__FILE__);  // Asignar memoria para los datos
  if (newNodo->data == NULL) {
    perror("Error al asignar memoria para los datos");
    myfree(newNodo);
    return NULL;
  }

  memcpy(newNodo->data, data, data_size);  // Copiar los datos
  newNodo->next = NULL;
  return newNodo;
}

// Función para insertar un nodo al inicio
int insert_node_begin(Nodo **head, void *data, size_t data_size) {
  Nodo *newNodo = create_nodo(data, data_size);
  if (newNodo == NULL) return -1;

  newNodo->next = *head;
  *head = newNodo;
  return 0;
}

// Función para insertar un nodo al final
int insert_nodo_end(Nodo **head, void *data, size_t data_size) {
  Nodo *newNodo = create_nodo(data, data_size);
  if (newNodo == NULL) return -1;

  if (*head == NULL) {
    *head = newNodo;
    return 0;
  }

  Nodo *temp = *head;
  while (temp->next != NULL) {
    temp = temp->next;
  }

  temp->next = newNodo;
  return 0;
}

// Función para liberar la memoria de la lista
int free_lista(Nodo **head) {
  if (*head == NULL) return -1;

  Nodo *current = *head;
  Nodo *next;

  while (current != NULL) {
    next = current->next;
    myfree(current->data);  // Liberar los datos
    myfree(current);        // Liberar el nodo
    current = next;
  }

  *head = NULL;  // Asegurarse de que la lista quede vacía
  return 0;
}


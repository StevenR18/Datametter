#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "..\..\Vt\includes\memory_traker.h"
#include "..\..\Vt\includes\liked_list.h"

// Función para crear un nodo
Nodo *create_nodo(wchar_t *data, size_t len) {
  Nodo *newNodo = (Nodo *)malloc(sizeof(Nodo));
    //mymalloc(sizeof(Nodo),__func__,__FILE__);
  if (newNodo == NULL) {
    perror("Error al asignar memoria para el nodo");
    return NULL;
  }

  newNodo->data = (wchar_t *)malloc(sizeof(wchar_t)*(len+1));
    ///mymalloc(len+1,__func__,__FILE__);  // Asignar memoria para los datos
  if (newNodo->data == NULL) {
    perror("Error al asignar memoria para los datos");
    myfree(newNodo);
    return NULL;
  }

  wmemcpy(newNodo->data, data, len);  // Copiar los datos
  newNodo->data[len]=L'\0';
  newNodo->next = NULL;
  return newNodo;
}

// Función para insertar un nodo al inicio
int insert_node_begin(Nodo **head, wchar_t *data, size_t len) {
  Nodo *newNodo = create_nodo(data, len);
  if (newNodo == NULL) return -1;

  newNodo->next = *head;
  *head = newNodo;
  return 0;
}

// Función para insertar un nodo al final
int insert_nodo_end(Nodo **head, wchar_t *data, size_t len) {
  Nodo *newNodo = create_nodo(data, len);
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
    free(current->data);  // Liberar los datos
    free(current);        // Liberar el nodo
    current = next;
  }

  *head = NULL;  // Asegurarse de que la lista quede vacía
  return 0;
}


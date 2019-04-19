#include <stdio.h>
#include <stdlib.h>

typedef struct dot{
  int Id;
  int discovery;
  int low;
  int OnStack;
	int group;
}Dot;

typedef struct list{
	Dot dot;
	struct list* next;
}List;

Dot New_Dot(int id){
	Dot toReturn;
	toReturn.Id = id;
	toReturn.discovery = -1;
	toReturn.low = -1;
	toReturn.OnStack = 0;
	toReturn.group = 0;
	return toReturn;
}

int Dot_Counter, Connection_Counter, counter, Visited, min;
List* L;
List *temp;

Dot* dots;
List** connections;
int** out;

int comparator(const void *v1, const void *v2){
	int* c1 = *((int**)v1);
	int* c2 = *((int**)v2);
	if (c1[0] == c2[0]){
		return c1[1] - c2[1];
	}
	else{
		return c1[0] - c2[0];
	}
}

void free_Adjacents(){
	int i;
	for (i = 0; i < Dot_Counter; i++){
		List* cursor = connections[i];
		List* tmp;
		while (cursor != NULL){
			tmp = cursor;
			cursor = cursor->next;
			free(tmp);
		}
	}
}

void free_Everything(){
	free(dots);
	free(out);
}

int create_Out(){
	List* cursor;
	int counter = 0, i;
	if (counter == Dot_Counter){
		for (i = 0; i < Dot_Counter; i++){
			cursor = connections[i];
			while (cursor != NULL){
				out[counter] = (int*)malloc(sizeof(int) * 2);
				out[counter][0] = dots[i].Id;
				out[counter][1] = cursor->dot.Id;
				counter++;
			}
		cursor = cursor->next;
		}
	}
	else{
		for (i = 0; i < Dot_Counter; i++){
			cursor = connections[i];
			while (cursor != NULL){
				if (counter == 0 && dots[i].group != dots[cursor->dot.Id - 1].group){
					out[counter] = (int*)malloc(sizeof(int) * 2);
					out[counter][0] = dots[i].group;
					out[counter][1] = dots[cursor->dot.Id - 1].group;
					counter++;
				}
				else if (dots[i].group != dots[cursor->dot.Id - 1].group){
					if ((dots[i].group != out[counter - 1][0]) || (dots[cursor->dot.Id - 1].group != out[counter - 1][1])){
						out[counter] = (int*)malloc(sizeof(int) * 2);
						out[counter][0] = dots[i].group;
						out[counter][1] = dots[cursor->dot.Id - 1].group;
						counter++;
					}
				}
			cursor = cursor->next;
			}
		}
	}
	return counter;
}

List* Push (List* lt, Dot dt){
	List* tmp = (List*)malloc(sizeof(List));
	dt.OnStack = 1;
	dots[dt.Id - 1].OnStack = 1;
	tmp->dot = dt;
	tmp->next = lt;
	return tmp;
}

void tmp_Push(Dot dt){
	List* tmp = (List*)malloc(sizeof(List));
	if (min == 0 || min > dt.Id)
		min = dt.Id;
	tmp->dot = dt;
	tmp->next = temp;
	temp = tmp;
}

void Pop_all(){
	List *tmp;
	while (temp != NULL){
		dots[temp->dot.Id - 1].group = min;
		tmp = temp;
		temp = temp->next;
		free(tmp);
	}
	min = 0;
}

Dot Pop (List* lt){
	Dot dt;
	if (lt == NULL){
		dt.Id = 0;
		dots[dt.Id - 1].OnStack = 0;
		return dt;
	}
	L = lt->next;
	lt->next = NULL;
	dt = lt->dot;
	dots[dt.Id - 1].OnStack = 0;
	free(lt);
	return dt;
}

int Minimum(int a, int b){
	if (a < b)
		return a;
	else
		return b;
}

int IsInStack(Dot dt){
	return dt.OnStack;
}

void Tarjan_Visit(Dot* U){
	List* cursor;
	Dot V;
	U = &dots[(*U).Id - 1];
	U->discovery = Visited;
	U->low = Visited;
	Visited++;
	L = Push(L, *U);
	cursor = connections[U->Id - 1];
	while (cursor != NULL){
		Dot dt = dots[cursor->dot.Id - 1];
		if (dt.discovery == -1 || IsInStack(dt)){
			if (dt.discovery == -1){
				Tarjan_Visit(&(cursor->dot));
			}
			dt = dots[dt.Id - 1];
			U->low = Minimum(U->low, dt.low);
		}
		cursor = cursor->next;
	}
	if (U->discovery == U->low){
		min = 0;
		counter++;
		if (L != NULL){
			V = Pop(L);
			tmp_Push(V);
		}
		else
			V = V;
		/*dots[V.Id - 1].group = U->Id;*/
		while(U->Id != V.Id){
			/*dots[V.Id - 1].group = U->Id;*/
			if (L != NULL){
				V = Pop(L);
				tmp_Push(V);
				/*dots[V.Id - 1].group = U->Id;*/
			}
			else
				V = V;
		}
	}
	Pop_all();
}

int SCC_Tarjan(){
	int i;
	min = 0;
	counter = 0;
	Visited = 0;
	L = NULL;
	for (i = 0; i < Dot_Counter; i++){
		if (dots[i].discovery == -1){
			Tarjan_Visit(&dots[i]);
		}
	}
	return counter;
}

int main(){
	int i, c1, c2, j;
  int origin, destiny, counter, size;

  if (scanf("%d", &Dot_Counter) != 1)
		printf("Erro: Número errado de argumentos");

  if (scanf("%d", &Connection_Counter) != 1)
		printf("Erro: Número errado de argumentos");

	dots = (Dot*)malloc(sizeof(Dot) * Dot_Counter);
	connections = (List**)malloc(sizeof(List*) * Dot_Counter);

	for (i = 0; i < Dot_Counter; i++){
		dots[i] = New_Dot(i + 1);
		connections[i] = (List*)malloc(sizeof(List));
		connections[i] = NULL;
	}

  for (i = 0; i < Connection_Counter; i++){
		List* tmp;
    if (scanf ("%d %d", &origin, &destiny) != 2)
			printf("Erro: Número errado de argumentos");
		tmp  = (List*)malloc(sizeof(List));
		tmp->dot = dots[destiny - 1];
		tmp->next = connections[origin - 1];
		connections[origin - 1] = tmp;
  }

	counter = SCC_Tarjan();

	/*for (i = 0; i < Dot_Counter; i++){
		printf("Dot %d is in group %d\n", dots[i].Id, dots[i].group);
	}*/

	printf("%d\n", counter);
	if (counter == 1){
		printf("%d\n", 0);
		free_Adjacents();
		free_Everything();
		return 0;
	}
	out = (int**)malloc(sizeof(int*) * Connection_Counter);
	size = create_Out();
	free_Adjacents();
	qsort(out, size, (sizeof(int) * 2), comparator);
	c1 = 0;
	c2 = 0;
	j = 0;
	for (i = 0; i < Connection_Counter; i++){
		if (out[i] == NULL)
			break;
		if (out[i][0] != c1 || out[i][1] != c2){
			j++;
			c1 = out[i][0];
			c2 = out[i][1];
		}
	}
	c1 = 0;
	c2 = 0;
	printf("%d\n", j);
	for (i = 0; i < Connection_Counter; i++){
		if (out[i] == NULL)
			break;
		if (out[i][0] != c1 || out[i][1] != c2){
			printf("%d %d\n", out[i][0], out[i][1]);
			c1 = out[i][0];
			c2 = out[i][1];
		}
	}
	free_Everything();
	return 0;
}

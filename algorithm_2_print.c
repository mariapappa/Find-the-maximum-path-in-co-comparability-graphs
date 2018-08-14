#include <stdio.h>
#include <stdlib.h>


void allocate_space(int v_num, int ***adj_addr, int **sigma_addr, int *****path_addr, int ****length_addr);
void read_edges(int **a, int size);
void get_ordering(int *s, int size);
void print_adj_matrix(int **a, int size);
void print_ordering(int *ordering, int size);
void print_temp_ordering(int *s, int size, int i);
void ldfs_plus(int **a, int v_num, int *s);
void compute_paths(int **a, int v_num, int *s, int ****path, int ***length);
int compute_max_path_length(int ***length, int v_num);
void print_max_paths(int ****path, int ***length, int v_num, int max_length);
void deallocate_space(int v_num, int **adj, int *sigma, int 
****path, int ***length);


void allocate_space(int v_num, int ***adj_addr, int **sigma_addr, int *****path_addr, int ****length_addr)
{
	int i, j, k;
	int **temp_adj, *temp_sigma, ****temp_path, ***temp_length;

	/* allocate space for adjacency matrix */
	temp_adj = (int **)malloc(v_num * sizeof(int *));
	if (temp_adj == NULL)
	{
		printf("\nNo space for adjacency matrix. Aborting...\n");
		exit(-1);
	}
	for (i=v_num; --i >= 0;  )
	{
		temp_adj[i] = (int *)malloc(v_num * sizeof(int));
		if (temp_adj[i] == NULL)
		{
            	printf("\nNo space for adjacency row. Aborting...\n");
			exit(-1);
		}
	}
	*adj_addr = temp_adj;

	/* allocate space for ordering */
	temp_sigma = (int *)malloc(v_num * sizeof(int));
	if (temp_sigma == NULL)
	{
		printf("\nNo space for ordering. Aborting...\n");
		exit(-1);
	}
	*sigma_addr = temp_sigma;

	/* allocate space for path matrix */
	temp_path = (int ****)malloc(v_num * sizeof(int ***));
	if (temp_path == NULL)
	{
		printf("\nNo space for path matrix. Aborting...\n");
		exit(-1);
	}
	for (i=v_num; --i >= 0;  )
	{
		temp_path[i] = (int ***)malloc(v_num * sizeof(int **));
		if (temp_path[i] == NULL)
		{
            	printf("\nNo space for paths. Aborting...\n");
			exit(-1);
		}
		for (j=v_num; --j >= 0;  )
		{
			temp_path[i][j] = (int **)malloc(v_num * sizeof(int *));
			if (temp_path[i][j] == NULL)
			{
            		printf("\nNo space for paths. Aborting...\n");
				exit(-1);
			}
			for (k=v_num; --k >= 0;  )
			{
				temp_path[i][j][k] = (int *)malloc(v_num * sizeof(int));
				if (temp_path[i][j][k] == NULL)
				{
            			printf("\nNo space for paths. Aborting...\n");
					exit(-1);
				}
			}
		}
	}
	*path_addr = temp_path;

	/* allocate space for length matrix */
	temp_length = (int ***)malloc(v_num * sizeof(int **));
	if (temp_length == NULL)
	{
		printf("\nNo space for length matrix. Aborting...\n");
		exit(-1);
	}
	for (i=v_num; --i >= 0;  )
	{
		temp_length[i] = (int **)malloc(v_num * sizeof(int *));
		if (temp_length[i] == NULL)
		{
            	printf("\nNo space for lengths. Aborting...\n");
			exit(-1);
		}
		for (j=v_num; --j >= 0;  )
		{
			temp_length[i][j] = (int *)malloc(v_num * sizeof(int));
			if (temp_length[i][j] == NULL)
			{
            		printf("\nNo space for lengths. Aborting...\n");
				exit(-1);
			}
		}
	}
	*length_addr = temp_length;
}


void deallocate_space(int v_num, int **adj, int *sigma, int ****path, int ***length)
{
	int i, j, k;

	/* deallocate space for adjacency matrix */
	for (i=v_num; --i >= 0;  )
		free(adj[i]);
	free(adj);

	/* deallocate space for ordering */
	free(sigma);

	/* deallocate space for path matrix */
	for (i=v_num; --i >= 0;  )
	{
		for (j=v_num; --j >= 0;  )
		{
			for (k=v_num; --k >= 0;  )
				free(path[i][j][k]);
			free(path[i][j]);
		}
		free(path[i]);
	}
	free(path);

	/* deallocate space for length matrix */
	for (i=v_num; --i >= 0;  )
	{
		for (j=v_num; --j >= 0;  )
			free(length[i][j]);
		free(length[i]);
	}
	free(length);
}


void read_edges(int **a, int size)
{
	int e_num, i, j, v1, v2;

	/* initialize adjacency matrix */
	for (i=size; --i >= 0;  )
	    for (j=size; --j >= 0;  )
	        a[i][j] = 0;

	/* read edges and update adjacency matrix */
	printf("Give the number of edges: ");
	scanf("%d", &e_num);
	for (i=0; i < e_num; ++i)
	{
		printf("Give the endpoint ids of edge %d: ", i+1);
		scanf("%d %d", &v1, &v2);
		if (v1 < 0 || v1 >= size || v2 < 0 || v2 >= size || v1 == v2)
		{
			printf("Illegal edge. Ignored...\n");
			continue;
		}
		if (a[v1][v2] != 0)
		{
			printf("Duplicate edge. Ignored...\n");
			continue;
		}
		a[v1][v2] = a[v2][v1] = 1;
	}
}


void print_adj_matrix(int **a, int size)
{
	int i, j;
	
	printf("\nThe adjacency matrix of the graph (v_num = %d) is:\n", size);
	printf("           ");
	for (i=0; i < size; ++i)
	    printf(" %2d", i);
 	printf("\n");
      for (i=0;  i < size; ++i)
	{
		printf("vertex %2d: ", i);
		for (j=0;  j < size; ++j)
		     printf(" %2d", a[i][j]);
		printf("\n");
	}
}


void get_ordering(int *s, int size)
{
	int i;
	
	for (i=size; --i >= 0; )
	{
		s[i] = i;
	}
}


void print_ordering(int *ordering, int size)
{
	int i;
	
	for (i=0; i < size; ++i)
		printf(" %2d", ordering[i]);
	printf("\n");
}


void print_temp_ordering(int *s, int size, int i)
{
	int t;
	
	printf("\nstep %2d: ", i);
	for (t=0; t < i; ++t)
	    printf(" %2d", s[t]);
    	printf(" [%2d]", s[i]);
    	for (t=i; ++t < size; )
   	     printf(" %2d", s[t]);
    	printf("\n");
}


void ldfs_plus(int **a, int v_num, int *s)
{
	int i, j, v, t, *temp_n, count_n;
	
	/* allocate space for temp_n */
	temp_n = (int *)malloc(v_num*sizeof(int));
	if (temp_n == NULL)
	{
		printf("\nCannot allocate auxiliary space. Aborting...\n");
		exit(-1);
	}
	
	/* reverse the order in s */
	for (i=0, j=v_num; --j > i;  ++i)
	{
		t = s[i];
		s[i] = s[j];
		s[j] = t;
	}
	
	/* reorder s[i+1,...,v_num-1] by prepending neighbors of s[i] */
	/* maintain relative ordering of neighbors and non-neighbors  */
	for (i=0; i < v_num; ++i)
	{
 		v = s[i];
        
		/* collect neighbors of v in temp_n */
       	for (count_n=0, t=i; ++t < v_num;  )
          		if (a[v][s[t]] == 1)
				temp_n[count_n++] = s[t];
        
        	/* shift non-neighbors of v to the end of s */
       	/* first, traverse non-neighbors of v at the end of s */
      	for (t=v_num; --t > i;  )
 			if (a[v][s[t]] == 1)
				break;
		/* shift remaining non-neighbors at the end of s */
		for (j=t; --t > i;  )
			if (a[v][s[t]] == 0)
 				s[j--] = s[t];
		/* finally, copy neighbors at the beginning of s */
		if (i + count_n != j)
			printf("*** ERROR in constructing new ordering! ***\n");
		for (; --count_n >= 0;  )
			s[j--] = temp_n[count_n];
        
		/* print current oredring */
		print_temp_ordering(s, v_num, i);
	}
	
	free(temp_n);
}


void compute_paths(int **a, int v_num, int *s, int ****path, int ***length)
{
	int i, j, y, k, x, t, value;
	char *ptr;

	ldfs_plus(a, v_num, s);

	/* 2: add isolated dummy vertex un+1 to ó=(u1,u2,...,un,un+1) */
	/* there is no need to add the extra vertex if we note that: */
	/* uy in V(G(i+1,j))  where  i+1 <= y <= j                   */
	/*                <==>  j == v_num-1 || a[s[y]][s[j+1]] == 0 */
	/* ui in V(G(i,j))  <==>  j==v_num-1 || a[s[i]][s[j+1]] == 0 */

	/* 3: for i = n downto 1 do */
	for (i = v_num; i >= 0; --i)
	{
		printf("i = %d \n", i);

		/* 4: for j = i to n do */
		for (j = i; j < v_num; ++j)
		{
			printf(" j = %d \n", j);

			/* NOTE: I execute steps 7-8 before steps 5-6 */
			/* 7: if ui in V(G(i, j)); note that i <= j */
			if (j == v_num-1 || a[s[i]][s[j+1]] == 0)
			{
                  printf("  ui in V(G(i,j)) \n");
                  
				path[i][i][j][0] = s[i];
				length[i][i][j] = 1;
			}		
			
			if (i < j)  /* i < j  <==> i+1 <= j */
			{	/* G(i + 1, j) not empty */
			   printf("  G(i+1,j) is not empty \n");

				/* 5: for every uy in V(G(i + 1, j)) do */
				for (y = i+1; y <= j ; ++y)
				{
                    printf("   y = %d \n", y);
                    
					/* check if uy in V(G(i+1,j)) */
					if (j == v_num-1 || a[s[y]][s[j+1]] == 0)
					{
                          printf("    uy in V(G(i+1,j)) \n");
                          
						/* 6: P(uy;i,j) <- P(uy;i+1,j); */
						/* l(uy;i,j) <- l(uy;i+1,j); */
						length[y][i][j] = k = length[y][i+1][j];
						for ( ; --k >= 0;  )
							path[y][i][j][k] = path[y][i+1][j][k];
					}
				}

				/* 9: for every ux in V(G(i+1, j)) do */
				for(x = i+1; x <= j; ++x)
				{
                      printf("   x = %d \n", x);
                      	
					/* 10: if ui in V(G(i, j)) ... */
					if ( (j == v_num-1
							 || a[s[i]][s[j+1]] == 0)
					/* 10: ... and ux in N(ui) */
					   && a[s[x]][s[i]] == 1 )
					{
                          printf("    ui in V(G(i,j) and ux in N(ui) \n");
						/* 11: if l(ui;i,j) < l(ux;i+1,j)+1 */
						if (length[i][i][j]
							    < length[x][i+1][j] + 1)
						{
                              printf("     update path and length \n");
                              
							/* 12: P(ui;i,j) <- (P(ux;i+1,j),ui); */
							/* 13: l(ui;i,j) <- l(ux;i+1,j) + 1; */
							length[i][i][j] = k = length[x][i+1][j] + 1;
							path[i][i][j][--k] = s[i];
							for ( ; --k >= 0;  )
								path[i][i][j][k] = path[x][i+1][j][k];
						}

					 /* 14: for every uy in V(G(i+1, x-1)) do */
					for(y = i+1; y < x; ++y)
					{
                          printf("    y = %d \n", y);
                          
						/* 15: if l(uy;i,j) < l(ux;i+1,j)
								    +l(uy;i+1,x-1)+1 */
						if (length[y][i][j] < length[x][i+1][j]
							   + length[y][i+1][x-1] + 1)
						{
                             printf("     update path and length \n");
							/* 16: P(uy;i,j) <- (P(ux;i+1,j),ui,P(uy;i+1,x-1)); */
							/* 17: l(uy;i,j) <- l(ux;i+1,j)+l(uy;i+1,x-1) + 1; */
							k = length[y][i+1][x-1];
							length[y][i][j] = t = k + length[x][i+1][j] + 1;
							for ( ; --k >= 0;  )
								path[y][i][j][--t] = path[y][i+1][x-1][k];
							k = length[x][i+1][j];
							path[y][i][j][k] = s[i];
							for ( ; --k >= 0;  )
								path[y][i][j][k] = path[x][i+1][j][k];						
						}
					}
                  }
				}
			}						
		}				
	}	
}

		
int compute_max_path_length(int ***length, int v_num)
{
	int i, j, m=v_num-1, cur_max_length=0;

	for (i=v_num; --i >= 0;  )
	{
		for (j=v_num; --j >= 0;  )
		{
			if (length[i][j][m] > cur_max_length)
				cur_max_length = length[i][j][m];
		}
	}
	return(cur_max_length);
}


void print_max_paths(int ****path, int ***length, int v_num, int max_length)
{
	int i, j, k, t, m=v_num-1;

	for (i=0; i < v_num; ++i)
	{
		for (j=0; j < v_num; ++j)
		{
			if (length[i][j][m] == max_length)
			{
				printf("\nPath[%2d;%2d,%2d] = ", i, j, m);
				for (k=0; k < max_length; ++k)
					printf("%2d ", path[i][j][m][k]);
				printf("\n");
			}
		}
	}
}
		

int main()
{
	int v_num, e_num, i, max_length;
	int *sigma, **adj, ****path, ***length;

	printf("Give the number of vertices: ");
	scanf("%d", &v_num);

	/* allocate space for auxiliary matrices */
	allocate_space(v_num, &adj, &sigma, &path, &length);

	read_edges(adj, v_num);
	print_adj_matrix(adj, v_num);
	
	get_ordering(sigma, v_num);
	printf("\nThe initial ordering is:  ");
	print_ordering(sigma, v_num);

	printf("\nThe new ordering is:  ");
	print_ordering(sigma, v_num);

	compute_paths(adj, v_num, sigma, path, length);

	max_length = compute_max_path_length(length, v_num);
	printf("\nMaximum path length = %d\n", max_length - 1);

	printf("\nLongest Paths\n");
	print_max_paths(path, length, v_num, max_length);

	/* deallocate space for auxiliary matrices */
	deallocate_space(v_num, adj, sigma, path, length);
}

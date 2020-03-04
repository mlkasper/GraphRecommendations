//CSCI415; Saeed Salem, 2/10/2020
//To compile: g++ -O3 -w assign1Graph.cpp -lpthread -o assign1Graph
//To run: ./assign1Graph filename k
// ./assign1Graph toyGraph1.txt 2

#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>  /* atoi */
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <pthread.h>
#include "timer.h"
using namespace std;

typedef vector<vector<int> > AdjacencyMatrix;
AdjacencyMatrix adjMatrix;

int thread_count = 1;
int k;
int n;
int local_n;

void printAdjMatrix()
{
    for (int i=0; i<adjMatrix.size(); i++)
    {
        for (int j=0; j<adjMatrix[i].size(); j++) cout<<adjMatrix[i][j]<<" ";
        cout<<endl;
    }
}

//Outputs Friends
void printFriends()
{
    for (int i=0; i<adjMatrix.size(); i++)
    {
        for (int j=0; j<adjMatrix[i].size(); j++) 
            if(adjMatrix[i][j] == 1)
                cout<<j<<" ";            
                cout<<endl;
    }
}


//Get Friends Method
void* getFriends(void* rank)
{
   long my_rank = (long) rank;
   int r = n%thread_count; 
   int i;
   int local_n = n/thread_count; 
   long long my_first_i;
   long long my_last_i;
   
    if(my_rank <r){
        my_first_i = my_rank * local_n + (my_rank*1);
        my_last_i = my_first_i + local_n +1;
    }   
    else
        {
        my_first_i = my_rank * local_n + r;
        my_last_i = my_first_i + local_n;
        }
       
   for (i = my_first_i; i < my_last_i; i++)
    {
        vector< pair<int,int> > commonFriendsCount;
        int count;
        vector<int> commonFriends(n,0);
        for (int j= 0; j<adjMatrix.size(); j++){

            if(i!= j && adjMatrix[i][j] == 0){
                count = 0;
                 for(int k = 0; k<adjMatrix.size(); k++)
                {
                    if(adjMatrix[i][k] == 1 && adjMatrix[j][k] == 1)
                    count++;
                }
            }
            
            commonFriendsCount.push_back(make_pair(count,j));          
        }

                cout<<"Friends suggestions for " << i << endl;
        sort(commonFriendsCount.begin(), commonFriendsCount.end());
        for(int m = commonFriendsCount.size()-1 ; m >= commonFriendsCount.size()- k ;m--){
            cout<< commonFriendsCount[m].second << " : " << commonFriendsCount[m].first<<endl;
        }
    cout<<endl;
   }
}

//Main        
int main(int argc, char** argv)
{
    long thread;
    double start, finish, elapsed;
    //threading
    pthread_t* thread_handles;
    if(argc<3){
      cout<<"To run: ./assign1Graph filename k"<<endl;
      cout<<"./assign1Graph networkDatasets/toyGraph1.txt 2"<<endl;
      return 0;
    }
      
    fstream myfile(argv[1],std::ios_base::in);
    k = atoi(argv[2]);
    int u,v;
    int maxNode = 0;
    vector<pair<int,int> > allEdges;
    while(myfile >> u >> v)
    {
        allEdges.push_back(make_pair(u,v));
        if(u > maxNode)
          maxNode = u;

        if(v > maxNode)
          maxNode = v;                 
    }

    thread_handles = (pthread_t*) malloc(thread_count*sizeof(pthread_t));
    n = maxNode +1;  //Since nodes starts with 0
    cout<<"Graph has "<< n <<" nodes"<<endl;

    adjMatrix = AdjacencyMatrix(n,vector<int>(n));
    //populate the matrix
    for(int i =0; i<allEdges.size() ; i++){
       u = allEdges[i].first;
       v = allEdges[i].second;
       adjMatrix[u][v] = 1;
       adjMatrix[v][u] = 1;
      
    }
     
     //timer
   GET_TIME(start);
   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], NULL,
         getFriends, (void*)thread);

   for (thread = 0; thread < thread_count; thread++)
      pthread_join(thread_handles[thread], NULL);

    GET_TIME(finish);
    printf("Serial time = %e seconds\n", finish - start);
return 0 ;
	//You can also make a list of neighbors for each node if you want.
 
return 0;
}
//End Main

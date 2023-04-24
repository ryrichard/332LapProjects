#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "sem.h"
#include <stdbool.h>

#define Distribution_Attempts 11
#define CHILD                 0
FILE *fp1, *match, *paper, *tobacco;
int main()
{
  srand(time(NULL));

  int pid;
  int randNum;
  int status;
  int Attps = Distribution_Attempts;
  
  int agent = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
  int lock = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
  int smoker_match = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
  int smoker_paper = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
  int smoker_tobacco = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
  
  sem_create(agent, 0);
  sem_create(lock, 1);
  sem_create(smoker_match, 0);
  sem_create(smoker_paper, 0);
  sem_create(smoker_tobacco, 0);
  
  int timesMatchSmoke = 0;
  int timesPaperSmoke = 0;
  int timesTobSmoke = 0;

  bool doneSmoking, doneSmoking1, doneSmoking2;

  fp1 = fopen("attempt.txt", "w+");
  fprintf(fp1, "%d\n", Attps); 
  fclose(fp1);
  match = fopen("match.txt", "w+");
  fprintf(match, "%d\n", timesMatchSmoke); 
  fclose(match);
  paper = fopen("paper.txt", "w+");
  fprintf(paper, "%d\n", timesPaperSmoke); 
  fclose(paper);
  tobacco = fopen("tobacco.txt", "w+");
  fprintf(tobacco, "%d\n", timesTobSmoke); 
  fclose(tobacco);


  if((pid = fork()) == -1)
  {
    perror("fork");
    exit(1);
  }

  if(pid == CHILD) //AGENT
  {
    printf("AgentId : %d\n", getpid());
    for(int i = 0; i < Distribution_Attempts; i++)
    {
      P(lock);
      randNum = rand() % 3 + 1;
      fp1 = fopen("attempt.txt", "r+");
      fscanf(fp1, "%d", &Attps);
      printf("Agent At Attempt: %d\n", Attps);

      if(randNum == 1)
      {
        V(smoker_match);
        printf("Matches got picked\n");
      }
      else if(randNum == 2)
      {
        V(smoker_paper);
        printf("Paper got picked\n");

      }
      else
      {
        V(smoker_tobacco);
        printf("Tob got picked\n");
      }
      fseek(fp1, 0L, 0);
      Attps--;
      fprintf(fp1, "%d \n", Attps);
      fclose(fp1);
      V(lock);
      P(agent);
    }
    printf("No more resources\n");
    V(smoker_match);
    V(smoker_paper);
    V(smoker_tobacco);
  }
  else
  {
    if((pid = fork()) == -1) 
    {
        perror("fork");
        exit(1);
    }
    if(pid == CHILD) //SMOKER_MATCH
    {
      printf("MatchID: %d\n", getpid());
      doneSmoking = false;
      while(doneSmoking == false)
      {
        P(smoker_match);
        P(lock);

        fp1 = fopen("attempt.txt", "r+");
        fscanf(fp1, "%d", &Attps);        
        fclose(fp1);
        printf("Match Attempts: %d\n", Attps);
        if(Attps == 0)
        {
          printf("NO MORE SMOKING FOR MATCH\n");
          doneSmoking = true;
        }
        else
        {
          match = fopen("match.txt", "r+");
          fscanf(match, "%d", &timesMatchSmoke);
          timesMatchSmoke++;
          fseek(match, 0L, 0);
          fprintf(match, "%d \n", timesMatchSmoke);
          fclose(match);
          printf("Matches smoked %d times\n", timesMatchSmoke);
        }
        V(agent);        
        V(lock);
      }
    }
    else
    {
      if((pid = fork()) == -1) 
      {
        perror("fork");
        exit(1);
      }
      if(pid == CHILD) //SMOKER_PAPER
      {
        printf("PaperID: %d\n", getpid());
        doneSmoking1 = false;
        while(doneSmoking1 == false)
        {
          P(smoker_paper);
          P(lock);

          fp1 = fopen("attempt.txt", "r+");
          fscanf(fp1, "%d", &Attps);  
          fclose(fp1);  
          printf("Paper Attempts: %d\n", Attps);

          if(Attps == 0)
          {
            printf("NO MORE SMOKING FOR PAPER\n");
            doneSmoking1 = true;
          }
          else
          {
            paper = fopen("paper.txt", "r+");
            fscanf(paper, "%d", &timesPaperSmoke);
            timesPaperSmoke++;
            fseek(paper, 0L, 0);
            fprintf(paper, "%d \n", timesPaperSmoke);
            fclose(paper);            
            printf("Paper smoked %d times\n", timesPaperSmoke);        
          }
          V(agent);
          V(lock);
        }

      }
      else
      {
        if((pid = fork()) == -1)
        {
          perror("fork");
          exit(1);
        }
        if(pid == CHILD) //SMOKER_TOBACCO
        {
          printf("TobaccoID: %d\n", getpid());
          doneSmoking2 = false;
          while(doneSmoking2 == false)
          {
            P(smoker_tobacco);
            P(lock);

            fp1 = fopen("attempt.txt", "r+");
            fscanf(fp1, "%d", &Attps);   
            fclose(fp1);
            printf("Tobacco Attempts: %d\n", Attps);

            if(Attps == 0)
            {
              printf("NO MORE FOR TOBACCO\n");
              doneSmoking2 = true;
            }
            else
            {
              tobacco = fopen("tobacco.txt", "r+");
              fscanf(tobacco, "%d", &timesTobSmoke);
              timesTobSmoke++;
              fseek(tobacco, 0L, 0);
              fprintf(tobacco, "%d \n", timesTobSmoke);
              fclose(tobacco); 
              
              printf("Tob smoked %d times\n", timesTobSmoke);
            }
            V(agent);
            V(lock); 
          }
        }
        else
        {
          pid == wait(&status);
          printf("child %d died\n", pid);

          pid == wait(&status);
          printf("child %d died\n", pid);

          pid == wait(&status);
          printf("child %d died\n", pid);

          pid == wait(&status);
          printf("child %d died\n", pid);

          match = fopen("match.txt", "r+");
          fscanf(match, "%d", &timesMatchSmoke);
          fclose(match);

          paper = fopen("paper.txt", "r+");
          fscanf(paper, "%d", &timesPaperSmoke);
          fclose(paper);            

          tobacco = fopen("tobacco.txt", "r+");
          fscanf(tobacco, "%d", &timesTobSmoke);
          fclose(tobacco); 

          printf("\n----Final Results----\nMatches Smoked %d times\nPaper Smoked %d times\nTobacco Smoked %d times\n", timesMatchSmoke, timesPaperSmoke, timesTobSmoke);
        }
        exit(0);
      }
      exit(0);
    }
    exit(0);
  }
  exit(0);
}

//     do forever {
//    P( lock );
//     randNum = rand( 1, 3 ); // Pick a random number from 1-3
//     if ( randNum == 1 ) {
//        // Put tobacco on table
//         // Put paper on table
//         V( smoker_match );  // Wake up smoker with match
//        } else if ( randNum == 2 ) {
//          // Put tobacco on table
//         // Put match on table
//         V( smoker_paper );  // Wake up smoker with paper
//       } else {
//         // Put match on table
//         // Put paper on table
//         V( smoker_tobacco ); } // Wake up smoker with tobacco
//       V( lock );
//       P( agent );  //  Agent sleeps
//      }  // end forever loop

// //I will give code to one of the smokers. The others are analogous.

//     do forever {
//       P( smoker_tobacco );  // Sleep right away
//        P( lock );
//        // Pick up match
//       // Pick up paper
//       V( agent );
//       V( lock );
//       // Smoke (but don't inhale).
//     }



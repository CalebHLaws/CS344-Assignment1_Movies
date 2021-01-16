
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Movie{
  char *title;
  int year;
  char *languages;
  double rating;
  struct Movie *next;
}Movie;

/* Parse the current line which is space delimited and create a
*  Movie struct with the data in this line
*/
struct Movie *createMovie(char *currLine)
{
    struct Movie *currMovie = malloc(sizeof(struct Movie));

    // For use with strtok_r
    char *saveptr;

    // The first token is the title
    char *token = strtok_r(currLine, ",", &saveptr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

    // The next token is the year
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->year = atoi(token);

    // The next token is the languages
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->languages = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->languages, token);

    // The last token is the rating
    token = strtok_r(NULL, "\n", &saveptr);
    currMovie->rating = strtod(token,NULL);

    // Set the next node to NULL in the newly created Movie entry
    currMovie->next = NULL;

    return currMovie;
}

/*
* Return a linked list of Movies by parsing data from
* each line of the specified file.
*/
struct Movie *processFile(char *filePath)
{
    // Open the specified file for reading only
    FILE *movieFile = fopen(filePath, "r");

    char *currLine = NULL;
    size_t len = 0;
    ssize_t nread;
    char *token;

    // The head of the linked list
    struct Movie *head = NULL;
    // The tail of the linked list
    struct Movie *tail = NULL;

    //Skip First line of CSV 
    nread = getline(&currLine, &len, movieFile);

    // Read the file line by line
    while ((nread = getline(&currLine, &len, movieFile)) != -1)
    {
        // Get a new Movie node corresponding to the current line
        struct Movie *newNode = createMovie(currLine);

        // Is this the first node in the linked list?
        if (head == NULL)
        {
            // This is the first node in the linked link
            // Set the head and the tail to this node
            head = newNode;
            tail = newNode;
        }
        else
        {
            // This is not the first node.
            // Add this node to the list and advance the tail
            tail->next = newNode;
            tail = newNode;
        }
    }
    free(currLine);
    fclose(movieFile);
    return head;
}

/*
* Print data for the given Movie
*/
void printMovie(struct Movie* aMovie){
  printf("%s, %d %s, %.1f\n", aMovie->title,
               aMovie->year,
               aMovie->languages,
               aMovie->rating);
}
/*
* Print the linked list of Movies
*/
void printMovieList(struct Movie *list)
{
    while (list != NULL)
    {
        printMovie(list);
        list = list->next;
    }
}

/*
* Returns number of movies
*/
int countMovies(struct Movie *list){
  int count=0;
  while (list != NULL)
    {
        count++;
        list = list->next;
    }
  return count;
}

/*
* Takes list of movies and prints a list of movies from a certain year
*/
void printYear(struct Movie *list, int option){
  printf("\n");
  int None = 1;
  while (list != NULL){
        if (list->year == option){
          printf("%s\n",list->title);
          None = 0;
        }
        list = list->next;
    }
  if(None){
    printf("No movies for the year: %i\n",option);
  }
  printf("\n");
}

/*
* Checks to see if the year has already been checked
*/
int notInBlacklist(int testyear,int* blacklist){
  int i=0;
    while(blacklist[i]!=-1){
      if(testyear == blacklist[i]){
        //printf("Skipped year %i\n",testyear); //FOR TESTING
        return 0;        
      }
      i++;
    }
  return 1;
}

/*
* Prints the highest rated movie of a given year
*/
void printHighestYear(int testyear,struct Movie *list){
  //printf("Checking year: %i\n",testyear); FOR TESTING
  struct Movie *highest = malloc(sizeof(struct Movie));
  highest->rating = -1;
  while(list!=NULL){
    if(list->year == testyear){
      if(list->rating > highest->rating){
        highest->title = list->title;
        highest->rating = list->rating;
      }
    }
    list=list->next;
  }
  printf("%i: %s , %.1f\n",testyear,highest->title,highest->rating);
  free(highest);
}
/*
* Adds number to the blacklist
*/
void updateBlacklist(int num,int* blacklist){
  int i =0;
  while(blacklist[i]!=-1){
    i++;
  }
  blacklist[i]=num;
  //printf("Blacklisted: %i\n",num); //FOR TESTING
}

/*
* Prints the highest rated movie for each year
*/
void highestRated(struct Movie *list){
  printf("\n");
  int count = countMovies(list)+1;
  int* blacklist = calloc(count,sizeof(int));
  
  for(int i=0;i<count;i++){
    blacklist[i] = -1;
  }

  while (list != NULL){
      if(notInBlacklist(list->year,blacklist)){
        printHighestYear(list->year,list);
        updateBlacklist(list->year,blacklist);
      }
      list = list->next;
    }
  printf("\n\n");
  free(blacklist);
}

/*
* Prints each movie that has a certain language
*/
void printMoviesWithLanguage(struct Movie *list,char* language){
  int None = 1;
  while (list != NULL)
    {
        if (strstr(list->languages,language)){
          printf("%s\n",list->title);
          None = 0;
        }
        list = list->next;
    }
  if(None){
    printf("No movies for the year: %s\n",language);
  }
  printf("\n");
}

/*
*   Process the file provided as an argument to the program to
*   create a linked list of Movie structs and print out the list.
*   Compile the program as follows:
*       gcc --std=gnu99 -o movies main.c
*/
int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movie_info1.txt\n");
        return EXIT_FAILURE;
    }
    struct Movie *list = processFile(argv[1]);
    //printMovieList(list);  //FOR TESTING
    printf("\n");
    int numberOfMovies = countMovies(list);
    printf("Processed file %s and parsed data for %i movies\n\n",argv[1],numberOfMovies);
    int choice,option;
    char* language;
    
    while(1==1){
      printf("1. Show movies released in the specified year\n");
      printf("2. Show highest rated movie for each year\n");
      printf("3. Show the title and year of realease of all movies in a specific language\n");
      printf("4. Exit from the program\n");
      printf("\nEnter a choice from 1 to 4: ");
      scanf("%i",&choice);
      if (choice == 1){
        printf("Enter the year for which you want to see movies: ");
        scanf("%i",&option);
        printYear(list,option);
      }
      else if (choice == 2){
        highestRated(list);
      }
      else if (choice == 3){
        printf("Enter the language for which you want to see movies: ");
        scanf("%s",language);
        printMoviesWithLanguage(list,language);
      }
      else if (choice == 4){
        return EXIT_SUCCESS;
      }
      else{
        printf("ERROR: Please input a valid input\n\n");
      }
      

    }    
}

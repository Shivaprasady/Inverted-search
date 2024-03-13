#define SUCCESS 0
#define FAILURE 1


#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

typedef struct Inverted_info
{
	char file_name[20];
	FILE *file_ptr;
	struct Inverted_info *link;
} InvertedInfo;

typedef struct Main_node
{
	int file_count;
	char word[20];
	struct Sub_node *Slink;
	struct Main_node *Mlink;
} mainnode;

typedef struct Sub_node
{
	int word_count;
	char file_name[20];
	struct Sub_node *slink;
} subnode;

typedef struct Hash_t
{
	int index;
	struct Main_node *Mainlink;
} hash;



// function declaration
int create(int argc,char *argv[],hash *HT);
int read_and_validate(int argc,char *argv[],InvertedInfo **head);
void check_duplicates(InvertedInfo **head);
int open_files(InvertedInfo **head);
void create_HT(hash *HT);
int create_database(InvertedInfo **head,hash *HT);
int travers_and_update(int i,hash *HT,char *word,InvertedInfo *temp);
int search_details(hash *HT,char search[20]);
void display_database(hash *HT);

int save_database(hash *HT,char save_file[20]);

int update_database(char *backup_file,hash *arr);

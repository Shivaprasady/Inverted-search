#include"invert.h"

int main(int argc,char *argv[])
{
	char backup_file[20];
	//creating hash table
	hash arr[28];
	create_HT(arr);

	int flag = 0;

	char save_file[20];
	char search[20];
	char update_file[20];

	int choice;
	char continu = 'y';
	while(continu == 'y' || continu  == 'Y')
	{
		printf("Select your choice amaong following option : \n");
		printf("1. Create DATABASE\n2. Display DATABASE\n3. Update DATABASE\n4. Search\n5.Save DATABASE\nEnter your choice\n");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1:
				if(flag > 0)
				{
					printf("Created database\n");
				}
				else
				{
					if(create(argc,argv,arr) == FAILURE)
					{
						printf("Error : unable to create database\n");
						return FAILURE;
					}
				}
				break;
			case 2:
				display_database(arr);
				break;
			case 3:
				//update database
				printf("Enter the file name to update data : ");
				scanf("%s",backup_file);
				if(update_database(backup_file,arr) == SUCCESS)
				{
					flag++;
					printf("Update files SUCCESSFUL from %s\n",backup_file);
				}
				else
				{
					printf("Update file FAILED from %s\n",backup_file);
					return FAILURE;
				}



				break;
			case 4:
				printf("Enter the word you want to search : ");
				scanf("%s",search);
				if(search_details(arr,search) == FAILURE)
				{
					printf("Word \"%s\" not found\n",search);
				}
				break;
			case 5:
				printf("Enter the file name to save database : ");
				scanf("%s",save_file);
				if(save_database(arr,save_file) == FAILURE)
				{
					printf("pass file name in .txt format\n");
				}
				else
				{
					printf("Save database SUCCESSFUL\n");
				}
				break;
			default:
				printf("Wrong input\n");


		}

		getchar();
		printf("Do you want to continue ?(y/Y) : ");
		scanf("%c",&continu);
	}
}



int create(int argc,char *argv[],hash *HT)
{
	InvertedInfo *head = NULL;
	if(argc > 1)
	{
		if(read_and_validate(argc,argv,&head) == FAILURE)
		{
			printf("Please pass correct argument\n");
			return FAILURE;
		}


		check_duplicates(&head);

		if(open_files(&head) == FAILURE)
		{
			return FAILURE;
		}

		if(create_database(&head,HT) == FAILURE)
		{
			return FAILURE;
		}
		printf("Data base created for ");
		InvertedInfo *temp = head;
		while(temp != NULL)
		{
			printf("%s\t",temp->file_name);
			temp = temp->link;
		}
		printf("\n");
	}
	else
	{
		printf("Please pass correct arguments\n");
		return FAILURE;
	}


}
int read_and_validate(int argc, char *argv[], InvertedInfo **head) {
	InvertedInfo *new_node;
	InvertedInfo *current;

	// Traverse the list to find the last node
	current = *head;
	while (current != NULL && current->link != NULL) {
		current = current->link;
	}

	for (int i = 1; i < argc; i++) {
		char *ptr = strstr(argv[i], ".txt");
		if (ptr == NULL || strcmp(ptr, ".txt") != 0) {
			return FAILURE;
		}

		new_node = malloc(sizeof(InvertedInfo));
		if (new_node == NULL) {
			return FAILURE;
		}

		strcpy(new_node->file_name, argv[i]);
		new_node->link = NULL;

		// If the list is empty, update the head
		if (*head == NULL) {
			*head = new_node;
		} else {
			// Otherwise, append the new node at the end
			current->link = new_node;
		}

		// Update the 'current' pointer to the new last node
		current = new_node;
	}

	return SUCCESS; // Assuming SUCCESS is defined elsewhere in your code
}



void check_duplicates(InvertedInfo **head) {
	InvertedInfo *temp1 = *head;

	// Outer loop to traverse the list
	while (temp1 != NULL && temp1->link != NULL) {
		InvertedInfo *prev = temp1;  // Previous node to track
		InvertedInfo *temp2 = temp1->link;

		// Inner loop to compare with subsequent nodes
		while (temp2 != NULL) {
			if (strcmp(temp1->file_name, temp2->file_name) == 0) {
				printf("Duplicate file found: %s\n", temp2->file_name);

				// Remove the duplicate node
				prev->link = temp2->link;
				free(temp2);
				temp2 = prev->link;  // Move temp2 to the next node
			} else {
				// Move both temp1 and temp2 pointers forward
				prev = temp2;
				temp2 = temp2->link;
			}
		}
		// Move temp1 to the next node for the outer loop
		temp1 = temp1->link;
	}
}


int open_files(InvertedInfo **head)
{
	InvertedInfo *temp = *head;
	while(temp != NULL)
	{
		temp->file_ptr = fopen(temp->file_name,"r");
		if(temp->file_ptr == NULL)
		{
			printf("Error : unable to open file - %s\n",temp->file_name);
			return FAILURE;
		}

		//checking file is empty or not

		fseek(temp->file_ptr,0,SEEK_END);

		if(ftell(temp->file_ptr) == 0)
		{
			printf("Error : file is empty\n");
			return FAILURE;
		}

		temp = temp->link;
	}

	return SUCCESS;
}



void create_HT(hash *HT)
{
	for(int i = 0 ; i < 28 ; i++)
	{
		HT[i].index = i;
		HT[i].Mainlink = NULL;
	}
}

int create_database(InvertedInfo **head,hash *HT)
{

	char word[20];
	char check_character;


	InvertedInfo *temp = *head;
	while(temp != NULL)
	{
		//printf("%s\n",temp->file_name);
		rewind(temp->file_ptr);
		while(fscanf(temp->file_ptr,"%s",word) != EOF)
		{

			char check_character = tolower(word[0]);
			if(check_character >= 97 && check_character <= 122)
			{
				if(travers_and_update(check_character-97,HT,word,temp) == FAILURE)
				{
					printf("Error : could not able to update DATA_BASE\n");
					return FAILURE;
				}
			}
			else if(check_character >= '0' && check_character <= '9')
			{
				if(travers_and_update(26,HT,word,temp) == FAILURE)
				{
					printf("Error : could not able to update DATA_BASE\n");
					return FAILURE;
				}
			}
			else
			{
				if(travers_and_update(27,HT,word,temp) == FAILURE)
				{
					printf("Error : could not able to update DATA_BASE\n");
					return FAILURE;
				}
			}
		}
		temp = temp->link;
	}
}


int travers_and_update(int i,hash *HT,char *word,InvertedInfo *temp)
{
	//printf("%s\n",temp->file_name);
	if(HT[i].Mainlink == NULL)
	{
		//	printf("%s - ",word);
		mainnode *mainnode_new = malloc(sizeof(mainnode));
		if(mainnode_new == NULL)
		{
			return FAILURE;
		}
		strcpy(mainnode_new->word,word);
		mainnode_new->Mlink = NULL;
		mainnode_new->file_count = 1;

		HT[i].Mainlink = mainnode_new;

		subnode *subnode_new = malloc(sizeof(subnode));
		if(subnode_new == NULL)
		{
			return FAILURE;
		}

		mainnode_new->Slink = subnode_new;

		subnode_new->word_count = 1;
		strcpy(subnode_new->file_name,temp->file_name);
		subnode_new->slink = NULL;
		//	printf("%s\n",mainnode_new->word);
	}
	else if(HT[i].Mainlink != NULL)
	{
		//		printf("%s - ",word);
		//	int flag = 0;
		mainnode *mainnode_temp = HT[i].Mainlink;
		mainnode *mainnode_prev = mainnode_temp;
		while(mainnode_temp != NULL)
		{
			mainnode_prev = mainnode_temp;

			subnode *subnode_temp = mainnode_temp->Slink;
			subnode *subnode_prev = subnode_temp;

			if(strcmp(mainnode_temp->word,word) == 0)
			{
				subnode_temp = mainnode_temp->Slink;
				subnode_prev = subnode_temp;
				while(subnode_temp != NULL)
				{
					if(strcmp(subnode_temp->file_name,temp->file_name) == 0 )
					{
						++(subnode_temp->word_count);
						return SUCCESS;
					}
					subnode_temp = subnode_temp->slink;
				}
				if(subnode_prev->slink == NULL)
				{
					subnode *new = malloc(sizeof(subnode));
					if(new == NULL)
					{
						return FAILURE;
					}

					strcpy(new->file_name,temp->file_name);
					new->word_count = 1;
					++(mainnode_temp->file_count);

					subnode_prev->slink = new;
					return SUCCESS;
				}
			}
			mainnode_temp = mainnode_temp->Mlink;
		}


		if(mainnode_prev->Mlink == NULL)
		{

			mainnode *mainnode1_new = malloc(sizeof(mainnode));
			strcpy(mainnode1_new->word,word);
			++(mainnode1_new->file_count);
			mainnode1_new->Mlink = NULL;

			subnode *subnode1_new = malloc(sizeof(subnode));
			subnode1_new->word_count = 1;
			strcpy(subnode1_new->file_name,temp->file_name);
			subnode1_new->slink = NULL;

			mainnode1_new->Slink = subnode1_new;

			mainnode_prev->Mlink = mainnode1_new;

			//			printf("%s\n",mainnode1_new->word);
			return SUCCESS;
		}
	}
}





int search_details(hash *HT,char search[20])
{
	int index; 
	char check = tolower(search[0]);
	if(check >= 97 && check <= 122)
	{
		index = check - 97;
	}
	else if(check >= '0' && check <= '9')
	{
		index  = 26;
	}
	else
	{
		index = 27;
	}

	mainnode *mainnode_temp = HT[index].Mainlink;

	while(mainnode_temp != NULL )
	{
		if(strcmp(mainnode_temp->word,search) == 0)
		{
			printf("the word \"%s\" is present in %d file(s)\n",mainnode_temp->word,mainnode_temp->file_count);

			subnode *subnode_temp = mainnode_temp->Slink;
			while(subnode_temp != NULL )
			{
				printf("In \"%s\" for \"%d\" time(s)\n",subnode_temp->file_name,subnode_temp->word_count);

				subnode_temp = subnode_temp->slink;
			}
			return SUCCESS;
		}
		mainnode_temp = mainnode_temp->Mlink;
	}
	return FAILURE;

}

void display_database(hash *HT)
{
	printf("Index\tword\tfile count\tfile(s) File name\tword count\n");
	printf("------------------------------------------------------------------------------\n");
	for(int i = 0 ; i < 28 ; i++)
	{
		if(HT[i].Mainlink != NULL)
		{

			mainnode *mainnode_temp = HT[i].Mainlink;
			while(mainnode_temp != NULL)
			{
				printf("[%d]",HT[i].index);
				printf("\t[ %s ]\t%d\tfile(s) FILE : ",mainnode_temp->word,mainnode_temp->file_count);

				subnode *subnode_temp = mainnode_temp->Slink;
				while(subnode_temp != NULL)
				{
					printf("\t%s\t%d",subnode_temp->file_name,subnode_temp->word_count);
					subnode_temp = subnode_temp->slink;
				}
				printf("\n");	
				mainnode_temp = mainnode_temp->Mlink;
			}

		}
	}

}

int save_database(hash *HT,char save_file[20])
{
	char *ptr = strstr(save_file,".txt");
	if(ptr == NULL)
	{
		return FAILURE;
	}
	if(strcmp(ptr,".txt") == 0)
	{
		FILE *save_file_ptr = fopen(save_file,"w");

		for(int i = 0 ; i < 28 ; i++)
		{
			if(HT[i].Mainlink != NULL)
			{

				mainnode *mainnode_temp = HT[i].Mainlink;
				while(mainnode_temp != NULL)
				{
					fprintf(save_file_ptr,"#%d",HT[i].index);
					fprintf(save_file_ptr,";%s;%d;",mainnode_temp->word,mainnode_temp->file_count);

					subnode *subnode_temp = mainnode_temp->Slink;
					while(subnode_temp != NULL)
					{
						fprintf(save_file_ptr,";%s;%d",subnode_temp->file_name,subnode_temp->word_count);
						subnode_temp = subnode_temp->slink;
					}
					fprintf(save_file_ptr,"#\n");	
					mainnode_temp = mainnode_temp->Mlink;
				}

			}
		}
	}

}

int update_database(char *backup_file,hash *arr)
{
	//open saved file in read mode
	FILE *fptr = fopen(backup_file,"r");
	if(fptr == NULL)
		return FAILURE;
	char check[20];
	fscanf(fptr,"%s",check);
	//check the file is saved file or not
	if(check[0] != '#')
		return FAILURE;
	rewind(fptr);

	char word[100];
	//read a file until reaches end
	while(fscanf(fptr,"%s",word) != EOF)
	{
		//getting index
		int ind = atoi(strtok(&word[1],";"));
		//check for index link
		if(arr[ind].Mainlink == NULL)
		{
			//create node update data and establish connection
			mainnode *m_new = malloc(sizeof(mainnode));
			subnode *s_new = malloc(sizeof(subnode));
			strcpy(m_new->word,strtok(NULL,";"));
			m_new->file_count = atoi(strtok(NULL,";"));
			m_new->Mlink = NULL;
			strcpy(s_new->file_name,strtok(NULL,";"));
			s_new->word_count = atoi(strtok(NULL,";"));
			s_new->slink = NULL;
			m_new->Slink = s_new;;
			subnode *temp = s_new;
			for(int i = 0; i < (m_new->file_count - 1); i++)
			{
				//creation of sub node update data and establish connection
				subnode *s_snew = malloc(sizeof(subnode));
				strcpy(s_snew->file_name,strtok(NULL,";"));
				s_snew->word_count = atoi(strtok(NULL,";"));
				s_new->slink = NULL;
				temp->slink = s_snew;
				temp = s_snew;

			}
			arr[ind].Mainlink = m_new;
		}
		else
		{
			//if index having node traverse till last main node
			mainnode *wn_addr = arr[ind].Mainlink;
			while(wn_addr->Mlink != NULL)
			{
				wn_addr = wn_addr->Mlink;
			}
			//create main node update data and establish connection
			mainnode *m_new = malloc(sizeof(mainnode));
			subnode *s_new = malloc(sizeof(subnode));
			strcpy(m_new->word,strtok(NULL,";"));
			m_new->file_count = atoi(strtok(NULL,";"));
			m_new->Mlink = NULL;
			strcpy(s_new->file_name,strtok(NULL,";"));
			s_new->word_count = atoi(strtok(NULL,";"));
			s_new->slink = NULL;
			m_new->Slink = s_new;;
			subnode *temp = s_new;

			for(int i = 0; i < (m_new->file_count - 1); i++)
			{
				//create sub node update data and establish connnection
				subnode *s_snew = malloc(sizeof(subnode));
				strcpy(s_snew->file_name,strtok(NULL,";"));
				s_snew->word_count = atoi(strtok(NULL,";"));
				s_new->slink = NULL;
				temp->slink = s_snew;
				temp = s_snew;

			}
			wn_addr->Mlink = m_new;
		}
	}
	return SUCCESS;
}

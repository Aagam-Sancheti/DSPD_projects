#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define size 30
// structure for individual
typedef struct individual_list
{
    int ID;
    char Name[size];
    int Age;
    int Dailystep_goal;
    int a_count[7];
    struct individual_list *next;
} i_list;

// structure for groups
typedef struct group_list
{
    int G_id;
    char G_name[size];
    i_list *members[5];
    int G_goal;
    int Gsize;
    int sum;
    struct group_list *next;
} g_list;

i_list *ihead = NULL;
g_list *ghead = NULL;

// creating indivial node
i_list *create_individual(int ID, char *Name, int Age, int Dailystep_goal, int a_count[7])
{
    i_list *new_individual = (i_list *)malloc(sizeof(i_list));
    if (new_individual == NULL)
    {
        printf("memory allocation failed");
        return NULL;
    }
    new_individual->ID = ID;
    strcpy(new_individual->Name, Name);
    new_individual->Age = Age;
    new_individual->Dailystep_goal = Dailystep_goal;
    for (int i = 0; i < 7; i++)
    {
        new_individual->a_count[i] = a_count[i];
    }
    new_individual->next = NULL;
    return new_individual;
}

// adding person to linkedlist of individuals
void Add_Person(i_list **ihead, i_list *new_individual)
{
    i_list *ptr = NULL;
    i_list *p = NULL;
    int flag = 0;
    ptr = new_individual;
    ptr->next = NULL;
    if (*ihead == NULL)
    {
        *ihead = ptr;
    }
    else
    {
        i_list *nptr = *ihead;
        int flag = 0;
        while ((*ihead != NULL) && (flag == 0))
        {
            if (nptr->ID < ptr->ID)
            {
                p = nptr;
                nptr = nptr->next;
            }
            else
            {
                flag = 1;
            }
        }
        if (p != NULL)
        {
            p->next = ptr;
            ptr->next = nptr;
        }
        else
        {
            ptr->next = nptr;
            *ihead = ptr;
        }
    }
}

g_list *create_group(int id, char *name, int *mem, int goal, int siz)
{
    g_list *new_group = (g_list *)malloc(sizeof(g_list));
    new_group->G_id = id;
    strcpy(new_group->G_name, name);
    new_group->G_goal = goal;
    new_group->next = NULL;

    i_list *pnode;
    int k = 0;
    while (k < siz)
    {
        int search_id = mem[k];
        pnode = ihead;
        while (pnode != NULL && pnode->ID != search_id)
        {
            pnode = pnode->next;
        }

        new_group->members[k] = pnode;
        k++;
    }

    new_group->Gsize = k;

    return new_group;
}

// inserting a group in linkedlist of groups
void insert_group(g_list **ghead, g_list *gnode)
{
    if (*ghead == NULL)
    {
        *ghead = gnode;
    }
    else
    {
        g_list *current = *ghead;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = gnode;
    }
}

void add_person_to_group(i_list *ihead, g_list *ghead, int Iid, int Gid)
{
    i_list *pnode = ihead;
    int flag = 0;
    int foundPerson = 0;
    while (pnode != NULL && flag == 0)
    {
        if (pnode->ID == Iid)
        {
            foundPerson = 1;
            flag = 1;
        }
        pnode = pnode->next;
    }
    if (foundPerson == 0)
    {
        printf("Person with ID %d does not exist.\n", Iid);
    }
    else
    {
        g_list *curr = ghead;
        int foundGroup = 0;
        int flag1 = 0;
        while (curr != NULL && flag1 == 0)
        {
            int foundMember = 0;
            for (int i = 0; i < curr->Gsize; i++)
            {
                if (curr->members[i]->ID == Iid)
                {
                    printf("Person with ID %d already belongs to group %s.\n", Iid, curr->G_name);
                    foundMember = 1;
                    flag = 1;
                }
            }
            if (foundMember == 1)
            {
                foundGroup = 1;
                break;
            }
            curr = curr->next;
        }
        if (foundGroup == 1)
        {
            return;
        }
        else
        {
            g_list *gnode = ghead;
            while (gnode != NULL && gnode->G_id != Gid)
            {
                gnode = gnode->next;
            }
            if (gnode == NULL)
            {
                printf("Group with ID %d does not exist.\n", Gid);
            }
            else if (gnode->Gsize == 5)
            {
                printf("Group %s is full.\n", gnode->G_name);
            }
            gnode->members[gnode->Gsize] = pnode;
            gnode->Gsize++;
            printf("Person %s added to group %s.\n", pnode->Name, gnode->G_name);
        }
    }
}

// void add_person_to_group(i_list *ihead,g_list*ghead, int pid, int gid) {
// 	int flag=0;
//     i_list *pnode = ihead; // Start from the next of the person list
//     while (pnode != NULL && pnode->ID != pid) { // Traverse the list until the person is found or the end is reached
//         pnode = pnode->next;
//     }
//     if (pnode == NULL) { // If the person is not found
//         printf("Person with ID %d does not exist.\n", pid);
//         return;
//     }
//     g_list* temp=ghead;
// 	while(temp!=NULL && flag==0 ){
// 		for(int i=0;i<5;i++){
// 			if(temp->members[i]->ID==pid){
//                flag=1;
// 			}
// 		temp=temp->next;
// 	}
// 	if(flag==1){
// 		printf("individual is already in a group");
// 	}
// 	else{
// 		g_list *gnode = ghead;
//     while (gnode != NULL && gnode->G_id != gid) {
//         gnode = gnode->next;
//     }
//     if (gnode == NULL) {
//         printf("Group with ID %d does not exist.\n", gid);
//         return;
//     }
//     if (gnode->Gsize == 5) {
//         printf("Group %s is full.\n", gnode->G_name);
//         return;
//     }
//     gnode->members[gnode->Gsize] = pnode; // Add the person to the group
//     gnode->Gsize++;
//     printf("Person %s added to group %s.\n", pnode->Name, gnode->G_name);

// 	}
// }
// }

void get_top_3(i_list *ihead)
{
    i_list *top[3] = {NULL, NULL, NULL}; // Array to store  the individuals
    int total[3] = {0, 0, 0};            // Array to store the total no of steps of the top 3 nodes
    i_list *current = ihead;
    int flag1;

    while (current != NULL)
    {
        int flag1 = 0;
        for (int i = 0; i < 7 && flag1 == 0; i++)
        {
            if (current->Dailystep_goal > current->a_count[i])
            {
                flag1 = 1;
            }
        }

        if (flag1 == 0)
        {
            int sum = 0;
            int flag = 1;
            for (int i = 0; i < 7; i++)
            {
                sum = sum + current->a_count[i];
            }
            for (int i = 0; i < 3; i++)
            {
                if (sum > total[i] && flag == 1)
                {
                    for (int j = 2; j > i; j--)
                    {
                        top[j] = top[j - 1];
                        total[j] = total[j - 1];
                    }
                    top[i] = current;
                    total[i] = sum;
                    flag = 0;
                }
            }
        }

        current = current->next;
    }
    printf("The top 3 individuals are:\n");
    for (int i = 0; i < 3; i++)
    {
        if (top[i] != NULL)
        {
            printf("%d. %s with %d steps\n", i + 1, top[i]->Name, total[i]);
        }
    }
}

/*void top3_individuals(i_list* lptr){

    int flag=0;int sum=0;int max=-1;
    i_list* ptr;
    i_list * temp=lptr;
    i_list * temp1=lptr;
   if(temp!=NULL){
    for(int i=0;i<7 && flag==0;i++){
        if(temp->Dailystep_goal>temp->a_count[i]){
            flag=1;
        }

      }
   }
   if(flag==0){
    for(int j=0;j<=7;j++){
    sum=sum+temp1->a_count[j];
    }
    if(sum>max){
        max=sum;
        i_list*ptr=temp1;
    }
    temp=temp->next;

   }
   else{
    temp1=temp1->next;
   }

   printf("the top individual is name:%s  id:%d ",ptr->Name,&ptr->ID);
}
*/

/*void top3_individuals(i_list* lptr){

    int flag=0;int sum=0;int max1,max2,max3;
    i_list* ptr;
    i_list * temp=lptr;
    i_list * temp1=lptr;
    i_list * temp2=lptr;
    i_list* t1 = NULL;
    i_list* t2 = NULL;
    i_list* t3 = NULL;
   if(temp!=NULL){
    for(int i=0;i<7 && flag==0;i++){
        if(temp->Dailystep_goal>temp->a_count[i]){
            flag=1;
        }

      }
   }
   if(flag==0){
    for(int j=0;j<=7;j++){
    sum=sum+temp1->a_count[j];
    }
    temp1->i_sum=sum;

    temp=temp->next;

   }
   else{
    temp1->i_sum=0;
    temp1=temp1->next;
   }

   max1=max2=max3=temp2->i_sum;
   while(temp2!=NULL){
    if(temp2->i_sum>max1){
        max3=max2;
        max2=max1;
        max1=temp2->i_sum;
    }
    else if (temp2->i_sum>max2){
        max3=max2;
        max2=temp2->i_sum;
    }
    else if(temp2->i_sum>max3){
        max3=temp2->i_sum;
    }
    temp2=temp2->next;
   }

   printf("the top individual is name:%s  id:%d ",ptr->Name,&ptr->ID);
}*/

void check_group_achievement(int gid)
{
    g_list *gnode = ghead;
    while (gnode != NULL && gnode->G_id != gid)
    {
        gnode = gnode->next;
    }
    if (gnode == NULL)
    { // If the group is not exit in ghead
        printf("Group with ID %d does not exist.\n", gid);
    }
    else
    {
        int total_steps = 0;
        for (int i = 0; i < gnode->Gsize; i++)
        {
            for (int j = 0; j < 7; j++)
            {
                total_steps += gnode->members[i]->a_count[j];
            }
        }
        if (total_steps >= gnode->G_goal)
        {
            printf("Group %s has achieved its weekly goal with %d steps.\n", gnode->G_name, total_steps);
        }
        else
        {
            printf("Group %s has not achieved its weekly goal. Total steps: %d\n", gnode->G_name, total_steps);
        }
    }
}

g_list *Gmerge(g_list *lptr1, g_list *lptr2)
{
    g_list *ptr1, *ptr2, *result, *tail;
    if (lptr2 == NULL)
    {
        result = lptr1;
    }
    else if (lptr1 == NULL)
    {
        result = lptr2;
    }
    else
    {
        ptr1 = lptr1;
        ptr2 = lptr2;
        if (lptr1->sum < lptr2->sum)
        {
            result = lptr1;
            ptr1 = ptr1->next;
        }
        else
        {
            result = lptr2;
            ptr2 = ptr2->next;
        }
        tail = result;
        while ((ptr1 != NULL) && (ptr2 != NULL))
        {
            if (ptr1->sum < ptr2->sum)
            {
                tail = tail->next = ptr1;
                ptr1 = ptr1->next;
            }
            else
            {
                tail->next = ptr2;
                tail = tail->next;
                ptr2 = ptr2->next;
            }
        }
        if (ptr1 != NULL)
        {
            tail->next = ptr1;
        }
        else
        {
            tail->next = ptr2;
        }
    }
    return result;
}
g_list *Gdivide(g_list *lptr)
{
    g_list *fast, *slow, *ptr;
    slow = lptr;
    fast = lptr->next->next;
    while (fast != NULL)
    {
        fast = fast->next;
        slow = slow->next;
        if (fast != NULL)
        {
            fast = fast->next;
        }
    }
    ptr = slow->next;
    slow->next = NULL;
    return ptr;
}
g_list *Gsort(g_list *lptr)
{
    g_list *ptr;
    g_list *head = lptr;
    if ((lptr != NULL) && (lptr->next != NULL))
    {
        ptr = Gdivide(lptr);
        lptr = Gsort(lptr);
        ptr = Gsort(ptr);
        head = Gmerge(lptr, ptr);
    }
    return head;
}
// leaderboard of groups
void Generate_leaderBoard(g_list *ghead)
{
    g_list *gnode = ghead;

    while (gnode != NULL)
    {
        int total_steps = 0;
        for (int i = 0; i < gnode->Gsize; i++)
        {
            for (int j = 0; j < 7; j++)
            {
                total_steps += gnode->members[i]->a_count[j];
            }
        }
        gnode->sum = total_steps;
        gnode = gnode->next;
    }
    g_list *temp = Gsort(ghead);
    ghead = temp;
    int i = 1;
    while (temp != NULL)
    {
        printf("%d   %s    %d\n", i, temp->G_name, temp->sum);
        i++;
        temp = temp->next;
    }
}

// rewars
void check_individual_reward(i_list *ihead, int id)
{
    i_list *top[3] = {NULL, NULL, NULL};
    int total[3] = {0, 0, 0};
    i_list *current = ihead;
    while (current->next != NULL)
    {
        int sum = 0;
        int flag = 1;
        for (int i = 0; i < 7; i++)
        {
            sum = sum + current->a_count[i];
        }
        if (sum > current->Dailystep_goal)
        {
            for (int i = 0; i < 3; i++)
            {
                if (sum > total[i] && flag == 1)
                {
                    for (int j = 2; j > i; j--)
                    {
                        top[j] = top[j - 1];
                        total[j] = total[j - 1];
                    }
                    top[i] = current;
                    total[i] = sum;
                    flag = 0;
                }
            }
        }
        current = current->next;
    }

    if (id == top[0]->ID)
    {
        printf("reward = 100");
    }
    else if (id == top[1]->ID)
    {
        printf("reward = 75");
    }
    else if (id == top[2]->ID)
    {
        printf("reward = 50");
    }
    else
    {
        printf("no reward");
    }
}

void delete_person(i_list *ihead, int id)
{
    i_list *temp = ihead, *prev = NULL;
    if (temp != NULL && temp->ID == id)
    {
        ihead = ihead->next;
        free(temp);
        return;
    }
    while (temp != NULL && temp->ID != id)
    {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL)
        return;
    prev->next = temp->next;
    free(temp);
}

void delete_group(g_list *ghead, int gid)
{
    g_list *gnode = ghead;
    g_list *prev = NULL;
    while (gnode != NULL && gnode->G_id != gid)
    {
        prev = gnode;
        gnode = gnode->next;
    }
    if (gnode == NULL)
    { // If the group is not found
        printf("group with ID %d does not exist.\n", gid);
        return;
    }
    if (prev == NULL)
    {
        ghead = gnode->next;
    }
    else
    {
        prev->next = gnode->next;
    }
    ghead->Gsize--; // Decrement the size

    free(gnode);
    gnode = NULL;
    printf("group with ID %d deleted.\n", gid);
}

void display_group_info(g_list *ghead, int id1)
{
    g_list *gnode = ghead;
    while (gnode != NULL && gnode->G_id != id1)
    {
        gnode = gnode->next;
    }
    if (gnode == NULL)
    {
        printf("Group with ID %d does not exist.\n", id1);
        return;
    }
    printf("Group name: %s\n", gnode->G_name);
    printf("Group ID: %d\n", gnode->G_id);
    printf("Group goal: %d\n", gnode->G_goal);
    printf("Group size: %d\n", gnode->Gsize);
    printf("Group members:\n");
    for (int i = 0; i < gnode->Gsize; i++)
    {
        printf("%d. %s (ID: %d)\n", i + 1, gnode->members[i]->Name, gnode->members[i]->ID);
    }
    int rank = 0;
    int sum = 0;
    for (int i = 0; i < gnode->Gsize; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            sum += gnode->members[i]->a_count[j]; // Add the steps to the sum
        }
    }
    g_list *curr = ghead;
    while (curr != NULL)
    {
        int total = 0;
        for (int i = 0; i < curr->Gsize; i++)
        {
            for (int j = 0; j < 7; j++)
            {
                total += curr->members[i]->a_count[j];
            }
        }
        if (total > sum)
        {
            rank++;
        }
        curr = curr->next;
    }
    rank++;
    printf("Group rank: %d\n\n", rank);
}

void suggest_goal_update(i_list *ihead, int pid)
{
    i_list *pnode = ihead;
    while (pnode != NULL && pnode->ID != pid)
    {
        pnode = pnode->next;
    }
    if (pnode == NULL)
    {
        printf("Person with ID %d does not exist.\n", pid);
    }
    else
    {
        int sum = 0;
        int flag = 0;
        for (int i = 0; i < 7; i++)
        {
            sum += pnode->a_count[i];
            if (pnode->a_count[i] > pnode->Dailystep_goal && flag == 0)
            {
                flag = 1;
            }
        }

        if (flag == 0)
        {
            printf("Person %s has not completed the daily goals.\n", pnode->Name);
            printf("SUGGESTION: try to complete ur daily goal after i will compare u with top members");
        }
        else
        {

            i_list *top[3] = {NULL, NULL, NULL};
            int total[3] = {0, 0, 0};
            i_list *current = ihead;
            while (current->next != NULL)
            {
                int sum = 0;
                int flag = 1;
                for (int i = 0; i < 7; i++)
                {
                    sum = sum + current->a_count[i];
                }
                if (sum > current->Dailystep_goal)
                {
                    for (int i = 0; i < 3; i++)
                    {
                        if (sum > total[i] && flag == 1)
                        {
                            for (int j = 2; j > i; j--)
                            {
                                top[j] = top[j - 1];
                                total[j] = total[j - 1];
                            }
                            top[i] = current;
                            total[i] = sum;
                            flag = 0;
                        }
                    }
                }
                current = current->next;
            }
            int avg = (total[0] + total[1] + total[2]) / 3;
            printf("try to increase ur goal to atleast %d", avg);
        }
    }
}

void merge_groups(g_list **ghead, i_list *phead, int gid1, int gid2)
{
    g_list *gnode1 = *ghead;
    g_list *prev1 = NULL;

    while (gnode1 != NULL && gnode1->G_id != gid1)
    {
        prev1 = gnode1;
        gnode1 = gnode1->next;
    }

    if (gnode1 == NULL)
    {
        printf("group with ID %d does not exist.\n", gid1);
        return;
    }

    g_list *gnode2 = *ghead;
    g_list *prev2 = NULL;

    while (gnode2 != NULL && gnode2->G_id != gid2)
    {
        prev2 = gnode2;
        gnode2 = gnode2->next;
    }

    if (gnode2 == NULL)
    {
        printf("group with ID %d does not exist.\n", gid2);
        return;
    }

    if (gnode1->Gsize + gnode2->Gsize > 5)
    {
        printf("The merged group will be too large.\n");
        return;
    }

    int new_id;
    char new_name[20];
    int new_goal;
    printf("enter the new id for the new group:");
    scanf("%d", &new_id);
    printf("enter the new name for the new group:");
    scanf("%s", new_name);
    printf("enter the new goal for the new group:");
    scanf("%d", &new_goal);
    int new_mem[5] = {0, 0, 0, 0, 0};
    int k = 0;
    for (int i = 0; i < gnode1->Gsize; i++)
    {
        new_mem[k] = gnode1->members[i]->ID; // Copy member structures, not just pointers
        k++;
    }

    // Copy members from the second group
    for (int i = 0; i < gnode2->Gsize; i++)
    {
        new_mem[k] = gnode2->members[i]->ID;
        k++;
    }
    g_list *new_group = create_group(new_id, new_name, new_mem, new_goal, 0);
    new_group->Gsize = k;

    if (prev1 == NULL)
    {
        *ghead = gnode1->next;
    }
    else
    {
        prev1->next = gnode1->next;
    }

    if (prev2 == NULL)
    {
        *ghead = gnode2->next;
    }
    else
    {
        prev2->next = gnode2->next;
    }

    insert_group(ghead, new_group);
    printf("New group %s created by merging group %s and group %s.\n", new_group->G_name, gnode1->G_name, gnode2->G_name);
    printf("The groups with ids %d %d \nare deleted", gnode1->G_id, gnode2->G_id);
    free(gnode1);
    free(gnode2);
}

void read_data_from_file()
{
    FILE *persons_file = fopen("persons.txt", "r");
    FILE *groups_file = fopen("groups.txt", "r");

    while (!feof(persons_file))
    {
        int id, age, goal, steps[7];
        char name[20];
        fscanf(persons_file, "%d %s %d %d", &id, name, &age, &goal);
        for (int j = 0; j < 7; j++)
        {
            fscanf(persons_file, "%d", &steps[j]);
        }
        i_list *pnode = create_individual(id, name, age, goal, steps);
        Add_Person(&ihead, pnode);
    }
    g_list *gnode;
    while (!feof(groups_file))
    {
        int id, siz, goal, members[5];
        char name[20];
        fscanf(groups_file, "%d %d", &id, &siz);
        for (int j = 0; j < siz; j++)
        {
            fscanf(groups_file, "%d", &members[j]);
        }
        fscanf(groups_file, "%s %d", name, &goal);

        gnode = create_group(id, name, members, goal, siz);
        insert_group(&ghead, gnode);
    }

    fclose(persons_file);
    fclose(groups_file);
}

void update_data(i_list *phead, g_list *ghead)
{
    FILE *file1 = fopen("harsha.txt", "w");
    FILE *file2 = fopen("b.txt", "w");

    if (file1 == NULL || file2 == NULL)
    {
        printf("Error opening file(s).\n");
        return;
    }

    i_list *ptr = phead;
    g_list *nptr = ghead;

    while (ptr != NULL)
    {
        int i = 0;
        fprintf(file1, "%d %s %d %d ", ptr->ID, ptr->Name, ptr->Age, ptr->Dailystep_goal);
        while (i < 7)
        {
            fprintf(file1, "%d ", ptr->a_count[i]);
            i++;
        }
        fprintf(file1, "\n");
        ptr = ptr->next;
    }

    fclose(file1);

    while (nptr != NULL)
    {
        int j = 0;
        fprintf(file2, "%d %d ", nptr->G_id, nptr->Gsize);
        while (j < nptr->Gsize)
        {
            fprintf(file2, "%d ", nptr->members[j]->ID);
            j++;
        }
        fprintf(file2, "%s %d ", nptr->G_name, nptr->G_goal);
        fprintf(file2, "\n");
        nptr = nptr->next;
    }

    fclose(file2);
}

int main()
{
    ihead = malloc(sizeof(i_list));
    if (ihead == NULL)
    {
        printf("Memory allocation error\n");
        return 0;
    }
    read_data_from_file();

    int choice, i, n, m;
    i_list *inode;
    int person_id, person_age, daily_goal, a_count1[7];
    char person_name[20];
    g_list *gnode;
    int group_id;
    int group_goal;
    int group_size;
    int mem1[5];
    char group_name[20];

    int exit = 0;

    while (exit != 1)
    {
        printf("-----------------------------------------------------------\n");
        printf("Step Counting application initialising ... \n");

        printf(" 1) Add a person \n 2) Create a group \n 3) get_top \n 4) check_group_achievement \n 5) Generate_leader_board \n 6) Check_individual_rewards \n 7) Delete_individual \n 8) Delete_group \n 9) Merge_groups \n 10) Display_group_info \n 11) Suggest_goal_update \n ");

        printf("enter the number of command : ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            printf("enter person id :");
            scanf("%d", &person_id);
            printf("enter person name :");
            scanf("%s", person_name);
            printf("enter person age");
            scanf("%d", &person_age);
            printf("enter daily step goal");
            scanf("%d", &daily_goal);
            i = 0;
            while (i < 7)
            {
                printf("enter  daily stepsof day %d:", i + 1);
                scanf("%d", &a_count1[i]);
                i++;
            }
            inode = create_individual(person_id, person_name, person_age, daily_goal, a_count1);
            Add_Person(&ihead, inode);
            update_data(ihead, ghead);
            break;
        }
        case 2:
        {
            printf("enter group id :");
            scanf("%d", &group_id);
            printf("enter group name :");
            scanf("%s", group_name);

            printf("enter group goal");
            scanf("%d", &group_goal);
            printf("enter group size");
            scanf("%d", &group_size);
            i = 0;
            while (i < group_size)
            {
                printf("enter id of members of group :");
                scanf("%d", &mem1[i]);
                i++;
            }
            gnode = create_group(group_id, group_name, mem1, group_goal, group_size);
            insert_group(&ghead, gnode);
            g_list *curr = ghead;
            while (curr != NULL)
            {
                printf("%d %s %d\n", curr->G_id, curr->G_name, curr->G_goal);
                curr = curr->next;
            }
            update_data(ihead, ghead);
            break;
        }
        case 3:
        {
            get_top_3(ihead);
            break;
        }
        case 4:
        {
            printf("enter the group id");
            scanf("%d", &n);
            check_group_achievement(n);
            break;
        }
        case 5:
        {
            Generate_leaderBoard(ghead);
            break;
        }
        case 6:
        {
            printf("enter id");
            scanf("%d", &n);
            check_individual_reward(ihead, n);
            break;
        }
        case 7:
        {
            printf("enter id");
            scanf("%d", &n);
            delete_person(ihead, n);
            update_data(ihead, ghead);
            break;
        }
        case 8:
        {
            printf("enter id");
            scanf("%d", &n);
            delete_group(ghead, n);
            update_data(ihead, ghead);
            break;
        }
        case 9:
        {
            printf("enterfirst group id");
            scanf("%d", &n);
            printf("enter second group id");
            scanf("%d", &m);
            merge_groups(&ghead, ihead, n, m);
            g_list *curr = ghead;
            while (curr != NULL)
            {
                printf("%d %s %d\n", curr->G_id, curr->G_name, curr->G_goal);
                curr = curr->next;
            }

            break;
        }
        case 10:
        {
            printf("enter  group id");
            scanf("%d", &n);
            display_group_info(ghead, n);
            break;
        }
        case 11:
        {
            printf("enter id");
            scanf("%d", &n);
            suggest_goal_update(ihead, n);
            break;
        }
        case 12:
        {
            exit = 1;
            break;
        }
        }
        printf("-----------------------------------------------------------\n");
    }

    free(ihead); // Don't forget to free allocated memory before exiting
    free(ghead);

    return 0;
}
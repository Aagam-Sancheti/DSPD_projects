#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Defining tree for person
typedef struct person
{
    int ID;
    char name[50];
    int age;
    int goal;
    int steps[7]; // array of weekly step count
    struct person *left;
    struct person *right;
    int height;
} person;

// linked list to store pointers of individuals
typedef struct GroupMember
{
    person *individual;
    struct GroupMember *next;
} GroupMember;

// Defining tree for group
typedef struct Group
{
    int groupID;
    char groupName[50];
    GroupMember *members; // header pointer to the linked list of members
    int group_goal;
    struct Group *left;
    struct Group *right;
    int height;
    int sum; // for storing totalsteps
} Group;

person *p_root = NULL;
Group *g_root = NULL;

// create node funtion for an new individual object (basically a constructor for person class)
person *create_person(int ID, char *name, int age, int goal, int *steps) // int * steps is a pointer to the array of daily steps
{
    person *individual = (person *)malloc(sizeof(person)); // typecasting the allocated memory for person type
    individual->ID = ID;
    strcpy(individual->name, name);
    individual->age = age;
    individual->goal = goal;
    for (int i = 0; i < 7; i++)
    {
        individual->steps[i] = steps[i];
    }
    individual->left = NULL;
    individual->right = NULL;
    individual->height = 1;
    return individual; // returning the pointer of the newly created person node
}

// Function to create a new group root object
Group *create_group(int groupID, char *groupName, int weeklyGoal)
{
    Group *group = (Group *)malloc(sizeof(Group));
    group->groupID = groupID;
    strcpy(group->groupName, groupName);
    group->group_goal = weeklyGoal;
    group->members = NULL;
    group->left = NULL;
    group->right = NULL;
    group->height = 1;
    group->sum = 0;
    return group; // returning the pointer of the newly created group node
}

// Function to get height of a node
int getheight(person *node)
{
    int ret_val;
    if (node == NULL)
        ret_val = 0;
    else
    {
        ret_val = node->height;
    }
    return ret_val;
}

// Function to get height of a node
int getheightgroup(Group *node)
{
    int ret_val;
    if (node == NULL)
        ret_val = 0;
    else
        ret_val = node->height;
    return ret_val;
}

// Get Balance factor of particular node N
int getBalance(person *N)
{
    int ret_val;
    if (N == NULL)
        ret_val = 0;
    ret_val = getheight(N->left) - getheight(N->right);
    return ret_val;
}

// Get Balance factor of particular node N
int getBalancegroup(Group *N)
{
    int ret_val;
    if (N == NULL)
        ret_val = 0;
    else
        ret_val = getheightgroup(N->left) - getheightgroup(N->right);
    return ret_val;
}

// Function to get maximum of two integers
int max(int a, int b)
{
    int result;
    if (a > b)
        result = a;
    else
        result = b;
    return result;
}

// Function to right rotate group subtree rooted with y
Group *rightRotategroup(Group *y)
{
    Group *x = y->left;
    Group *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(getheightgroup(y->left), getheightgroup(y->right)) + 1;
    x->height = max(getheightgroup(x->left), getheightgroup(x->right)) + 1;

    // Returning the new root
    return x;
}

// Function to left rotate group subtree rooted with x
Group *leftRotategroup(Group *x)
{
    Group *y = x->right;
    Group *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(getheightgroup(x->left), getheightgroup(x->right)) + 1;
    y->height = max(getheightgroup(y->left), getheightgroup(y->right)) + 1;

    // Returning the new root
    return y;
}

// Function to right rotate person subtree rooted with y
person *rightRotate(person *y)
{
    person *x = y->left;
    person *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(getheight(y->left), getheight(y->right)) + 1;
    x->height = max(getheight(x->left), getheight(x->right)) + 1;

    // Returning the new root
    return x;
}

// Function to left rotate person subtree rooted with x
person *leftRotate(person *x)
{
    person *y = x->right;
    person *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(getheight(x->left), getheight(x->right)) + 1;
    y->height = max(getheight(y->left), getheight(y->right)) + 1;

    // Return the new root
    return y;
}

// Function to insert a new person node in the AVL tree
person *add_person(person *p_root, person *pnode)
{
    // Perform the normal BST insertion
    if (p_root == NULL)
    {
        return pnode; // Return the new node if the tree is empty
    }
    if (pnode->ID < p_root->ID)
        p_root->left = add_person(p_root->left, pnode);
    else if (pnode->ID > p_root->ID)
        p_root->right = add_person(p_root->right, pnode);
    // Equal IDs are not allowed, no need to return anything

    // Update height of this ancestor node
    p_root->height = 1 + max(getheight(p_root->left), getheight(p_root->right));

    // Get the balance factor of this ancestor node to check whether this node became unbalanced
    int balance = getBalance(p_root);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && pnode->ID < p_root->left->ID)
        return rightRotate(p_root);

    // Right Right Case
    if (balance < -1 && pnode->ID > p_root->right->ID)
        return leftRotate(p_root);

    // Left Right Case
    if (balance > 1 && pnode->ID > p_root->left->ID)
    {
        p_root->left = leftRotate(p_root->left);
        return rightRotate(p_root);
    }

    // Right Left Case
    if (balance < -1 && pnode->ID < p_root->right->ID)
    {
        p_root->right = rightRotate(p_root->right);
        return leftRotate(p_root);
    }

    else
    {
        return p_root; // Returning the unchanged root pointer
    }
}

// Function to insert a new group root
Group *insert_group(Group **g_root, Group *gnode)
{
    if (*g_root == NULL)
    {
        return gnode; // Exit after insertion and return the updated root
    }

    if (gnode->groupID < (*g_root)->groupID)
        (*g_root)->left = insert_group(&((*g_root)->left), gnode);
    else if (gnode->groupID > (*g_root)->groupID)
        (*g_root)->right = insert_group(&((*g_root)->right), gnode);
    // Equal IDs are not allowed, no need to return anything

    // Update height of this ancestor root
    (*g_root)->height = 1 + max(getheightgroup((*g_root)->left), getheightgroup((*g_root)->right));

    // Get the balance factor of this ancestor root to check whether this root became unbalanced
    int balance = getBalancegroup(*g_root);

    // If this root becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && gnode->groupID < (*g_root)->left->groupID)
        *g_root = rightRotategroup(*g_root);

    // Right Right Case
    if (balance < -1 && gnode->groupID > (*g_root)->right->groupID)
        *g_root = leftRotategroup(*g_root);

    // Left Right Case
    if (balance > 1 && gnode->groupID > (*g_root)->left->groupID)
    {
        (*g_root)->left = leftRotategroup((*g_root)->left);
        *g_root = rightRotategroup(*g_root);
    }

    // Right Left Case
    if (balance < -1 && gnode->groupID < (*g_root)->right->groupID)
    {
        (*g_root)->right = rightRotategroup((*g_root)->right);
        *g_root = leftRotategroup(*g_root);
    }

    return *g_root; // Return the updated root
}

// Function to search for an individual in the AVL tree
person *search_person(person *p_root, int ID)
{
    person *ret_val;
    // Base cases: root is NULL or key is present at root
    if (p_root == NULL || p_root->ID == ID)
        ret_val = p_root;

    // Key is greater than root's ID
    if (p_root->ID < ID)
        search_person(p_root->right, ID);

    else
    {
        // Key is smaller than root's ID
        search_person(p_root->left, ID);
    }
    return ret_val;
}

// Function to search for a group in the AVL tree
Group *search_group(Group *g_root, int groupID)
{
    Group *ret_val;
    // Base cases: root is NULL or key is present at root
    if (g_root == NULL || g_root->groupID == groupID)
        ret_val = g_root;

    // Key is greater than root's groupID
    if (g_root->groupID < groupID)
        search_group(g_root->right, groupID);

    else
    {
        // Key is smaller than root's groupID
        search_group(g_root->left, groupID);
    }
    return ret_val;
}

// Function to calculate total steps of an person
int getTotalSteps(person *individual)
{
    int totalSteps = 0;
    for (int i = 0; i < 7; i++)
    {
        totalSteps = totalSteps + individual->steps[i];
    }
    return totalSteps;
}
// array to store top 3 persons
person *top[3] = {NULL};
int topSteps[3] = {0};

// Recursive function to find top 3 persons
void findTop3(person *p_root, person *top[], int topSteps[], int *count)
{ // count maintains the index of top 3 people
    if (p_root != NULL)
    {

        if (p_root->goal <= getTotalSteps(p_root))
        {
            // Check if there's space in top 3 or if the current person has more steps
            if (*count < 3 || (top[2] && getTotalSteps(p_root) > topSteps[2]))
            {
                // If top 3 is not full, add the current person
                if (*count < 3)
                {
                    top[*count] = p_root;
                    topSteps[*count] = getTotalSteps(p_root);
                    (*count)++;
                }
                else
                { // If top 3 is full, replace the person with the least steps
                    top[2] = p_root;
                    topSteps[2] = getTotalSteps(p_root);
                }

                // Bubble up the newly added person to maintain sorted order
                int index = *count - 1;
                while (index > 0 && topSteps[index] > topSteps[index - 1])
                {
                    // Swap the person and its steps with the previous one
                    person *tempperson = top[index];
                    top[index] = top[index - 1];
                    top[index - 1] = tempperson;

                    int tempSteps = topSteps[index];
                    topSteps[index] = topSteps[index - 1];
                    topSteps[index - 1] = tempSteps;

                    index--;
                }
            }
        }
        findTop3(p_root->right, top, topSteps, count);
        findTop3(p_root->left, top, topSteps, count);
    }
}

// Function to display the top 3 persons who have completed their daily step goals
void Get_top_3(person *p_root)
{
    int count = 0;

    findTop3(p_root, top, topSteps, &count);

    printf("Top 3 persons:\n");
    for (int i = 0; i < count; ++i)
    {
        printf("ID: %d Name: %s Total Steps: %d\n", top[i]->ID, top[i]->name, topSteps[i]);
    }
}

// Function to perform inorder traversal of AVL tree and update total steps of Groups
void calculateGroupTotalSteps(Group *g_root)
{
    if (g_root != NULL)
    {
        calculateGroupTotalSteps(g_root->left);
        calculateGroupTotalSteps(g_root->right);

        int totalSteps = 0;
        GroupMember *currentMember = g_root->members;
        while (currentMember != NULL)
        {
            totalSteps += getTotalSteps(currentMember->individual);
            currentMember = currentMember->next;
        }
        g_root->sum = totalSteps;
    }
}

// Function to display whether a given group has completed its weekly group goal
void Check_group_achievement(Group *g_root, int groupID)
{
    // Function to calculate total weekly steps of a group

    Group *group = search_group(g_root, groupID);
    if (group != NULL)
    {

        if (group->sum >= group->group_goal)
            printf("Group '%s' has achieved its weekly group goal of %d with %d steps.\n", group->groupName, group->group_goal, group->sum);
        else
            printf("Group '%s' has not achieved its weekly group goal.\n", group->groupName);
    }
    else
    {
        printf("Group with ID %d not found. \n", groupID);
    }
}

// Function to count the number of nodes in the AVL tree
int countNodes(Group *root)
{
    int outcome;
    if (root == NULL)
    {
        outcome = 0;
    }
    else
    {
        outcome = 1 + countNodes(root->left) + countNodes(root->right);
    }
    return outcome;
}

// Function to merge two sorted arrays of Group pointers  ( part of merge sort function)
void merge(Group **arr, int l, int m, int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temporary arrays
    Group **L = (Group **)malloc(n1 * sizeof(Group *));
    Group **R = (Group **)malloc(n2 * sizeof(Group *));
    if (L == NULL || R == NULL)
    {
        printf("Memory allocation failed\n");
        exit(1);
    } // safety check for correct memory allocation

    // Copy data to temporary arrays L[] and R[]
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Merge the temporary arrays back into arr[l..r]
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2)
    {
        if (L[i]->sum >= R[j]->sum)
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    // Copy the remaining elements of L[], if any
    while (i < n1)
        arr[k++] = L[i++];

    // Copy the remaining elements of R[], if any
    while (j < n2)
        arr[k++] = R[j++];

    free(L);
    free(R);
}

// Function to implement merge sort on an array of Group pointers
void mergeSort(Group **arr, int l, int r)
{
    if (l < r)
    {
        int m = l + (r - l) / 2; // Calculate the middle index

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        // Merge the sorted halves
        merge(arr, l, m, r);
    }
}

// take the nodes from the tree and store it into the array
void populateArray(Group *root, Group **arr, int *index)
{
    if (root == NULL)
        return;

    populateArray(root->left, arr, index);
    arr[(*index)++] = root;
    populateArray(root->right, arr, index);
}

// Function to generate and display the leaderboard
void Generate_leader_board(Group *g_root)
{
    if (g_root == NULL)
    {
        printf("No groups found.\n");
        return;
    }

    // Count the number of nodes in the AVL tree
    int count = countNodes(g_root);

    // Allocate memory for the array based on the number of nodes in the AVL tree
    Group **arr = (Group **)malloc(sizeof(Group *) * count);
    if (arr == NULL)
    {
        printf("Memory allocation failed\n");
    }
    else
    {
        int index = 0;
        populateArray(g_root, arr, &index);

        // Sort the array based on total steps using merge sort
        mergeSort(arr, 0, count - 1);

        // Display leaderboard
        printf("Leader Board:\n");
        for (int i = 0; i < count; i++)
        {
            printf("%d. %s with total steps: %d\n", i + 1, arr[i]->groupName, arr[i]->sum);
        }

        // Free dynamically allocated memory
        free(arr);
    }
}

int calculateGroupRank(Group **g_root, int groupID)
{
    int rank = -1; // Initialize rank as -1 (individualcating group not found)

    if (*g_root == NULL)
    {
        printf("No groups found.\n");
        return rank;
    }

    // Count the number of nodes in the AVL tree
    int count = countNodes(*g_root);

    // Allocate memory for the array based on the number of nodes in the AVL tree
    Group **arr = (Group **)malloc(sizeof(Group *) * count);
    if (arr == NULL)
    {
        printf("Memory allocation failed\n");
    }

    // Sort the array based on total steps using merge sort
    mergeSort(arr, 0, count - 1);
    int checker = 0;
    // Search for the group with the given groupID in the array
    for (int i = 0; i < count && checker == 0; i++)
    {
        if (arr[i]->groupID == groupID)
        {
            rank = i + 1; // Adjust rank to start from 1
            checker = 1;  // Found the group, no need to continue searching
        }
    }

    // Free dynamically allocated memory
    free(arr);

    return rank;
}

// Function to display rewards earned by an person
void check_individual_rewards(person *p_root, int ID)
{
    int count = 0;
    findTop3(p_root, top, topSteps, &count);
    // Check if root is not NULL before accessing its attributes
    person *curr = search_person(p_root, ID);

    if (curr != NULL)
    {

        // Calculate total steps of the person
        int totalSteps = getTotalSteps(curr);

        // Check if the person has met their goal
        if (curr->goal <= totalSteps)
        {
            int rank; // Display rewards based on rank
            if (curr->ID == top[0]->ID)
                printf("person with ID %d earned 100 points.\n", ID);
            else if (curr->ID == top[1]->ID)
                printf("person with ID %d earned 75 points.\n", ID);
            else if (curr->ID == top[2]->ID)
                printf("person with ID %d earned 50 points.\n", ID);
            else
                printf("person with ID %d reaced their goal but did not earn any points.\n", ID);
        }
        else
        {
            printf("person with ID %d did not meet their goal.\n", ID);
        }
    }
    else
    {
        printf("Error: Root is NULL.\n");
    }
}

void removeFromGroup(Group **g_root, int ID) // this function is a part of delete_individual
{
    Group *currGroup = *g_root;
    if (currGroup != NULL)
    {
        removeFromGroup(&(currGroup->left), ID);  // Remove from left subtree
        removeFromGroup(&(currGroup->right), ID); // Remove from right subtree

        GroupMember *prev = NULL;
        GroupMember *currperson = currGroup->members;

        while (currperson != NULL)
        {
            if (currperson->individual->ID == ID)
            {
                // Remove the person from the group
                if (prev == NULL)
                {
                    currGroup->members = currperson->next; // Update head if removing the first member
                }
                else
                {
                    prev->next = currperson->next;
                }
                // Free the memory for the person and set its pointer to NULL
                free(currperson->individual);
                currperson->individual = NULL;
                free(currperson); // Free the memory for the GroupMember structure
                return;           // Exit function after removing the person
            }
            prev = currperson;
            currperson = currperson->next;
        }
    }
    printf("person with ID %d not found in any group.\n", ID);
}

person *delete_individual(person **p_root, Group **g_root, int ID)
{
    person *temp_p_root_ptr;
    if (*p_root == NULL)        // p_root is the root node pointer of the persons tree
        temp_p_root_ptr = NULL; // person not found, no changes needed

    if (ID < (*p_root)->ID)
        (*p_root)->left = delete_individual(&((*p_root)->left), g_root, ID);
    else if (ID > (*p_root)->ID)
        (*p_root)->right = delete_individual(&((*p_root)->right), g_root, ID);
    else
    {
        // Node with one or no child
        if ((*p_root)->left == NULL)
        {
            temp_p_root_ptr = (*p_root)->right;
            free(*p_root);
        }
        else if ((*p_root)->right == NULL)
        {
            temp_p_root_ptr = (*p_root)->left;
            free(*p_root);
        }
        else
        {
            // Node with two children
            temp_p_root_ptr = (*p_root)->right;
            while (temp_p_root_ptr && temp_p_root_ptr->left != NULL)
                temp_p_root_ptr = temp_p_root_ptr->left;

            (*p_root)->ID = temp_p_root_ptr->ID;
            strcpy((*p_root)->name, temp_p_root_ptr->name);
            (*p_root)->age = temp_p_root_ptr->age;
            (*p_root)->goal = temp_p_root_ptr->goal;
            memcpy((*p_root)->steps, temp_p_root_ptr->steps, sizeof(int) * 7);

            (*p_root)->right = delete_individual(&((*p_root)->right), g_root, temp_p_root_ptr->ID);

            // Now, update the group to remove the person
            removeFromGroup(g_root, ID);
        }
    }
    temp_p_root_ptr = *p_root;
    return temp_p_root_ptr;
}

// Function to delete a group but retain its persons, similar function as delete individual
Group *delete_group(Group **g_root, int groupID)
{
    Group *temp_g_root;
    // Base case: If the tree is empty
    if (*g_root == NULL)
        temp_g_root = *g_root;

    // If the key to be deleted is smaller than the g_root's key, then it lies in the left subtree
    if (groupID < (*g_root)->groupID)
        (*g_root)->left = delete_group(&((*g_root)->left), groupID);

    // If the key to be deleted is greater than the g_root's key, then it lies in the right subtree
    else if (groupID > (*g_root)->groupID)
        (*g_root)->right = delete_group(&((*g_root)->right), groupID);

    // If key is same as g_root's key, then This is the node to be deleted
    else
    {
        // Node with one or no child
        if ((*g_root)->left == NULL)
        {
            temp_g_root = (*g_root)->right;
            *g_root = NULL;
            free(*g_root);
        }
        else if ((*g_root)->right == NULL)
        {
            temp_g_root = (*g_root)->left;
            *g_root = NULL;
            free(*g_root);
        }

        // Node with two children
        temp_g_root = (*g_root)->right;
        while (temp_g_root && temp_g_root->left != NULL)
            temp_g_root = temp_g_root->left;

        (*g_root)->groupID = temp_g_root->groupID;
        strcpy((*g_root)->groupName, temp_g_root->groupName);
        (*g_root)->group_goal = temp_g_root->group_goal;
        (*g_root)->right = delete_group(&((*g_root)->right), temp_g_root->groupID);
    }
    printf("Group with %d is deleted\n", groupID);
    temp_g_root = *g_root;
    return temp_g_root;
}

// Function to add an person to a group
void addMemberToGroup(Group *group, person *person)
{
    Group *currGroup = group;
    GroupMember *currMember = currGroup->members;
    struct person *current = person;

    if (currGroup == NULL)
    {
        printf("Group does not exist.\n");
    }
    else
    {
        // Check if the group is full
        int count = 0;
        GroupMember *tempMember = currMember;
        while (tempMember != NULL)
        {
            count++;
            tempMember = tempMember->next;
        }

        if (count >= 5)
        {
            printf("Group %s is full.\n", currGroup->groupName);
        }
        else
        {
            // Check if the person is already in the group
            while (currMember != NULL)
            {
                if (currMember->individual->ID == current->ID)
                {
                    printf("Person with ID %d already belongs to group %s.\n", current->ID, currGroup->groupName);
                    return; // Exit function if person is already in the group
                }
                currMember = currMember->next;
            }

            // If person is not already in the group, add them
            GroupMember *newMember = (GroupMember *)malloc(sizeof(GroupMember));
            if (newMember == NULL)
            {
                printf("Memory allocation failed.\n");
                return;
            }
            newMember->individual = current;
            newMember->next = currGroup->members;
            currGroup->members = newMember;
        }
    }
}

// Function to merge two groups and create a new group
Group *merge_groups(Group **g_root, int groupID1, int groupID2)
{
    Group *new_group;
    // Get input for the new group's ID, name, and goals
    int newid, newGoals;
    char newname[20];
    printf("Enter the new id for the group:");
    scanf("%d", &newid);
    printf("Enter the new name for the group:");
    scanf("%s", newname);
    printf("Enter the new goal for the group:");
    scanf("%d", &newGoals);

    // Search for the groups to merge
    Group *group1 = search_group(*g_root, groupID1);
    Group *group2 = search_group(*g_root, groupID2);

    // If either of the groups is not found, return
    if (group1 == NULL || group2 == NULL)
    {
        printf("One or both of the groups to merge were not found.\n");
        new_group = *g_root;
    }

    // Create a new group to merge into
    Group *group = create_group(newid, newname, newGoals); // Assuming create_group function exists

    // Merge the members of group2 into mergedGroup
    GroupMember *currentMember = group2->members;
    while (currentMember != NULL)
    {
        addMemberToGroup(group, currentMember->individual);
        currentMember = currentMember->next;
    }

    // Merge the members of group1 into mergedGroup
    currentMember = group1->members;
    while (currentMember != NULL)
    {
        addMemberToGroup(group, currentMember->individual);
        currentMember = currentMember->next;
    }
    // Add the merged group to the main group list
    *g_root = insert_group(g_root, group); // Assuming add_group function exists
    // Delete group1 and group2
    *g_root = delete_group(g_root, groupID1); // Assuming delete_group function exists
    *g_root = delete_group(g_root, groupID2);

    new_group = *g_root;
    return new_group;
}

// Function to display group information including group goals and ranks
void Display_group_range_info(Group *g_root, int groupID)
{
    int rank = calculateGroupRank(&g_root, groupID);
    Group *root = search_group(g_root, groupID);
    if (root == NULL)
    {
        printf("Group with ID %d not found\n", groupID);
        return;
    }
    else
    {
        printf("Group Name: %s\n", root->groupName);
        printf("Group ID: %d\n", root->groupID);
        printf("Weekly Group Goal: %d\n", root->group_goal);
        printf("Group Rank: %d\n", rank);
        printf("Group Members:\n");
        GroupMember *currentMember = root->members;
        while (currentMember != NULL)
        {
            if (currentMember->individual != NULL)
            {
                printf("ID: %d, Name: %s\n", currentMember->individual->ID, currentMember->individual->name);
            }
            currentMember = currentMember->next;
        }
        printf("\n");
    }
}

// Function to calculate average steps per day for an individual
int calculateAverageSteps()
{
    int totalSteps = 0;
    for (int i = 0; i < 3; ++i)
        totalSteps = totalSteps + topSteps[i]; // topsteps is a global array that stores the steps of top 3 individuals
    totalSteps = (totalSteps / 21) + 1;
    return totalSteps;
}

// Function to suggest daily goal update for an individual
void Suggest_goal_update(person *p_root, int ID)
{
    // Search for the individual
    person *individual = search_person(p_root, ID);

    // If individual not found, return
    if (individual == NULL)
    {
        printf("person with ID %d not found.\n", ID);
        return;
    }

    // Calculate average steps per day
    int averageSteps = calculateAverageSteps();
    printf("It is suggested daily step goal %d for individual with ID %d.\n", averageSteps, ID);
}

// file-handling functions
void read_data_from_file()
{
    FILE *file_pointer = fopen("persons.txt", "r");
    if (file_pointer != NULL)
    {
        int n;
        fscanf(file_pointer, "%d", &n); // Read the number of persons

        // Read person data
        for (int i = 0; i <= n; i++)
        {
            int id, age, goal, steps[7];
            char name[50]; // Adjusted size for longer names
            fscanf(file_pointer, "%d %s %d %d", &id, name, &age, &goal);
            for (int j = 0; j < 7; j++)
            {
                fscanf(file_pointer, "%d", &steps[j]);
            }
            person *individual = create_person(id, name, age, goal, steps);
            p_root = add_person(p_root, individual); // Insert person into AVL tree
        }

        int m;
        fscanf(file_pointer, "%d", &m); // Read the number of groups (m)

        // Read group data
        for (int k = 0; k <= m; k++)
        {
            int id, goal;
            char name[50];
            fscanf(file_pointer, "%d %s %d", &id, name, &goal);

            // Create a new group
            Group *new_group = create_group(id, name, goal);

            for (int l = 0; l <= 5; l++)
            {
                int memberID;
                fscanf(file_pointer, "%d", &memberID);
                if (memberID != 0)
                {
                    // Add member to the group
                    person *member = search_person(p_root, memberID);
                    if (member != NULL)
                    {
                        addMemberToGroup(new_group, member);
                    }
                }
                // insert the group into the group tree ( g_root)
                g_root = insert_group(&g_root, new_group);
            }
        }
        fclose(file_pointer);
    }
    else
    {
        printf("Error opening File!\n");
    }
}

// Function to print the preorder traversal of person tree
void preOrder(person *root)
{
    if (root != NULL)
    {
        printf("ID: %d, Name: %s\n", root->ID, root->name);
        preOrder(root->left);
        preOrder(root->right);
    }
}

// Function to print the preorder traversal of group tree
void preOrdergroup(Group *root)
{
    if (root != NULL)
    {
        printf("Group ID: %d, Group Name: %s\n", root->groupID, root->groupName);
        preOrdergroup(root->left);
        preOrdergroup(root->right);
    }
}

int main()
{
    read_data_from_file();
    calculateGroupTotalSteps(g_root);
    preOrder(p_root);
    preOrdergroup(g_root);

    int x, x1, x2, x3, x4, x5, x6;
    int y;

    int choice;
    bool exit = false;
    while (!exit)
    {
        printf("\nMenu of Choices:\n");
        printf("1. Get top 3 persons\n");
        printf("2. Check group achievement\n");
        printf("3. Generate leader board\n");
        printf("4. Check person Rewards\n");
        printf("5. Delete group \n");
        printf("6. Delete person\n");
        printf("7. Merge Groups\n");
        printf("8. Display group info\n");
        printf("9.suggest Goal update\n");
        printf("10.Exit\n");
        printf("------------------------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        printf("------------------------------------------------------------\n");

        switch (choice)
        {
        case 1:
            Get_top_3(p_root);
            printf("------------------------------------------------------------\n\n");
            break;
        case 2:
            printf("enter the id: ");
            scanf("%d", &x);
            Check_group_achievement(g_root, x);
            printf("------------------------------------------------------------\n\n");
            break;
        case 3:
            Generate_leader_board(g_root);
            printf("------------------------------------------------------------\n\n");
            break;
        case 4:
            printf("enter the id: ");
            scanf("%d", &x1);
            check_individual_rewards(p_root, x1);
            printf("------------------------------------------------------------\n\n");
            break;
        case 5:
            printf("enter the id: ");
            scanf("%d", &x2);
            delete_group(&g_root, x2);
            printf("------------------------------------------------------------\n\n");
            break;
        case 6:
            printf("enter the id: ");
            scanf("%d", &x3);
            delete_individual(&p_root, &g_root, x3);
            preOrder(p_root);
            printf("------------------------------------------------------------\n\n");
            break;
        case 7:
            printf("enter the id1 and id2: ");
            scanf("%d %d", &x4, &y);
            merge_groups(&g_root, x4, y);
            printf("------------------------------------------------------------\n\n");
            break;
        case 8:
            printf("enter the id: ");
            scanf("%d", &x5);
            Display_group_range_info(g_root, x5);
            printf("------------------------------------------------------------\n\n");
            break;
        case 9:
            Get_top_3(p_root);
            printf("enter the id: ");
            scanf("%d", &x6);
            Suggest_goal_update(p_root, x6);
            printf("------------------------------------------------------------\n\n");
            break;
        case 10:
            printf("Exiting program. Goodbye!\n");
            printf("------------------------------------------------------------\n\n");
            exit = true;
            // free(p_root); //free allocated memory before exiting
            // free(g_root);
            break;
        default:
            printf("Invalid choice. Please enter a valid option.\n");
            printf("------------------------------------------------------------\n\n");
        }
    }

    return 0;
}

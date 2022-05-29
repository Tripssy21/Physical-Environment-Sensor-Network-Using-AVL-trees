#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define SIZE_STATION 100
#define SIZE_TYPE 10

typedef struct time
{
    int hour, min, sec;
} time;

typedef struct date
{
    int day, month, year;
} date;

typedef struct sensor_node
{
    int sensor_ID, height;
    char sensor_type[SIZE_TYPE];
    char sensor_station[SIZE_STATION];
    time duration;
    struct sensor_node *left;
    struct sensor_node *right;
} sensor_node;

typedef struct record
{
    int sensor_ID, height;
    float data;
    time t;
    date d;
    struct record *left;
    struct record *right;
} record;

int max(int a, int b)
{
    return (a > b) ? a : b;
};

int height_sensor_node(sensor_node *node)
{
    if (node == NULL)
        return 0;
    return node->height;
};

int height_database_node(record *node)
{
    if (node == NULL)
        return 0;
    return node->height;
};

sensor_node *root_sensor_network = NULL;

record *root_record = NULL;

int compare_date(date d1, date d2)
{
    if (d1.year > d2.year)
        return 1;
    else if (d1.year < d2.year)
        return -1;
    else
    {
        if (d1.month > d2.month)
            return 1;
        else if (d1.month < d2.month)
            return -1;
        else
        {
            if (d1.day > d2.day)
                return 1;
            else if (d1.day < d2.day)
                return -1;
            else
                return 0;
        }
    }
}

sensor_node *create_sensor_node(int SID, char SType[], char SStation[], time duration)
{
    sensor_node *new_node;
    new_node = (sensor_node *)malloc(sizeof(sensor_node));
    new_node->sensor_ID = SID;
    strcpy(new_node->sensor_type, SType);
    strcpy(new_node->sensor_station, SStation);
    new_node->duration = duration;
    new_node->height = 1;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

int get_Balance_sensor_database(sensor_node *node)
{
    if (node == NULL)
        return 0;
    return height_sensor_node(node->left) - height_sensor_node(node->right);
}


sensor_node *right_Rotate_sensor_database(sensor_node *y)
{
    sensor_node *x = y->left;
    sensor_node *temp = x->right;

    x->right = y;
    y->left = temp;

    y->height = max(height_sensor_node(y->left), height_sensor_node(y->right)) + 1;
    x->height = max(height_sensor_node(x->left), height_sensor_node(x->right)) + 1;

    return x;
}

sensor_node *left_Rotate_sensor_database(sensor_node *x)
{
    sensor_node *y = x->right;
    sensor_node *temp = y->left;

    y->left = x;
    x->right = temp;

    x->height = max(height_sensor_node(x->left), height_sensor_node(x->right)) + 1;
    y->height = max(height_sensor_node(y->left), height_sensor_node(y->right)) + 1;

    return y;
}

sensor_node *create_sensor_database(sensor_node *root, sensor_node *node)
{
    if (!root)
    {
        printf("5");
        return (create_sensor_node(node->sensor_ID, node->sensor_type, node->sensor_station, node->duration));
    }
    if (node->sensor_ID < root->sensor_ID)
        root->left = create_sensor_database(root->left, node);
    else if (node->sensor_ID >= root->sensor_ID)
        root->right = create_sensor_database(root->right, node);
    else
        return root;
    root->height = 1 + max(height_sensor_node(root->left), height_sensor_node(root->right));

    int balance = get_Balance_sensor_database(root);

    if (balance > 1 && node->sensor_ID < root->left->sensor_ID)
        return right_Rotate_sensor_database(root);

    if (balance > -1 && node->sensor_ID > root->left->sensor_ID)
        return left_Rotate_sensor_database(root);

    if (balance > 1 && node->sensor_ID > root->left->sensor_ID)
    {
        root->left = left_Rotate_sensor_database(node->left);
        return right_Rotate_sensor_database(root);
    }
    return root;
}

record *create_central_repo_node(int SID, date d, time t, int data)
{
    record *new_node;
    new_node = (record *)malloc(sizeof(record));
    new_node->sensor_ID = SID;
    new_node->t = t;
    new_node->d = d;
    new_node->data = data;
    new_node->height = 1;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

int get_Balance_central_repo(record *node)
{
    if (node == NULL)
        return 0;
    return height_database_node(node->left) - height_database_node(node->right);
}


record *left_Rotate_central_repo(record *x)
{
    record *y = x->right;
    record *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height_database_node(x->left), height_database_node(x->right)) + 1;
    y->height = max(height_database_node(y->left), height_database_node(y->right)) + 1;

    return y;
}


record *right_Rotate_central_repo(record *y)
{
    record *x = y->left;
    record *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height_database_node(y->left), height_database_node(y->right)) + 1;
    x->height = max(height_database_node(x->left), height_database_node(x->right)) + 1;

    return x;
}


record *central_repository(record *root, record *node)
{
    if (!root)
        return (create_central_repo_node(node->sensor_ID, node->d, node->t, node->data));
    if (node->sensor_ID < root->sensor_ID)
        root->left = central_repository(root->left, node);
    else if (node->sensor_ID >= root->sensor_ID)
        root->right = central_repository(root->right, node);

    root->height = 1 + max(height_database_node(root->left), height_database_node(root->right));

    int balance = get_Balance_central_repo(root);

    if (balance > 1 && node->sensor_ID < root->left->sensor_ID)
        return right_Rotate_central_repo(root);

    if (balance > -1 && node->sensor_ID > root->left->sensor_ID)
        return left_Rotate_central_repo(root);

    if (balance > 1 && node->sensor_ID > root->left->sensor_ID)
    {
        root->left = left_Rotate_central_repo(node->left);
        return right_Rotate_central_repo(root);
    }

    if (balance > -1 && node->sensor_ID < root->left->sensor_ID)
    {
        root->right = right_Rotate_central_repo(root->right);
        return left_Rotate_central_repo(root);
    }
    return root;
}

int Sensor_check(sensor_node *root, char SStation[], char SType[])
{
    if (!root)
        return 0;
    if (root->sensor_station == SStation && root->sensor_type == SType)
        return 1;
    Sensor_check(root->left, SStation, SType);
    Sensor_check(root->right, SStation, SType);
}

sensor_node *Install_new_Sensor(char SStation[], char SType[])
{
    if (!Sensor_check(root_sensor_network, SStation, SType))
    {
        int SID;
        time duration;
        printf("Enter Sensor ID of sensor to be installed : ");
        scanf("%d", &SID);
        printf("Enter time interval between successive fetching data(in hour<space>min<space>sec format) : ");
        scanf("%d%d%d", &duration.hour, &duration.min, &duration.sec);
        root_sensor_network = create_sensor_database(root_sensor_network, create_sensor_node(SID, SType, SStation, duration));
        printf("Sensor installed\n");
    }
    else
    {
        printf("Sensor of %s type is already situated at %s\n", SType, SStation);
    }
    return root_sensor_network;
}

sensor_node *search_database_node(int SID, sensor_node *root)
{
    if (root->sensor_ID > SID)
        return search_database_node(SID, root->left);
    else if (root->sensor_ID == SID || !root)
        return root;
    else if (root->sensor_ID < SID)
        return search_database_node(SID, root->right);
}

record *search_central_repo_node(int SID, record *root)
{
    if (!root)
        return root;
    if (root->sensor_ID < SID)
        return search_central_repo_node(SID, root->right);
    else if (root->sensor_ID == SID)
        return root;
    else if (root->sensor_ID > SID)
        return search_central_repo_node(SID, root->left);
}

int day_count(date d1, date d2)
{
    return (d1.day - d2.day) + 30 * (d1.month - d2.month) + 365 * (d1.year - d2.year);
}

sensor_node *min_value_database_node(sensor_node *node)
{
    sensor_node *current = node;

    while (current->left)
        current = current->left;

    return current;
}

record *min_value_central_repo_node(record *node)
{
    record *current = node;

    while (current->left)
        current = current->left;

    return current;
}

sensor_node *delete_database_node(sensor_node *root, int SID)
{

    if (!root)
        return root;

    if (SID > root->sensor_ID)
        root->right = delete_database_node(root->right, SID);

    else if (SID < root->sensor_ID)
        root->left = delete_database_node(root->left, SID);

    else
    {
        if (!root->left || !root->right)
        {
            sensor_node *temp = root->left ? root->left : root->right;

            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
                *root = *temp;
            free(temp);
        }
        else
        {
            sensor_node *temp = min_value_database_node(root->right);

            root->sensor_ID = temp->sensor_ID;

            root->right = delete_database_node(root->right, temp->sensor_ID);
        }
    }

    if (!root)
        return root;

    root->height = 1 + max(height_sensor_node(root->left), height_sensor_node(root->right));

    int balance = get_Balance_sensor_database(root);

    if (balance > 1 && get_Balance_sensor_database(root->left) >= 0)
        return right_Rotate_sensor_database(root);

    if (balance > 1 && get_Balance_sensor_database(root->left) < 0)
    {
        root->left = left_Rotate_sensor_database(root->left);
        return right_Rotate_sensor_database(root);
    }

    if (balance < -1 && get_Balance_sensor_database(root->right) <= 0)
        return left_Rotate_sensor_database(root);

    if (balance < -1 && get_Balance_sensor_database(root->right) > 0)
    {
        root->right = right_Rotate_sensor_database(root->right);
        return left_Rotate_sensor_database(root);
    }

    return root;
}

record *delete_central_repo_node(record *root, int SID)
{

    if (!root)
        return root;

    if (SID > root->sensor_ID)
        root->right = delete_central_repo_node(root->right, SID);

    else if (SID < root->sensor_ID)
        root->left = delete_central_repo_node(root->left, SID);

    else
    {
        if (!root->left || !root->right)
        {
            record *temp = root->left ? root->left : root->right;

            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
                *root = *temp;
            free(temp);
        }
        else
        {
            record *temp = min_value_central_repo_node(root->right);

            root->sensor_ID = temp->sensor_ID;

            root->right = delete_central_repo_node(root->right, temp->sensor_ID);
        }
    }

    if (!root)
        return root;

    root->height = 1 + max(height_database_node(root->left), height_database_node(root->right));

    int balance = get_Balance_central_repo(root);

    if (balance > 1 && get_Balance_central_repo(root->left) >= 0)
        return right_Rotate_central_repo(root);

    if (balance > 1 && get_Balance_central_repo(root->left) < 0)
    {
        root->left = left_Rotate_central_repo(root->left);
        return right_Rotate_central_repo(root);
    }

    if (balance < -1 && get_Balance_central_repo(root->right) <= 0)
        return left_Rotate_central_repo(root);

    if (balance < -1 && get_Balance_central_repo(root->right) > 0)
    {
        root->right = right_Rotate_central_repo(root->right);
        return left_Rotate_central_repo(root);
    }

    return root;
}

void Idle_sensor(date d, sensor_node *root)
{
    if (!root)
        return;
    Idle_sensor(d, root->left);
    if (day_count(d, search_central_repo_node(root->sensor_ID, root_record)->d) > 60)
    {
        record *temp = search_central_repo_node(root->sensor_ID, root_record);
        while (temp)
        {
            root_record = delete_central_repo_node(root_record, temp->sensor_ID);
            temp = search_central_repo_node(root->sensor_ID, root_record);
        }
        root_sensor_network = delete_database_node(root_sensor_network, root->sensor_ID);
    }
    Idle_sensor(d, root->right);
}

void Retrieve_info(record *root, char SType[], date d_start, date d_end)
{
    if (!root)
        return;
    Retrieve_info(root->left, SType, d_start, d_end);
    if (!strcmp(SType, search_database_node(root->sensor_ID, root_sensor_network)->sensor_type) && (compare_date(root->d, d_start) != -1) && (compare_date(root->d, d_end) != 1))
        printf("%d\t\t%s\t\t%.2d/%.2d/%.2d\t%.2d:%.2d:%.2d\t%f\n", root->sensor_ID, SType, root->d.day, root->d.month, root->d.year, root->t.hour, root->t.min, root->t.sec, root->data);
    Retrieve_info(root->right, SType, d_start, d_end);
}

void sensors_in_between(record *root, int SID_start, int SID_end)
{
    if (!root)
        return;
    sensors_in_between(root->left, SID_start, SID_end);
    if (root->sensor_ID <= SID_end && root->sensor_ID >= SID_start)
    {
        printf("%d\t\t%.2d/%.2d/%.2d\t%.2d:%.2d:%.2d\t%f\n", root->sensor_ID, root->d.day, root->d.month, root->d.year, root->t.hour, root->t.min, root->t.sec, root->data);
    }
    sensors_in_between(root->right, SID_start, SID_end);
}

void print_database(sensor_node *root)
{
    if (!root)
        return;
    print_database(root->left);
    printf("%d\t\t%s\t\t%s\t\t%d:%d:%d\n", root->sensor_ID, root->sensor_type, root->sensor_station, root->duration.hour, root->duration.min, root->duration.sec);
    print_database(root->right);
}

void AQI_status_date(char status[], record *root)
{
    if (!root)
        return;
    char str[30];
    AQI_status_date(status, root->left);
    if (!strcmp(search_database_node(root->sensor_ID, root_sensor_network)->sensor_type, "AQI"))
    {

        switch (1 + (int)root->data / 50)
        {
        case 1:
            strcpy(str, "good");
            break;
        case 2:
            strcpy(str, "satisfactory");
            break;
        case 3:
            strcpy(str, "moderately polluted");
            break;
        case 4:
            strcpy(str, "moderately polluted");
            break;
        case 5:
            strcpy(str, "poor");
            break;
        case 6:
            strcpy(str, "poor");
            break;
        case 7:
            strcpy(str, "may cause respiratory illness");
            break;
        case 8:
            strcpy(str, "may cause respiratory illness");
            break;
        case 9:
            strcpy(str, "severe");
            break;
        case 10:
            strcpy(str, "severe");
            break;
        default:
            strcpy(str, "hazardous");
            break;
        }
        if (!strcmp(status, str))
        {
            printf("date with %s AQI status is : %d/%d/%d\n", status, root->d.day, root->d.month, root->d.year);
        }
    }
    AQI_status_date(status, root->right);
}

float max_pollution(record *root)
{
    if (!root)
        return 0;
    if (!strcmp(search_database_node(root->sensor_ID, root_sensor_network)->sensor_type, "AQI"))
    {
        if (root->data > max_pollution(root->left))
            return root->data;
        if (root->data > max_pollution(root->right))
            return root->data;
    }
}

int month_max_pollution(record *root)
{
    if (!root)
        return 0;
    if (!strcmp(search_database_node(root->sensor_ID, root_sensor_network)->sensor_type, "AQI"))
    {
        if (root->data > max_pollution(root->left))
            return root->d.month;
        if (root->data > max_pollution(root->right))
            return root->d.month;
    }
}

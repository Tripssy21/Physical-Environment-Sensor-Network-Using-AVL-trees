#include "Server_functions.c"

int main()
{
    printf("******************************************************************************************************************************************\n");
    printf("                                       WELCOME TO SERVER SYSTEM MANAGEMENT SOFTWARE\n");
    printf("******************************************************************************************************************************************\n");
    int option = 1;
    while (option)
    {
        printf("\nChoose the operation to be performed :\n\n");
        printf("Press 0 to exit\n");
        printf("Press 1 : Install new sensor\n");
        printf("Press 2 : Delete Idel sensor\n");
        printf("Press 3 : To retrieve information\n");
        printf("Press 4 : To print information of sensor range\n");
        printf("Press 5 : Input data\n"); //Just for testing
        printf("Press 6 : to view server database\n");
        printf("Press 7 : to view dates with hazardous status\n");
        printf("Press 8 : to find month with maximum pollution\n");
        printf("\nChoice : ");
        scanf("%d", &option);
        switch (option)
        {
            int SID_start, SID_end, SID, data;
            char SType[SIZE_TYPE], SStation[SIZE_STATION];
            date d_default, d_today;
            time t;
        case 1:
            printf("Enter sensor station : ");
            scanf("%s", SStation);
            fflush;
            printf("Enter sensor type : ");
            scanf("%s", SType);
            fflush;
            root_sensor_network = Install_new_Sensor(SStation, SType);
            break;
        case 2:
            printf("Enter todays date (in day<space>month<space>year format) : ");
            scanf("%d%d%d", &d_today.day, &d_today.month, &d_today.year);
            Idle_sensor(d_today, root_sensor_network);
            break;
        case 3:
            printf("Enter sensor type : ");
            scanf("%s", SType);
            int choice;
            printf("\nPress 1 : Retrieve information Depending on sensor type (retrieves till date data)\n");
            printf("\nPress 2 : Retrieve information Depending on specified date range (multiple dates) for specific sensor type\n");
            scanf("%d", &choice);
            if (choice == 1)
            {
                d_default.day = 0;
                d_default.month = 0;
                d_default.year = 0;
                printf("Enter todays date (in day<space>month<space>year format) : ");
                scanf("%d%d%d", &d_today.day, &d_today.month, &d_today.year);
                printf("Sensor ID\tSensor type\tDate\t\tTime\t\tData\n");
                Retrieve_info(root_record, SType, d_default, d_today);
            }
            else if (choice == 2)
            {
                date d_start, d_end;
                printf("Enter starting date (in day<space>month<space>year format) : ");
                scanf("%d%d%d", &d_start.day, &d_start.month, &d_start.year);
                printf("Enter ending date (in day<space>month<space>year format) : ");
                scanf("%d%d%d", &d_end.day, &d_end.month, &d_end.year);
                printf("Sensor ID\tSensor type\tDate\t\tTime\t\tData\n");
                Retrieve_info(root_record, SType, d_start, d_end);
            }
            else
                printf("Invalid choice, Please try again\n");
            break;
        case 4:
            printf("Starting sensor id : ");
            scanf("%d", &SID_start);
            printf("ending sensor id : ");
            scanf("%d", &SID_end);
            printf("Sensor ID\tDate\t\tTime\t\tData\n");
            sensors_in_between(root_record, SID_start, SID_end);
            break;
        case 5:
            printf("Enter Sensor id : ");
            scanf("%d", &SID);
            date d;
            printf("Enter date (in day<space>month<space>year format) : ");
            scanf("%d%d%d", &d.day, &d.month, &d.year);
            printf("Enter time (in hour<space>min<space>sec format) : ");
            scanf("%d%d%d", &t.hour, &t.min, &t.sec);
            printf("Enter data : ");
            scanf("%d", &data);
            root_record = central_repository(root_record, create_central_repo_node(SID, d, t, data));
            break;
        case 6:
            printf("Sensor ID\tSensor type\tSensor station\tDuration\n");
            print_database(root_sensor_network);
            break;
        case 7:
            AQI_status_date("hazardous", root_record);
            break;
        case 8:
            printf("month with maximum pollution : %d\n", month_max_pollution(root_record));
            break;
        default:
            if (option)
                printf("Invalid key, Please try again\n");
            break;
        }
    }
    return 0;
}

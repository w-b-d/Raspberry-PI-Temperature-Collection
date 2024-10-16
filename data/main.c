#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>


void menu();
void average(MYSQL *conn);
void min(MYSQL * conn);
void min(MYSQL * conn);


int main()
{



    MYSQL *conn;

    MYSQL_RES *res;

    MYSQL_ROW row;

    char *server = "localhost";

    char *user = "john";

    char *password = "password";

    char *database = "finalprojectdb";

    conn = mysql_init(NULL);


    if (!mysql_real_connect(conn, server,

                            user, password, database, 0, NULL, 0))

    {

        fprintf(stderr, "%s\n", mysql_error(conn));

        exit(1);
    }


    int choice;


    while(choice != -1)
    {
        menu();
        scanf("%d", &choice);

        switch(choice)
        {
        case-1:
        break;
        case 1:
            average(conn);
            break;
        case 2:
            min(conn);
            break;
        case 3:
        max(conn);
        break;





        }
    }





























    return 0;
}



void menu()
{

    puts("Enter an option from the menu:");
    puts("1: Calculate Average");
    puts("2: Calculate Min");
    puts("3: calculate Max");
    puts("-1: end");


}



void average(MYSQL *conn)
{
    int choice;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char sql[100];


    if (mysql_query(conn, "SELECT MIN(time), MAX(time) FROM data"))
    {
        fprintf(stderr, "Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    res = mysql_store_result(conn);
    if (res == NULL)
    {
        fprintf(stderr, "Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    row = mysql_fetch_row(res);
    if (row == NULL)
    {
        fprintf(stderr, "No data found in the table.\n");
        mysql_free_result(res);
        mysql_close(conn);
        exit(1);
    }
    puts("Enter a day range between the following for data: yyyy-mm-dd");


    printf("First day: %s\n", row[0]);
    printf("Last Day: %s\n", row[1]);
    char first_date[10];
    char last_date[10];
    puts("Enter first day");
    scanf("%s",&first_date);
    puts("Enter last day");
    scanf("%s", &last_date);


    puts("Average of temp: 1 , Average of Humid:2");
    scanf("%d", &choice);
    float temp_a;
    float humid_a;
    int count = 0;
    float total = 0;
    if(choice == 1)
    {
        sprintf(sql, "SELECT temp FROM data WHERE time >= '%s' AND time < '%s'", first_date, last_date);


        if (mysql_query(conn, sql) != 0)
        {
            fprintf(stderr, "mysql_query() failed: %s\n", mysql_error(conn));
            mysql_close(conn);
            return 1;
        }

        res = mysql_store_result(conn);
        if (res == NULL)
        {
            fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
            mysql_close(conn);
            return 1;
        }
        count = 0;
        total = 0;
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            total += atoi(row[0]);
            count += 1;



        }
        temp_a = total/ count;

        printf("The average temp is: %.2f ", temp_a);


    }
    if(choice == 2)
    {
        sprintf(sql, "SELECT humidity FROM data WHERE time >= '%s' AND time < '%s'", first_date, last_date);


        if (mysql_query(conn, sql) != 0)
        {
            fprintf(stderr, "mysql_query() failed: %s\n", mysql_error(conn));
            mysql_close(conn);
            return 1;
        }

        res = mysql_store_result(conn);
        if (res == NULL)
        {
            fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
            mysql_close(conn);
            return 1;
        }
        count = 0;
        total = 0;
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            total += atoi(row[0]);
            count += 1;



        }
        humid_a = total/ count;

        printf("The average humidity is: %.2f ", humid_a);


    }






}



void min(MYSQL * conn)
{
    int choice;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char sql[200];
    if (mysql_query(conn, "SELECT MIN(time), MAX(time) FROM data"))
    {
        fprintf(stderr, "Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    res = mysql_store_result(conn);
    if (res == NULL)
    {
        fprintf(stderr, "Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    row = mysql_fetch_row(res);
    if (row == NULL)
    {
        fprintf(stderr, "No data found in the table.\n");
        mysql_free_result(res);
        mysql_close(conn);
        exit(1);
    }
    puts("Enter a day range between the following for data: yyyy-mm-dd");


    printf("First day: %s\n", row[0]);
    printf("Last Day: %s\n", row[1]);
    char first_date[10];
    char last_date[10];
    puts("Enter first day");
    scanf("%s",&first_date);
    puts("Enter last day");
    scanf("%s", &last_date);

    puts("Choose an option\n1:Min Temp\n2: Min Humidity");
    scanf("%d", &choice);
    if(choice == 1 )
    {


        sprintf(sql, "SELECT MIN(temp) FROM data WHERE time >= '%s' AND time < '%s'", first_date, last_date);

        mysql_query(conn, sql);
        res = mysql_store_result(conn);

        row = mysql_fetch_row(res);
        printf("The min Temp is %s\n", row[0]);







    }

    if(choice == 2 )

    {


        sprintf(sql, "SELECT MIN(humidity) FROM data WHERE time >= '%s' AND time < '%s'", first_date, last_date);

        mysql_query(conn, sql);
        res = mysql_store_result(conn);

        row = mysql_fetch_row(res);
        printf("The min humidity is %s\n", row[0]);







    }

}

void max(MYSQL * conn)
{
    int choice;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char sql[200];

    if (mysql_query(conn, "SELECT MIN(time), MAX(time) FROM data"))
    {
        fprintf(stderr, "Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    res = mysql_store_result(conn);
    if (res == NULL)
    {
        fprintf(stderr, "Error %u: %s\n", mysql_errno(conn), mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    row = mysql_fetch_row(res);
    if (row == NULL)
    {
        fprintf(stderr, "No data found in the table.\n");
        mysql_free_result(res);
        mysql_close(conn);
        exit(1);
    }
    puts("Enter a day range between the following for data: yyyy-mm-dd");


    printf("First day: %s\n", row[0]);
    printf("Last Day: %s\n", row[1]);
    char first_date[10];
    char last_date[10];
    puts("Enter first day");
    scanf("%s",&first_date);
    puts("Enter last day");
    scanf("%s", &last_date);

    puts("Choose an option\n1:Max Temp\n2: Max Humidity");
    scanf("%d", &choice);
    if(choice == 1 )
    {


        sprintf(sql, "SELECT MAX(temp) FROM data WHERE time >= '%s' AND time < '%s'", first_date, last_date);

        mysql_query(conn, sql);
        res = mysql_store_result(conn);

        row = mysql_fetch_row(res);
        printf("The max Temp is %s\n", row[0]);







    }

    if(choice == 2 )

    {


        sprintf(sql, "SELECT MAX(humidity) FROM data WHERE time >= '%s' AND time < '%s'", first_date, last_date);

        mysql_query(conn, sql);
        res = mysql_store_result(conn);

        row = mysql_fetch_row(res);
        printf("the max Humidity is %s\n", row[0]);







    }




}





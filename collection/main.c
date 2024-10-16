

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>


#define MAXTIMINGS 85
#define DHTPIN 7

#include <mysql/mysql.h>



int LCDAddr = 0x27;
int BLEN = 1;
int fd;

int dht11_dat[5] = { 0, 0, 0, 0, 0 };





void write_word(int data)
{
    int temp = data;
    if ( BLEN == 1 )
        temp |= 0x08;
    else
        temp &= 0xF7;
    wiringPiI2CWrite(fd, temp);
}

void send_command(int comm)
{
    int buf;
// Send bit7-4 firstly
    buf = comm & 0xF0;
    buf |= 0x04; // RS = 0, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB; // Make EN = 0
    write_word(buf);

// Send bit3-0 secondly
    buf = (comm & 0x0F) << 4;
    buf |= 0x04; // RS = 0, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB; // Make EN = 0
    write_word(buf);
}

void send_data(int data)
{
    int buf;
// Send bit7-4 firstly
    buf = data & 0xF0;
    buf |= 0x05; // RS = 1, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB; // Make EN = 0
    write_word(buf);

// Send bit3-0 secondly
    buf = (data & 0x0F) << 4;
    buf |= 0x05; // RS = 1, RW = 0, EN = 1
    write_word(buf);
    delay(2);
    buf &= 0xFB; // Make EN = 0
    write_word(buf);
}

void init()
{
    send_command(0x33); // Must initialize to 8-line mode at first
    delay(5);
    send_command(0x32); // Then initialize to 4-line mode
    delay(5);
    send_command(0x28); // 2 Lines & 5*7 dots
    delay(5);
    send_command(0x0C); // Enable display without cursor
    delay(5);
    send_command(0x01); // Clear Screen
    wiringPiI2CWrite(fd, 0x08);
}

void clear()
{
    send_command(0x01); //clear Screen
}

void write(int x, int y, char data[])
{
    int addr, i;
    int tmp;
    if (x < 0) x = 0;
    if (x > 15) x = 15;
    if (y < 0) y = 0;
    if (y > 1) y = 1;

// Move cursor
    addr = 0x80 + 0x40 * y + x;
    send_command(addr);

    tmp = strlen(data);
    for (i = 0; i < tmp; i++)
    {
        send_data(data[i]);
    }
}


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

    if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0))

    {

        fprintf(stderr, "%s\n", mysql_error(conn));

        exit(1);

    }

    if (mysql_query(conn, "show tables"))

    {

        fprintf(stderr, "%s\n", mysql_error(conn));

        exit(1);

    }
    res = mysql_use_result(conn);


    printf("MySQL Tables in data database:\n");

    while ((row = mysql_fetch_row(res)) != NULL)

        printf("%s \n", row[0]);







    wiringPiSetup();
    //int lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);


    printf( "Raspberry Pi wiringPi DHT11 Temperature test program\n" );
    /*  if ( wiringPiSetup() == -1 )
          exit( 1 );
      while ( 1 )
      {
          read_dht11_dat();
          delay( 1000 );
      }
      return(0); */

    while(1)
    {


        time_t current_time;
        struct tm * time_info;
        char timeString[7];  // space for "HH:MM:SS\0"
        char out[80];
        char* dot = ".";

        int w,x,y,z;




        time(&current_time);
        time_info = localtime(&current_time);


        read_dht11_dat();


        w = dht11_dat[0] ;
        x = dht11_dat[1];
        y = dht11_dat[2];
        z = dht11_dat[3] ;

        strftime(timeString, sizeof(timeString), "%H:%M", time_info);

        sprintf(out,"%d.%d %d.%d %s",w,x,y,z,timeString);



        char convert[15];

        sprintf(convert, "%d.%d %d.%d\0", w,x,y,z);

        double humid2 ;
        double temp2;

        sscanf(convert,"%lf %lf",&humid2,&temp2);


        if(humid2 != 0 && temp2 != 0  )
        {

        if(humid2 > 0 && temp2 > 0){

        if(temp2 < 45){



            char query[100];


            sprintf(query, "INSERT INTO data VALUES (0,NOW(),%f, %f)", humid2, temp2);


            if (mysql_query(conn, query))
            {
                fprintf(stderr, "Error: %s\n", mysql_error(conn));
                return 1;
            }

            printf("Data inserted successfully!\n");
            delay(5000);

            fd = wiringPiI2CSetup(LCDAddr);
            init();
            write(0, 0, "Humi Temp Time ");
            write(1, 1, out);
            }}





















        }







    }

    mysql_free_result(res);

    mysql_close(conn);
// clear();
}


void read_dht11_dat()
{
    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;
    float f;
    dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;
    pinMode( DHTPIN, OUTPUT );
    digitalWrite( DHTPIN, LOW );
    delay( 18 );
    digitalWrite( DHTPIN, HIGH );
    delayMicroseconds( 40 );
    pinMode( DHTPIN, INPUT );
    for ( i = 0; i < MAXTIMINGS; i++ )
    {
        counter = 0;
        while ( digitalRead( DHTPIN ) == laststate )
        {
            counter++;
            delayMicroseconds( 1 );
            if ( counter == 255 )
            {
                break;
            }
        }
        laststate = digitalRead( DHTPIN );
        if ( counter == 255 )
            break;
        if ( (i >= 4) && (i % 2 == 0) )
        {
            dht11_dat[j / 8] <<= 1;
            if ( counter > 16 )
                dht11_dat[j / 8] |= 1;
            j++;
        }
    }
    if ( (j >= 40) &&
            (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )
    {
        if (!(dht11_dat[0] == 0 && dht11_dat[1] == 0 && dht11_dat[2] == 0 && dht11_dat[3] == 0))
        {


            f = dht11_dat[2] * 9. / 5. + 32;

            printf( "Humidity = %d.%d %% Temperature = %d.%d C (%.1f F)\n",
                    dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f );
        }



    }
    else
    {
        printf( "Data not good, skip\n" );
    }
}



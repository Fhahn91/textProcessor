/* Program that opens and scans a text for data, using structures to form a database. The data is scanned and stored from the text file,
 * into the database structure and then calculated and stored in another structure which is used to display information */


  /* Directions: When entering value of text file you wish to search through make sure to add .txt to the end of each file */

#include <stdio.h>
#include <stdlib.h>

/* Macro that defines the amount of days to be scanned */
#define MAXDAYS 7
#define AMNTOFSITES 3
#define MAXNAMELEN 50

/* User defined structure to hold data */
typedef struct
{
    int site_id_num,
        wnd_spd,
        day_of_mnth,
        temp;
}measured_data_t;

/* User defined structure to hold processed data */
typedef struct
{
    float AvgWndSpd[AMNTOFSITES];
    int   extrema[AMNTOFSITES],
          extremaID[AMNTOFSITES],
          wndSpdID[AMNTOFSITES];
}calcData_t;

/* Prototypes */
int getExtrema(measured_data_t metData[MAXDAYS*AMNTOFSITES], calcData_t *calcPlace);
int getAvgWndSpd(measured_data_t metData[MAXDAYS*AMNTOFSITES], calcData_t *calcPlace);
int fscanGetData(FILE *filePlace, measured_data_t *dataPlace);
void printCalcData(calcData_t calcPlace);

/* Main Driver Function */
int main()
{
    /* Declaring Variables and user-defined structures to be used in main */
    int i;
    measured_data_t dataBsData[MAXDAYS*AMNTOFSITES];
    calcData_t dataBsCalc;

    char textfileName[MAXNAMELEN];

    printf("Please enter the name of the database.txt you'd wish to search(Ex. database.Txt): ");
    scanf("%s", textfileName);

    /* Opening Text File */
    FILE *fileData;
    fileData = fopen(textfileName ,"r");

    /* Scanning file and storing in database structure */
    for ( i = 0; i < (MAXDAYS*AMNTOFSITES) ; i++)
    {
        fscanGetData(fileData, &dataBsData[i]);
    }


    /* Calling Functions to Calculate, Sort, and Display data from Meteorlogical Data text file */
    getExtrema(dataBsData, &dataBsCalc);
    getAvgWndSpd(dataBsData, &dataBsCalc);
    printCalcData(dataBsCalc);

    return(0);
}

/* Function that scans file and places data in measured_data_t structure */
int
fscanGetData(FILE *filePlace, measured_data_t *dataPlace)
{
    int result;

    result = fscanf(filePlace, " %d %d %d %d",   &dataPlace->site_id_num,
                                                 &dataPlace->day_of_mnth,
                                                 &dataPlace->wnd_spd,
                                                 &dataPlace->temp);
    /* Checks for successful input */
    if (result == 4)
        result = 1;
    else if (result != EOF)
        result = 0;


    return(result);
}

/* Function that sorts the data in descending order for a given site and calculates highest extrema by subtracting largest value from
 * smallest value, calculated values are stored in CalcData structure */
int
getExtrema(measured_data_t metData[MAXDAYS*AMNTOFSITES], calcData_t *calcPlace)
{
    int dayCnt,
        siteCnt,
        tempData,
        siteTemp,
        tempSrt,
        sortCnt,
        sortCntr;

    /* Sorting data and calculating Extrema */
    for ( siteCnt = 0 ; siteCnt < AMNTOFSITES; siteCnt++)
    {

        for ( dayCnt = 0; dayCnt < MAXDAYS; dayCnt++)
            {
                tempData = metData[dayCnt].temp;
                /* printf("\n%d*", tempData); */

                if (&metData[dayCnt + (siteCnt*MAXDAYS)].temp > &metData[dayCnt + (siteCnt*MAXDAYS)+1].temp)
                {
                    metData[dayCnt + (siteCnt*MAXDAYS)].temp = metData[dayCnt + (siteCnt*MAXDAYS)+1].temp;
                    /* printf("\n%d", metData[dayCnt + (siteCnt*MAXDAYS)].temp); */
                    metData[dayCnt + (siteCnt*MAXDAYS)+1].temp = tempData;
                }
            }

        calcPlace->extrema[siteCnt] = (metData[(MAXDAYS*(siteCnt + 1))-MAXDAYS].temp - metData[(MAXDAYS*(siteCnt + 1))-1].temp);
        calcPlace->extremaID[siteCnt] = metData[MAXDAYS * siteCnt].site_id_num;
    }

    /* Determining highest value by sorting */
    for ( sortCntr = 0; sortCntr < AMNTOFSITES; sortCntr++)
        {
            tempSrt = 0;
            siteTemp = 0;

            for ( sortCnt = 0; sortCnt < AMNTOFSITES - 1; sortCnt++)
            {
                /* printf("\n%d",sortCnt); */

                tempSrt = calcPlace->extrema[sortCnt];
                siteTemp = calcPlace->extremaID[sortCnt];

                /* printf("\n%d", siteTemp);
                 * printf("\n%d", tempSrt);
                 * getch();
                 */

                if (calcPlace->extrema[sortCnt] < calcPlace->extrema[sortCnt+1])
                {
                    calcPlace->extrema[sortCnt] = calcPlace->extrema[sortCnt+1];
                    calcPlace->extremaID[sortCnt] = calcPlace->extremaID[sortCnt+1];
                    calcPlace->extrema[sortCnt+1] = tempSrt;
                    calcPlace->extremaID[sortCnt+1] = siteTemp;
                }
            }
        }

    return(0);
}

/* Function that calculates the average wind speed and sorts the data in descending order for a given site and stores in CalcData structure*/
int
getAvgWndSpd(measured_data_t metData[MAXDAYS*AMNTOFSITES], calcData_t *calcPlace)
{
    int     siteCnt,
            dayCnt,
            sortCnt,
            sortCntr,
            tempID;
    float   ttlWndSpd,
            tempData;

        ttlWndSpd = 0;
        /* Finding average of wind speeds data for a given site location */
        for ( siteCnt = 0; siteCnt < AMNTOFSITES; siteCnt++)
        {
            ttlWndSpd = 0;
            for ( dayCnt = 0; dayCnt < MAXDAYS; dayCnt++)
            {
                ttlWndSpd += metData[dayCnt + ( siteCnt * MAXDAYS )].wnd_spd;
                /*printf("\n%f", ttlWndSpd); */
            }
            calcPlace->AvgWndSpd[siteCnt] = (ttlWndSpd / MAXDAYS);
            calcPlace->wndSpdID[siteCnt] = metData[siteCnt * MAXDAYS].site_id_num;
        }

        /* Sorts average wind speeds from highest to lowest */
        for ( sortCntr = 0; sortCntr < AMNTOFSITES; sortCntr++)
        {
            for ( sortCnt = 0; sortCnt < AMNTOFSITES; sortCnt++)
            {
                tempData = calcPlace->AvgWndSpd[sortCnt];
                tempID = calcPlace->wndSpdID[sortCnt];
                if (calcPlace->AvgWndSpd[sortCnt] < calcPlace->AvgWndSpd[sortCnt+1])
                {
                    calcPlace->AvgWndSpd[sortCnt] = calcPlace->AvgWndSpd[sortCnt+1];
                    calcPlace->wndSpdID[sortCnt] = calcPlace->wndSpdID[sortCnt+1];
                    calcPlace->AvgWndSpd[sortCnt+1] = tempData;
                    calcPlace->wndSpdID[sortCnt+1] = tempID;
                }
            }
        }

    return(0);
}

/* Function that prints data previously calculated, displaying the highest wind speed and extrema temperature and their respective site IDs */
void
printCalcData(calcData_t calcPlace)
{
    printf("\n%s\t%s", "The Site with the Highest Wind Speed Average", "The Site with the Highest Extrema Temperature");
    printf("\n--------------------------------------------\t--------------------------------------------");
    printf("\nSiteID: %d - Wind Speed:%.2f(Knots)\t\tSiteID: %d - Temperature: %dC\370", calcPlace.wndSpdID[0], calcPlace.AvgWndSpd[0], calcPlace.extremaID[0], calcPlace.extrema[0]);
}

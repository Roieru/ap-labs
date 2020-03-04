#include <stdio.h>

static char daytab[2][12] = {
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

static char *month_names[] = {
    "Jan", "Feb", "Mar", "Apr",
    "May", "Jun", "Jul", "Aug",
    "Sep", "Oct", "Nov", "Dec"
};

void month_day(int year, int yearday, int *pmonth, int *pday){
    
    int leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
    
    if(yearday<1 || (leap && yearday>366) || (!leap && yearday>365)) {
        printf("Not a valid yearday.\n");
    } else{
        int i = 0;
        int days = *(*(daytab + leap) + i);
        
        while(yearday>days){
            yearday = yearday - days;
            i++;
            days = *(*(daytab + leap) + i);
        }
        
        *pmonth = i;
        *pday = yearday;
        
    }
    
}

int main(int argc, char **argv)
{
    
    int year = atoi(argv[1]);
    
    int yearday = atoi(argv[2]);
    
    int pmonth = 0;
    
    int pday = 0;

    month_day(year,yearday,&pmonth,&pday);

    if(pday>9){
        printf("%s %d, %d\n",month_names[pmonth],pday,year);
    } else if(pday>0){
        printf("%s 0%d, %d\n",month_names[pmonth],pday,year);
    } 

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void show_help(void)
{
    puts("If you want to create file-enter file name in command line");
    puts("If you want to work with file-enter file name in command line");
}

char* check_input(void)
{
    char *string=(char*)calloc(9,sizeof(char));
    do {
        fgets(string,9, stdin);
        if(strlen(string)==8)
            while(getchar()!='\n');
    } while (string[0] == '\n');
    return string;
}

void add_info(FILE *fl)
{
    int length,i,j,element;
    do
    {
        puts("enter length of array");
        length=atoi(check_input());
    }while(!length || length==1);
    fwrite(&length,sizeof(int),1,fl);
    printf("enter numbers:\n");
    for(i=0;i<length;i++)
    {
        for(j=0;j<length;j++)
        {
            element=atoi(check_input());
            fwrite(&element,sizeof(int),1,fl);
        }
    }
}

void fill_array(int **array, int length, FILE *fl)
{
    int num,i,j;
    for(i=0;i<length;i++)
        for(j=0;j<length;j++)
        {
            fread(&num,sizeof(int),1,fl);
            array[i][j]=num;
        }
}

void show_array(int **array, int length)
{
    int i,j;
    for(i=0;i<length;i++)
    {
        printf("\n");
        for(j=0;j<length;j++)
            printf("%3d",array[i][j]);
    }
}

void count_el(int **array,int length, FILE *fl)
{
    int i,j,sum=0;
    for(i=1;i<length;i++)
        for(j=0;j<i;j++)
        {
            sum+=array[i][j];
        }
    printf("\nsum=%d\n",sum);
    fseek(fl,0,2);
    fwrite(&sum,sizeof(int),1,fl);
}

create_array(FILE *fl)
{
    int **array,length,height,i,j;
    rewind(fl);
    fread(&length,sizeof(int),1,fl);
    printf("\n%d length",length);
    do
    {
        array=(int**)calloc(length,sizeof(int*));
        for(i=0;i<length;i++)
        {
            array[i]=(int*)calloc(length,sizeof(int));
            if(!array[i])
            for(j=i;j>=0;j--)
            {
                free(array[j]);
                free(array);
                array=NULL;
            }
        }
    }while(!array);
    fill_array(array,length,fl);
    show_array(array,length);
    count_el(array,length,fl);
}

int main(int argc,char *argv[])
{
    char filename[10];
    if(argc==1)
    {
        show_help();
    }
    FILE *fl;
    int i=0;
    puts("1-create binary file");
    puts("2-use file");
    i=atoi(check_input());
    if(i==1)
    {
        if(!(fl=fopen(argv[1],"a+b")))
        {
            puts("error, cannot create file");
            return;
        }

    }
    if(i==2)
    {
        if(!(fl=fopen(argv[1],"w+b")))
        {
            puts("error, cannot create file");
            return;
        }

    }
    strcpy(filename,argv[1]);
    rewind(fl);
    add_info(fl);
    if(fclose(fl))
        puts("error, cannot close file");
    if(!(fl=fopen(filename,"rb")))
    {
        puts("error, cannot open file");
        return;
    }
    create_array(fl);
    if(fclose(fl))
        puts("error, cannot close file");

    return 0;
}

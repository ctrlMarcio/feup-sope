#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

const char *FILE_NAME = "structs";
const int MAX_SIZE = 50;

struct student_grade
{
    char name[50];
    int grade;
};

int main()
{
    int fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0700);
    if (fd == -1)
    {
        perror("Error");
        return 1;
    }

    write(STDOUT_FILENO, "Student,Grade (RETURN to stop)\n................................\n", 64);

    char buffer[MAX_SIZE];
    char *name, *grade;
    while (read(STDIN_FILENO, buffer, MAX_SIZE) > 0 && buffer[0] != '\n')
    {
        name = strtok(buffer, ",");
        grade = strtok(NULL, ",");

        if (!grade)
        {
            write(STDOUT_FILENO, "Insert grade pretty please\n", 27);
            continue;
        }

        struct student_grade student;
        student.grade = atoi(grade);
        strncpy(student.name, name, strlen(name));

        write(fd, &student, sizeof(student));

        memset(buffer, 0, sizeof(buffer));
    }

    close(fd);
    return 0;
}
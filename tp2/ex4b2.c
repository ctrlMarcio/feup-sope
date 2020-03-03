#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const int MAX_SIZE = 50;

struct student_grade
{
    char name[50];
    int grade;
};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        write(STDERR_FILENO, "Usage: ", 7);
        write(STDERR_FILENO, argv[0], strlen(argv[0]));
        write(STDERR_FILENO, " <structs_name>\n", 16);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        perror(argv[0]);
        return 1;
    }

    struct student_grade student;

    while (read(fd, &student, sizeof(student)))
    {
        write(STDOUT_FILENO, "Name: ", 6);
        write(STDOUT_FILENO, student.name, strlen(student.name));
        write(STDOUT_FILENO, "\nGrade: ", 8);
        char grade[5];
        sprintf(grade, "%d", student.grade);
        write(STDOUT_FILENO, grade, strlen(grade));
        write(STDOUT_FILENO, "\n", 1);
    }

    close(fd);
    return 0;
}
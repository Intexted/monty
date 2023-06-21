#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "monty.h"
#define MAX_LINE_LENGTH 1024

void opcode_push(stack_t **stack, unsigned int line_number);
void opcode_pall(stack_t **stack, unsigned int line_number);
int is_number(const char *str);

/**
 * main - Entry point
 * @argc: Number of command-line arguments
 * @argv: Array of command-line arguments
 *
 * Return: 0 on success
 */
int main(int argc, char *argv[])
{
	FILE *file;
	char line[MAX_LINE_LENGTH];
	unsigned int line_number = 0;
	stack_t *stack = NULL;
	char *opcode;
	void (*opcode_func)(stack_t **stack, unsigned int line_number);
	int i;
	instruction_t opcodes[] = {
		{"push", opcode_push},
		{"pall", opcode_pall},
		/* Add more opcode-function pairs here */
		{NULL, NULL}
	};
	if (argc != 2)
	{
		fprintf(stderr, "USAGE: monty file\n");
		exit(EXIT_FAILURE);
	}
	file = fopen(argv[1], "r");
	if (file == NULL)
	{
		fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	while (fgets(line, MAX_LINE_LENGTH, file) != NULL)
	{
		line_number++;
		opcode = strtok(line, " \t\n");
		if (opcode == NULL || strncmp(opcode, "#", 1) == 0)
			continue;
		opcode_func = NULL;
		for (i = 0; opcodes[i].opcode != NULL; i++)
		{
			if (strcmp(opcode, opcodes[i].opcode) == 0)
			{
				opcode_func = opcodes[i].f;
				break;
			}
		}
		if (opcode_func == NULL)
		{
			fprintf(stderr, "L%d: unknown instruction %s\n", line_number, opcode);
			exit(EXIT_FAILURE);
		}
		opcode_func(&stack, line_number);
	}
	fclose(file);
	exit(EXIT_SUCCESS);
}


/**
 * opcode_push - Pushes an element to the stack
 * @stack: Double pointer to the stack
 * @line_number: Line number being executed
 */
void opcode_push(stack_t **stack, unsigned int line_number)
{
	stack_t *new_node;
	char *n;

	n = strtok(NULL, " \t\n");
	if (n == NULL || !is_number(n))
	{
		fprintf(stderr, "L%d: usage: push integer\n", line_number);
		exit(EXIT_FAILURE);
	}
	new_node = malloc(sizeof(stack_t));
	if (new_node == NULL)
	{
		fprintf(stderr, "Error: malloc failed\n");
		exit(EXIT_FAILURE);
	}

	new_node->n = atoi(n);
	new_node->next = *stack;
	*stack = new_node;
}

/**
 * opcode_pall - Prints all elements in the stack
 * @stack: Double pointer to the stack
 * @line_number: Line number being executed
 */
void opcode_pall(stack_t **stack, unsigned int line_number)
{
	stack_t *current;
	(void)line_number;

	current = *stack;
	while (current != NULL)
	{
		printf("%d\n", current->n);
		current = current->next;
	}
}

/**
 * is_number - Checks if a string represents a number
 * @str: String to check
 *
 * Return: 1 if string is a number, 0 otherwise
 */
int is_number(const char *str)
{
	if (str == NULL || *str == '\0')
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	for (; *str != '\0'; str++)
	{
		if (!isdigit(*str))
			return (0);
	}
	return (1);
}

#include "monty.h"
#define _GNU_SOURCE

/* Global variable */
stack_t *stack = NULL;

/* Function implementations for opcode functions */

void opcode_push(stack_t **stack, unsigned int line_number)
{
    int value;
    stack_t *new_node = malloc(sizeof(stack_t));
    char *value_str = strtok(NULL, " \t\n");
    if (value_str == NULL)
    {
        fprintf(stderr, "L%u: usage: push integer\n", line_number);
        exit(EXIT_FAILURE);
    }

    value = atoi(value_str);

    if (new_node == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        exit(EXIT_FAILURE);
    }

    new_node->n = value;
    new_node->prev = NULL;
    new_node->next = *stack;

    if (*stack != NULL)
        (*stack)->prev = new_node;

    *stack = new_node;
}

void opcode_pall(stack_t **stack, unsigned int line_number __attribute__((unused)))
{
    /* Implementation for pall opcode */
    /* Prints all values in the stack */
    /* Access stack and print values */
    stack_t *current = *stack;

    while (current != NULL)
    {
        printf("%d\n", current->n);
        fflush(stdout); 
        current = current->next;
    }
}


/* Main function */

int main(int argc, char *argv[])
{
   
    char *line = NULL;

    unsigned int line_number = 1;
    FILE *file = fopen(argv[1], "r");
    size_t len = 0;

	/* Check if file argument is provided */
    if (argc != 2)
    {
        fprintf(stderr, "USAGE: monty file\n");
        exit(EXIT_FAILURE);
    }

    /* Open and read the Monty byte code file */
    if (file == NULL)
    {
        fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    /* Process each line of the file */
    while (fgets(line, len, file) != NULL)
    {
        /* Tokenize line to extract opcode and arguments */
        char *opcode = strtok(line, " \t\n");

        if (opcode != NULL && opcode[0] != '#') /* Ignore comments */
        {
            /* Match opcode with corresponding function from instruction_t structure and execute it */
            if (strcmp(opcode, "push") == 0)
                opcode_push(&stack, line_number);
            else if (strcmp(opcode, "pall") == 0)
                opcode_pall(&stack, line_number);
            else
            {
                fprintf(stderr, "L%u: unknown instruction %s\n", line_number, opcode);
                exit(EXIT_FAILURE);
            }
        }

        line_number++;
    }

    /* Clean up and close the file */
    fclose(file);
    free(line);

    return EXIT_SUCCESS;
}


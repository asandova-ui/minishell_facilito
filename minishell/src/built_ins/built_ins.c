
#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

int ft_isspace(int c) {
    return (c == ' ' || c == '\t' || c == '\n' || 
            c == '\r' || c == '\v' || c == '\f');
}

char* first_words(char* str)
{
    int len;
    char *end;

    len = 0;
    while (*str && ft_isspace((unsigned char)*str))
    {
        str++;
    }
    if (*str == '\0')
        return NULL;
    end = str;
    while (*end && !ft_isspace((unsigned char)*end))
    {
        end++;
    }
    len = end - str + 1;
    char* word = (char*)malloc(len + 1);
    if (word == NULL) {
        return NULL;
    }
    // Copiar la palabra a la nueva memoria y añadir el carácter nulo
    ft_strlcpy(word, str, len);
    word[len] = '\0';
    return (word);
}
void built_ins(char *line, t_minish *mini)
{
    char *first_word;
    first_word = first_words(line);
    if (first_word == NULL)
        return;

    if (ft_strcmp(first_word, "echo") == 0)
    {
        char *args = strtok(NULL, "\n");
        if (args != NULL && mini->comillas != 1)
            ft_echo(args);
    }
    else if (ft_strcmp(first_word, "cd") == 0)
    {
        // Implement cd functionality
    }
    else if (ft_strcmp(first_word, "pwd") == 0)
    {
        ft_pwd();
    }
    else if (ft_strcmp(first_word, "export") == 0)
    {
        ft_export(line, mini);
    }
    else if (ft_strcmp(first_word, "unset") == 0)
    {
        // Implement unset functionality
    }
    else if (ft_strcmp(first_word, "env") == 0)
    {
        ft_env(mini->env);
    }
    else if (ft_strcmp(first_word, "exit") == 0)
    {
		mini->exit = 1;
        //exit_command();
    }
    free (first_word);
}

int ft_strcmp(const char *str1, const char *str2)
{
    while (*str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
    }
    return (*(unsigned char *)str1 - *(unsigned char *)str2);
}
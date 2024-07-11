
#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

void	ft_cd(char *path, t_minish *mini)
{
	char cwd[PATH_MAX];
	char *oldpwd;

	//Obtiene el directorio actual
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		return ;
	}
	//Lo guardar OLD_PWD
	oldpwd = ft_strdup(cwd);
	if (oldpwd == NULL)
	{
		perror("strdup");
		return ;
	}
	//Intenta cambiar el directorio
	if (chdir(path) != 0)
	{
		ft_printf("cd: no such file or directory: %s\n", path);
		free(oldpwd);
		return ;
	}
	//Actualiza OLDPWD en env
	add_or_update_env_var(mini, "OLDPWD", oldpwd);
	free(oldpwd);
	//Obtiene el nuevo directorio actual
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		return ;
	}
	//Actualiza PWD en env
	add_or_update_env_var(mini, "PWD", cwd);
}

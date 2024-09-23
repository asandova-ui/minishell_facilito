/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:34:22 by alonso            #+#    #+#             */
/*   Updated: 2024/09/23 13:34:35 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

int	ft_cd(char *path, t_minish *mini)
{
	char cwd[PATH_MAX];
	char *oldpwd;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		return (1);
	}
	oldpwd = ft_strdup(cwd);
	if (oldpwd == NULL)
	{
		perror("strdup");
		return (1);
	}
	if (chdir(path) != 0)
	{
		ft_printf("cd: no such file or directory: %s\n", path);
		free(oldpwd);
		return (1);
	}
	add_or_update_env_var(mini, "OLDPWD", oldpwd);
	free(oldpwd);
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		return (1);
	}
	add_or_update_env_var(mini, "PWD", cwd);
	return (0);
}
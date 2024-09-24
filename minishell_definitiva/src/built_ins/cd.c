/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jamorale <jamorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:34:22 by alonso            #+#    #+#             */
/*   Updated: 2024/09/24 01:16:27 by jamorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../printf/includes/ft_printf.h"
#include "../../printf/libft/libft.h"

int	get_current_dir(t_cd_ctx *ctx)
{
	if (getcwd(ctx->cwd, sizeof(ctx->cwd)) == NULL)
	{
		perror("getcwd");
		return (0);
	}
	return (1);
}

int	change_dir(char *path, t_cd_ctx *ctx)
{
	if (chdir(path) != 0)
	{
		ft_printf("cd: no such file or directory: %s\n", path);
		free(ctx->oldpwd);
		return (0);
	}
	return (1);
}

void	update_env_vars(t_cd_ctx *ctx)
{
	add_or_update_export_env_var(ctx->mini, "OLDPWD", ctx->oldpwd);
	add_or_update_export_env_var(ctx->mini, "PWD", ctx->cwd);
}

int	ft_cd(char *path, t_minish *mini)
{
	t_cd_ctx	ctx;

	ctx.mini = mini;
	if (!get_current_dir(&ctx))
		return (1);
	ctx.oldpwd = ft_strdup(ctx.cwd);
	if (ctx.oldpwd == NULL)
	{
		perror("strdup");
		return (1);
	}
	if (!change_dir(path, &ctx))
		return (1);
	if (!get_current_dir(&ctx))
		return (1);
	update_env_vars(&ctx);
	free(ctx.oldpwd);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alonso <alonso@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 01:08:17 by jamorale          #+#    #+#             */
/*   Updated: 2024/10/01 09:48:14 by alonso           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"
#include "../../../printf/includes/ft_printf.h"
#include "../../../printf/libft/libft.h"

void	swap(void *a, void *b, size_t size)
{
	void	*tmp;

	tmp = malloc(size);
	if (!tmp)
		return ;
	ft_memcpy(tmp, a, size);
	ft_memcpy(a, b, size);
	ft_memcpy(b, tmp, size);
	free(tmp);
}

void	adjust_indices(size_t *i, size_t *j, char *pivot, t_qsort_ctx *ctx)
{
	while (ctx->compar(ctx->base + (*i) * ctx->size, pivot) < 0)
		(*i)++;
	while (ctx->compar(ctx->base + (*j) * ctx->size, pivot) > 0)
		(*j)--;
}

size_t	partition(size_t nitems, char **pivot, t_qsort_ctx *ctx)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = nitems - 1;
	while (1)
	{
		adjust_indices(&i, &j, *pivot, ctx);
		if (i >= j)
			break ;
		swap(ctx->base + i * ctx->size, ctx->base + j * ctx->size, ctx->size);
		if (*pivot == ctx->base + i * ctx->size)
			*pivot = ctx->base + j * ctx->size;
		else if (*pivot == ctx->base + j * ctx->size)
			*pivot = ctx->base + i * ctx->size;
		i++;
		j--;
	}
	return (i);
}

void	ft_qsort(void *base, size_t nitems, size_t size,
		int (*compar)(const void *, const void *))
{
	t_qsort_ctx	ctx;
	char		*pivot;
	size_t		partition_idx;

	if (nitems < 2)
		return ;
	ctx.base = base;
	ctx.size = size;
	ctx.compar = compar;
	pivot = base + (nitems / 2) * size;
	partition_idx = partition(nitems, &pivot, &ctx);
	ft_qsort(base, partition_idx, size, compar);
	ft_qsort(base + partition_idx * size, nitems - partition_idx, size, compar);
}

void	ft_sprintf(char **dest, const char *name, const char *value)
{
	int	name_len;
	int	value_len;
	int	total_len;

	name_len = ft_strlen(name);
	value_len = ft_strlen(value);
	total_len = name_len + 1 + value_len;
	*dest = (char *)malloc(total_len + 1);
	if (*dest == NULL)
	{
		perror("Error allocating memory");
		return ;
	}
	ft_strcpy(*dest, name);
	ft_strcat(*dest, "=");
	ft_strcat(*dest, value);
}

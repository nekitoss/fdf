/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preparation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nekitoss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/24 21:23:16 by nekitoss          #+#    #+#             */
/*   Updated: 2017/05/24 21:23:17 by nekitoss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "h_fdf.h"

void		find_height(t_fdf *ls)
{
	int	i;
	int	j;

	ls->max_h = (ls->arr[0][0]).z;
	ls->min_h = (ls->arr[0][0]).z;
	i = 0;
	while (i < ls->num_rows)
	{
		j = 0;
		while (j < ls->num_cols)
		{
			if (ls->max_h < (ls->arr[i][j]).z)
				ls->max_h = (ls->arr[i][j]).z;
			if (ls->min_h > (ls->arr[i][j]).z)
				ls->min_h = (ls->arr[i][j]).z;
			j++;
		}
		i++;
	}
}

void		make_color(t_fdf *ls)
{
	int	i;
	int	j;
	int red;
	int blue;

	find_height(ls);
	i = 0;
	while (i < ls->num_rows)
	{
		j = 0;
		while (j < ls->num_cols)
		{
			red = (255 / (ls->max_h - ls->min_h + 1)) * ((ls->orig)[i][j]).z;
			blue = 255 - red;
			((ls->orig)[i][j]).color = red;
			((ls->orig)[i][j]).color = ((ls->orig)[i][j]).color << 16;
			((ls->orig)[i][j]).color += blue;
			((ls->arr)[i][j]).color = ((ls->orig)[i][j]).color;
			j++;
		}
		i++;
	}
}

void		convert_row(t_fdf *ls, char **row, int r_num)
{
	int	i;

	i = 0;
	if (ls->num_cols &&
		ls->num_cols == (int)ft_arrlen((void **)row))
	{
		ls->orig[r_num] = (t_point *)ft_memalloc(sizeof(t_point)
			* ls->num_cols);
		ls->arr[r_num] = (t_point *)ft_memalloc(sizeof(t_point) * ls->num_cols);
		while (i < ls->num_cols)
		{
			((ls->orig)[r_num][i]).x = (FLOAT_T)i;
			((ls->orig)[r_num][i]).y = (FLOAT_T)r_num;
			((ls->orig)[r_num][i]).z = (FLOAT_T)ft_atoi(row[i]);
			i++;
		}
		ft_memcpy((void *)ls->arr[r_num], (void *)ls->orig[r_num],
			sizeof(t_point) * ls->num_cols);
	}
	else
		error_msg("Wrong line length in map!");
}

void		rows_into_array(t_fdf *ls)
{
	int		i;
	char	**tmp;

	i = 0;
	tmp = ft_strsplit((ls->text)[i], ' ');
	ls->num_cols = ft_arrlen((void **)tmp);
	ft_arrdel((void ***)&tmp);
	ls->arr = (t_point **)ft_newarr(ls->num_rows);
	ls->orig = (t_point **)ft_newarr(ls->num_rows);
	while (i < ls->num_rows)
	{
		convert_row(ls, ft_strsplit((ls->text)[i], ' '), i);
		i++;
	}
}

void		count_save_rows(t_fdf *ls)
{
	int	i;

	ls->fd = open(ls->addr, O_RDONLY);
	while ((ls->ret = get_next_line(ls->fd, &(ls->line))) > 0)
	{
		ft_strdel(&(ls->line));
		(ls->num_rows)++;
	}
	if (ls->fd < 0 || ls->ret < 0)
		error("Error opening the file ");
	if (ls->num_rows == 0)
		error_msg("Error : Empty map file!");
	close(ls->fd);
	i = 0;
	ls->fd = open(ls->addr, O_RDONLY);
	ls->text = (char **)ft_newarr(ls->num_rows);
	while ((ls->ret = get_next_line(ls->fd, &(ls->line))) > 0)
	{
		(ls->text)[i] = ls->line;
		i++;
	}
	close(ls->fd);
}

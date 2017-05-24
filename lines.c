/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lines.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nekitoss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/24 21:23:04 by nekitoss          #+#    #+#             */
/*   Updated: 2017/05/24 21:23:04 by nekitoss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "h_fdf.h"

void		line1(t_fdf *ls, int color)
{
	ls->d = (abs(ls->y2 - ls->y1) << 1) - abs(ls->x2 - ls->x1);
	ls->d1 = abs(ls->y2 - ls->y1) << 1;
	ls->d2 = (abs(ls->y2 - ls->y1) - abs(ls->x2 - ls->x1)) << 1;
	pixel_to_img(ls, ls->x1, ls->y1, color);
	ls->x = ls->x1 + (ls->x2 >= ls->x1 ? 1 : -1);
	ls->y = ls->y1;
	ls->i = 1;
	while (ls->i <= abs(ls->x2 - ls->x1))
	{
		if (ls->d > 0)
		{
			ls->d += ls->d2;
			ls->y += (ls->y2 >= ls->y1 ? 1 : -1);
		}
		else
			ls->d += ls->d1;
		pixel_to_img(ls, ls->x, ls->y, color);
		ls->i++;
		ls->x += (ls->x2 >= ls->x1 ? 1 : -1);
	}
}

void		line2(t_fdf *ls, int color)
{
	ls->d = (abs(ls->x2 - ls->x1) << 1) - abs(ls->y2 - ls->y1);
	ls->d1 = abs(ls->x2 - ls->x1) << 1;
	ls->d2 = (abs(ls->x2 - ls->x1) - abs(ls->y2 - ls->y1)) << 1;
	pixel_to_img(ls, ls->x1, ls->y1, color);
	ls->y = ls->y1 + (ls->y2 >= ls->y1 ? 1 : -1);
	ls->x = ls->x1;
	ls->i = 1;
	while (ls->i <= abs(ls->y2 - ls->y1))
	{
		if (ls->d > 0)
		{
			ls->d += ls->d2;
			ls->x += (ls->x2 >= ls->x1 ? 1 : -1);
		}
		else
			ls->d += ls->d1;
		pixel_to_img(ls, ls->x, ls->y, color);
		ls->i++;
		ls->y += (ls->y2 >= ls->y1 ? 1 : -1);
	}
}

void		line(t_fdf *ls, int color)
{
	if (abs(ls->y2 - ls->y1) <= abs(ls->x2 - ls->x1))
	{
		line1(ls, color);
	}
	else
	{
		line2(ls, color);
	}
}

void		make_lines(t_fdf *ls)
{
	int	i;
	int	j;

	i = -1;
	while (++i < ls->num_rows)
	{
		j = -1;
		while (++j < ls->num_cols)
		{
			ls->x1 = (ls->arr[i][j]).x;
			ls->y1 = (ls->arr[i][j]).y;
			ls->y2 = (ls->arr[i][(j + ((j < ls->num_cols - 1) ? 1 : 0))]).y;
			ls->x2 = (ls->arr[i][(j + ((j < ls->num_cols - 1) ? 1 : 0))]).x;
			if (!((ABS(ls->x1) > ls->size_y && ABS(ls->x2) > ls->size_y) ||
				(ABS(ls->y1) > ls->size_x && ABS(ls->y2) > ls->size_x)))
				line(ls, (ls->arr[i][j]).color);
			ls->x1 = (ls->arr[i][j]).x;
			ls->y1 = (ls->arr[i][j]).y;
			ls->y2 = (ls->arr[(i + ((i < ls->num_rows - 1) ? 1 : 0))][j]).y;
			ls->x2 = (ls->arr[(i + ((i < ls->num_rows - 1) ? 1 : 0))][j]).x;
			if (!((ABS(ls->x1) > ls->size_y && ABS(ls->x2) > ls->size_y) ||
				(ABS(ls->y1) > ls->size_x && ABS(ls->y2) > ls->size_x)))
				line(ls, (ls->arr[i][j]).color);
		}
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nekitoss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/24 21:21:21 by nekitoss          #+#    #+#             */
/*   Updated: 2017/05/24 21:21:25 by nekitoss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "h_fdf.h"

FLOAT_T		rad(double deg)
{
	return (deg * M_PI / 180.0);
}

void		calculate_angels(int i, int j, t_fdf *ls)
{
	FLOAT_T x;
	FLOAT_T y;
	FLOAT_T z;

	y = ((ls->orig[i][j]).x * cos(rad(ls->angle_y)) -
			(ls->orig[i][j]).y * sin(rad(ls->angle_y))) * ls->zoom;
	x = ((ls->orig[i][j]).x * sin(rad(ls->angle_y)) +
			(ls->orig[i][j]).y * cos(rad(ls->angle_y))) * ls->zoom;
	(ls->arr[i][j]).x = x;
	(ls->arr[i][j]).y = y;
	z = ((ls->orig[i][j]).z * ls->zoom * cos(rad(ls->angle_z)) -
			(ls->arr[i][j]).x * sin(rad(ls->angle_z)));
	x = ((ls->orig[i][j]).z * ls->zoom * sin(rad(ls->angle_z)) +
			(ls->arr[i][j]).x * cos(rad(ls->angle_z)));
	(ls->arr[i][j]).x = x;
	(ls->arr[i][j]).z = z;
	y = ((ls->arr[i][j]).y * cos(rad(ls->angle_x)) -
			(ls->arr[i][j]).z * sin(rad(ls->angle_x)));
	z = ((ls->arr[i][j]).y * sin(rad(ls->angle_x)) +
			(ls->arr[i][j]).z * cos(rad(ls->angle_x)));
	(ls->arr[i][j]).z = z;
	(ls->arr[i][j]).y = y + ls->shift_y;
	(ls->arr[i][j]).x += ls->shift_x;
}

void		rotate(t_fdf *ls)
{
	int		i;
	int		j;

	i = 0;
	while (i < ls->num_rows)
	{
		j = 0;
		while (j < ls->num_cols)
		{
			calculate_angels(i, j, ls);
			pixel_to_img(ls, (ls->arr[i][j]).x, (ls->arr[i][j]).y,
				(ls->arr[i][j]).color);
			j++;
		}
		i++;
	}
}

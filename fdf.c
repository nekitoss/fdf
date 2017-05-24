/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nekitoss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/24 21:21:53 by nekitoss          #+#    #+#             */
/*   Updated: 2017/05/24 21:21:55 by nekitoss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "h_fdf.h"

void		key1(int key, t_fdf *ls)
{
	if (key == 69 || key == 24)
		ls->zoom *= 1.5;
	if (key == 78 || key == 27)
		ls->zoom *= 0.5;
	if (key == 82 || key == 29)
		ls->zoom = 1;
	if (key == 0)
		(ls->angle_x) -= 10;
	if (key == 2)
		(ls->angle_x) += 10;
	if (key == 1)
		(ls->angle_z) -= 10;
	if (key == 13)
		(ls->angle_z) += 10;
	if (key == 12)
		(ls->angle_y) -= 10;
	if (key == 14)
		(ls->angle_y) += 10;
	if (key == 123)
		(ls->shift_y) -= 0.05 * ls->size_y;
	if (key == 124)
		(ls->shift_y) += 0.05 * ls->size_y;
	if (key == 125)
		(ls->shift_x) += 0.05 * ls->size_x;
}

int			key_f(int key, t_fdf *ls)
{
	key1(key, ls);
	if (key == 126)
		(ls->shift_x) -= 0.05 * ls->size_x;
	if (key == 8)
		ls->color_off = ls->color_off ^ 1;
	if (key == 37)
		ls->lines_off = ls->lines_off ^ 1;
	if (key == 48)
		ls->help_on = ls->help_on ^ 1;
	if (key == 15)
	{
		ls->angle_x = 0;
		ls->angle_y = 0;
		ls->angle_z = 0;
	}
	if (key == 53)
	{
		mlx_destroy_window(ls->mlx_ptr, ls->win_ptr);
		exit(0);
	}
	redraw(ls);
	return (0);
}

void		init_struct(t_fdf *ls)
{
	count_save_rows(ls);
	rows_into_array(ls);
	make_color(ls);
	ls->mlx_ptr = mlx_init();
	ls->angle_z = 210;
	ls->angle_x = -170;
	ls->angle_y = -210;
	ls->size_x = 2560;
	ls->size_y = 1315;
	ls->zoom = ceil(MIN(ls->size_x / (ls->num_rows - 1),
		ls->size_y / (ls->num_cols - 1)));
	ls->center_x = ls->size_x / 2;
	ls->center_y = ls->size_y / 2;
	ls->shift_x = ls->center_y;
}

int			main(int argc, char **argv)
{
	t_fdf	*ls;

	ls = (t_fdf *)ft_memalloc(sizeof(t_fdf));
	if (argc == 2)
	{
		ls->addr = argv[1];
		init_struct(ls);
		if (ls->mlx_ptr)
		{
			ls->win_ptr = mlx_new_window(ls->mlx_ptr, ls->size_x + 19,
				ls->size_y + 19, ls->addr);
			if (ls->win_ptr)
			{
				redraw(ls);
				mlx_hook(ls->win_ptr, 2, 5, key_f, ls);
				mlx_loop(ls->mlx_ptr);
			}
		}
		else
			error("MLX error:");
	}
	else
		ft_putendl("Usage : ./fdf <filename>");
	return (0);
}

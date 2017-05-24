/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nekitoss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/24 21:22:41 by nekitoss          #+#    #+#             */
/*   Updated: 2017/05/24 21:22:42 by nekitoss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "h_fdf.h"

void		pixel_to_img(t_fdf *ls, int i, int j, int color)
{
	int *tmp;

	tmp = (int *)&(ls->data[IMG_ROW(i) + IMG_COL(j)]);
	if ((i < ls->size_y) && (j < ls->size_x) && i >= 0 && j >= 0)
	{
		if (ls->color_off)
			*tmp = WHITE;
		else
			*tmp = color;
	}
}

void		redraw(t_fdf *ls)
{
	mlx_clear_window(ls->mlx_ptr, ls->win_ptr);
	if (ls->img_ptr)
		mlx_destroy_image(ls->mlx_ptr, ls->img_ptr);
	ls->img_ptr = mlx_new_image(ls->mlx_ptr, ls->size_x, ls->size_y);
	if (!(ls->img_ptr))
		error("IMG_not created");
	ls->data = mlx_get_data_addr(ls->img_ptr, &(ls->bits_per_pixel),
		&(ls->size_line), &(ls->endian));
	rotate(ls);
	if (!(ls->lines_off))
		make_lines(ls);
	mlx_put_image_to_window(ls->mlx_ptr, ls->win_ptr, ls->img_ptr, 10, 10);
	if (ls->help_on)
		print_help(ls);
	else
		mlx_string_put(ls->mlx_ptr, ls->win_ptr, 0, 0, 0x1000FFFF,
			"press TAB to show/hide help");
}

void		make_image(t_fdf *ls)
{
	int	i;
	int	j;

	i = 0;
	while (i < ls->num_rows && (i * ls->zoom) < ls->size_y)
	{
		j = 0;
		while (j < ls->num_cols && (j * ls->zoom) < ls->size_x)
		{
			(ls->arr[i][j]).x = (ls->orig[i][j]).x * ls->zoom;
			(ls->arr[i][j]).y = (ls->orig[i][j]).x * ls->zoom;
			if ((ls->arr[i][j]).z)
				pixel_to_img(ls, (i * ls->zoom), (j * ls->zoom), 0x0000FFFF);
			else
				pixel_to_img(ls, (i * ls->zoom), (j * ls->zoom), 0x00FF0000);
			j++;
		}
		i++;
	}
}

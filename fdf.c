#include "h_fdf.h"

void		error(char *str)
{
	perror(str);
	exit(1);
}

void		error_msg(char *str)
{
	ft_putendl(str);
	exit(1);
}

FLOAT_T		rad(double deg)
{
	return (deg * M_PI / 180.0);
}

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
	printf("max_h = %d; min_h = %d\n", ls->max_h, ls->min_h);
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

void		print_help(t_fdf *ls)
{
	mlx_string_put(ls->mlx_ptr, ls->win_ptr, 0, 0, 0x1000FFFF,
		"W, A, S, D, Q, E : rotating in 3 axis");
	mlx_string_put(ls->mlx_ptr, ls->win_ptr, 0, 20, 0x1000FFFF,
		"arrows : moving around");
	mlx_string_put(ls->mlx_ptr, ls->win_ptr, 0, 40, 0x1000FFFF,
		"- , +, 0 : for zoom");
	mlx_string_put(ls->mlx_ptr, ls->win_ptr, 0, 60, 0x1000FFFF,
		"C : disable colors");
	mlx_string_put(ls->mlx_ptr, ls->win_ptr, 0, 80, 0x1000FFFF,
		"R : reset angles to zero");
	mlx_string_put(ls->mlx_ptr, ls->win_ptr, 0, 100, 0x1000FFFF,
		"L : enable / disable lines");
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
